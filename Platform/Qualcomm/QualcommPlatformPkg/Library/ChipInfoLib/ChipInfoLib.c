/** @file
  This file implements the parts of the ChipInfo driver specific to UEFI
  PUBLIC CLASSES:
    ChipInfo_Init

  Copyright (c) Qualcomm Technologies, Inc. and/or its subsidiaries.
  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

/*==============================================================================
      Include Files
==============================================================================*/
#include <Uefi.h>
#include <Base.h>
#include <Library/BaseLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/ChipInfoLib.h>
#include <PlatformInfoDefs.h>
#include "ChipInfoLocal.h"
#include "ChipInfoImage.h"

/*=========================================================================
      Data Definitions
==========================================================================*/

/*
 * Internal structure to hold chip info.
 *
 * During XBL Core, ChipInfoDxe init pulls data from the SMEM region
 * and stores it in here.
 *
 * The query APIs read data from this structure.
 */
ChipInfoCtxtType  ChipInfoCtxt;

/*==============================================================================
      Functions
==============================================================================*/
/**
  Initialize the ChipInfo driver.

  Any missing or unknown information will be indicated as specified
  in the individual API documentation.

  @return  CHIPINFO_SUCCESS always.
**/
ChipInfoResult
ChipInfo_Init (
  VOID
  )
{
  UINT32                    *aCPUClusters, *aDisabledFeaturesArray, *aCPUCoresArray;
  UINT32                    nClusterArraySize;
  PlatformInfoSMemType      *pSMem;
  PlatformInfoPartInfoType  *pPartInfo;
  UINT32                    nSize;

  if (ChipInfoCtxt.bInitComplete) {
    return CHIPINFO_SUCCESS;
  }

  // Get the address of the SoCInfo structure using the
  // image-specific API
  pSMem = ChipInfo_GetSocInfo ();

  if (pSMem == NULL) {
    // This will only happen in pre-sil on a standalone build.
    // Return SUCCESS using the UNKNOWN values for each field,
    // and let the information be set manually. However, mark the driver as
    // initialized, to record that this has happened.
    AsciiStrCpyS (ChipInfoCtxt.szChipIdString, CHIPINFO_MAX_ID_LENGTH, "UNKNOWN");
    ChipInfoCtxt.bInitComplete = TRUE;
    return CHIPINFO_SUCCESS;
  }

  // Read the chip information from SMem
  ChipInfoCtxt.nRawPartNum     = pSMem->nRawChipId;
  ChipInfoCtxt.eChipInfoId     = (ChipInfoIdType)pSMem->eChipId;
  ChipInfoCtxt.nRevNumber      = pSMem->nRawChipVersion;
  ChipInfoCtxt.nVersion        = pSMem->nChipVersion;
  ChipInfoCtxt.eFamilyId       = (ChipInfoFamilyType)pSMem->nChipFamily;
  ChipInfoCtxt.nFamilyDeviceId = pSMem->nRawDeviceNumber;
  ChipInfoCtxt.nRawFamilyId    = pSMem->nRawDeviceFamily;
  ChipInfoCtxt.eFoundryId      = (ChipInfoFoundryIdType)pSMem->nFoundryId;
  ChipInfoCtxt.nSerialNum      = pSMem->nChipSerial;
  ChipInfoCtxt.nQFPROMChipId   = pSMem->nQFPROMChipId;
  ChipInfoCtxt.nModemSupported = pSMem->nModemSupported;
  AsciiStrCpyS (ChipInfoCtxt.szChipIdString, CHIPINFO_MAX_ID_LENGTH, pSMem->szChipIdStr);

  if (pSMem->nFormat >= PLATFORMINFO_VERSION (0, 14)) {
    ChipInfoCtxt.nNumClusters = pSMem->nNumClusters;

    // Avoid crashes from DALSYS_Malloc on some images if size == 0
    if (ChipInfoCtxt.nNumClusters > 0) {
      aCPUClusters      = (UINT32 *)((UINTN *)pSMem + pSMem->nClusterArrayOffset);
      nClusterArraySize = ChipInfoCtxt.nNumClusters * sizeof (UINT32);

      ChipInfoCtxt.aCPUClusters = AllocatePages (EFI_SIZE_TO_PAGES (nClusterArraySize));
      if (ChipInfoCtxt.aCPUClusters != NULL) {
        CopyMem (ChipInfoCtxt.aCPUClusters, aCPUClusters, nClusterArraySize);
      }
    }

    aDisabledFeaturesArray = (UINT32 *)((UINTN *)pSMem + pSMem->nDisabledFeaturesArrayOffset);
    CopyMem (
      ChipInfoCtxt.aDisabledFeatures,
      aDisabledFeaturesArray,
      pSMem->nNumParts * sizeof (UINT32)
      );
  }

  if (pSMem->nFormat >= PLATFORMINFO_VERSION (0, 16)) {
    ChipInfoCtxt.eFeatureCode = pSMem->eFeatureCode;
    ChipInfoCtxt.ePCode       = pSMem->ePCode;
  }

  if (pSMem->nFormat >= PLATFORMINFO_VERSION (0, 19)) {
    ChipInfoCtxt.nNumFunctionalClusters = pSMem->nNumFunctionalClusters;
    ChipInfoCtxt.nBootCluster           = pSMem->nBootCluster;
    ChipInfoCtxt.nBootCore              = pSMem->nBootCore;
  }

  if (pSMem->nFormat >= PLATFORMINFO_VERSION (0, 20)) {
    ChipInfoCtxt.nRawPackageType = pSMem->nRawPackageType;
  }

  if (pSMem->nFormat >= PLATFORMINFO_VERSION (0, 21)) {
    aDisabledFeaturesArray = (UINT32 *)((UINTN *)pSMem + pSMem->nPartialFeaturesArrayOffset);
    CopyMem (
      ChipInfoCtxt.aPartialFeatures,
      aDisabledFeaturesArray,
      pSMem->nNumParts * sizeof (UINT32)
      );
  } else {
    CopyMem (
      ChipInfoCtxt.aPartialFeatures,
      ChipInfoCtxt.aDisabledFeatures,
      CHIPINFO_NUM_PARTS * sizeof (UINT32)
      );
  }

  if (pSMem->nFormat >= PLATFORMINFO_VERSION (0, 22)) {
    aCPUCoresArray = (UINT32 *)((UINTN *)pSMem + pSMem->nCPUCoresArrayOffset);
    /*aCPUClusterCoresArray contains no.of cores per cluster for all clusters */
    nClusterArraySize      = ChipInfoCtxt.nNumClusters * sizeof (UINT32);
    ChipInfoCtxt.aNumCores = AllocatePages (EFI_SIZE_TO_PAGES (nClusterArraySize));
    if (ChipInfoCtxt.aNumCores != NULL) {
      CopyMem (ChipInfoCtxt.aNumCores, aCPUCoresArray, nClusterArraySize);
    }
  }

  if (pSMem->nFormat >= PLATFORMINFO_VERSION (0, 23)) {
    nSize                  = pSMem->nNumPartInstances * sizeof (PlatformInfoPartInfoType);
    ChipInfoCtxt.aPartInfo = AllocatePages (EFI_SIZE_TO_PAGES (nSize));
    if (ChipInfoCtxt.aPartInfo) {
      pPartInfo = (PlatformInfoPartInfoType *)((UINTN *)pSMem + pSMem->anPartInstancesOffset);
      CopyMem (ChipInfoCtxt.aPartInfo, pPartInfo, nSize);
      ChipInfoCtxt.nPartInfoLen = pSMem->nNumPartInstances;
    }
  }

  ChipInfo_UnmapSMem ();
  ChipInfoCtxt.bInitComplete = TRUE;
  return CHIPINFO_SUCCESS;
} /* END ChipInfo_DriverInit */
