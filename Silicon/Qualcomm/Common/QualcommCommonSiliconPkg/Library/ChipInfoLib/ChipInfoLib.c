/** @file
  This file implements the parts of the ChipInfo driver specific to UEFI.

  @par Glossary:
    - Qfprom - Qualcomm Fuse Programmable Read-Only Memory
    - Smem   - Shared Memory

  Copyright (c) Qualcomm Technologies, Inc. and/or its subsidiaries.<BR>
  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

/*==============================================================================
      Include Files
==============================================================================*/
#include <Uefi.h>
#include <Base.h>
#include <Library/BaseLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/ChipInfoLib.h>
#include <Library/MemoryAllocationLib.h>
#include <PlatformInfoDefs.h>
#include "ChipInfoImage.h"
#include "ChipInfoLocal.h"

/*=========================================================================
      Data Definitions
==========================================================================*/

/**
  Internal structure to hold chip info.

  During XBL Core, ChipInfoDxe init pulls data from the SMEM region
  and stores it in here.

  The query APIs read data from this structure.
**/
ChipInfoCtxtType  mChipInfoCtxt;

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
EFIAPI
ChipInfoInit (
  VOID
  )
{
  UINT32                    *CpuClusters;
  UINT32                    *DisabledFeaturesArray;
  UINT32                    *CpuCoresArray;
  UINT32                    ClusterArraySize;
  PlatformInfoSmemType      *Smem;
  PlatformInfoPartInfoType  *PartInfo;
  UINT32                    Size;

  if (mChipInfoCtxt.InitComplete) {
    return CHIPINFO_SUCCESS;
  }

  // Get the address of the SoCInfo structure using the
  // image-specific API
  Smem = ChipInfoGetSocInfo ();

  if (Smem == NULL) {
    // This will only happen in pre-sil on a standalone build.
    // Return SUCCESS using the UNKNOWN values for each field,
    // and let the information be set manually. However, mark the driver as
    // initialized, to record that this has happened.
    AsciiStrCpyS (mChipInfoCtxt.ChipIdString, CHIPINFO_MAX_ID_LENGTH, "UNKNOWN");
    mChipInfoCtxt.InitComplete = TRUE;
    return CHIPINFO_SUCCESS;
  }

  // Read the chip information from Smem
  mChipInfoCtxt.RawPartNum     = Smem->RawChipId;
  mChipInfoCtxt.ChipInfoId     = (ChipInfoIdType)Smem->ChipId;
  mChipInfoCtxt.RevNumber      = Smem->RawChipVersion;
  mChipInfoCtxt.Version        = Smem->ChipVersion;
  mChipInfoCtxt.FamilyId       = (ChipInfoFamilyType)Smem->ChipFamily;
  mChipInfoCtxt.FamilyDeviceId = Smem->RawDeviceNumber;
  mChipInfoCtxt.RawFamilyId    = Smem->RawDeviceFamily;
  mChipInfoCtxt.FoundryId      = (ChipInfoFoundryIdType)Smem->FoundryId;
  mChipInfoCtxt.SerialNum      = Smem->ChipSerial;
  mChipInfoCtxt.QfpromChipId   = Smem->QfpromChipId;
  mChipInfoCtxt.ModemSupported = Smem->ModemSupported;
  AsciiStrCpyS (mChipInfoCtxt.ChipIdString, CHIPINFO_MAX_ID_LENGTH, Smem->ChipIdStr);

  if (Smem->Format >= PLATFORMINFO_VERSION (0, 14)) {
    mChipInfoCtxt.NumClusters = Smem->NumClusters;

    // Avoid crashes from DALSYS_Malloc on some images if size == 0
    if (mChipInfoCtxt.NumClusters > 0) {
      CpuClusters      = (UINT32 *)((UINTN *)Smem + Smem->ClusterArrayOffset);
      ClusterArraySize = mChipInfoCtxt.NumClusters * sizeof (UINT32);

      mChipInfoCtxt.CpuClusters = AllocatePages (EFI_SIZE_TO_PAGES (ClusterArraySize));
      if (mChipInfoCtxt.CpuClusters != NULL) {
        CopyMem (mChipInfoCtxt.CpuClusters, CpuClusters, ClusterArraySize);
      }
    }

    DisabledFeaturesArray = (UINT32 *)((UINTN *)Smem + Smem->DisabledFeaturesArrayOffset);
    CopyMem (
      mChipInfoCtxt.DisabledFeatures,
      DisabledFeaturesArray,
      Smem->NumParts * sizeof (UINT32)
      );
  }

  if (Smem->Format >= PLATFORMINFO_VERSION (0, 16)) {
    mChipInfoCtxt.FeatureCode = Smem->FeatureCode;
    mChipInfoCtxt.PCode       = Smem->PCode;
  }

  if (Smem->Format >= PLATFORMINFO_VERSION (0, 19)) {
    mChipInfoCtxt.NumFunctionalClusters = Smem->NumFunctionalClusters;
    mChipInfoCtxt.BootCluster           = Smem->BootCluster;
    mChipInfoCtxt.BootCore              = Smem->BootCore;
  }

  if (Smem->Format >= PLATFORMINFO_VERSION (0, 20)) {
    mChipInfoCtxt.RawPackageType = Smem->RawPackageType;
  }

  if (Smem->Format >= PLATFORMINFO_VERSION (0, 21)) {
    DisabledFeaturesArray = (UINT32 *)((UINTN *)Smem + Smem->PartialFeaturesArrayOffset);
    CopyMem (
      mChipInfoCtxt.PartialFeatures,
      DisabledFeaturesArray,
      Smem->NumParts * sizeof (UINT32)
      );
  } else {
    CopyMem (
      mChipInfoCtxt.PartialFeatures,
      mChipInfoCtxt.DisabledFeatures,
      CHIPINFO_NUM_PARTS * sizeof (UINT32)
      );
  }

  if (Smem->Format >= PLATFORMINFO_VERSION (0, 22)) {
    CpuCoresArray = (UINT32 *)((UINTN *)Smem + Smem->CpuCoresArrayOffset);
    // CpuCoresArray contains no. of cores per cluster for all clusters
    ClusterArraySize        = mChipInfoCtxt.NumClusters * sizeof (UINT32);
    mChipInfoCtxt.NumCores = AllocatePages (EFI_SIZE_TO_PAGES (ClusterArraySize));
    if (mChipInfoCtxt.NumCores != NULL) {
      CopyMem (mChipInfoCtxt.NumCores, CpuCoresArray, ClusterArraySize);
    }
  }

  if (Smem->Format >= PLATFORMINFO_VERSION (0, 23)) {
    Size                    = Smem->NumPartInstances * sizeof (PlatformInfoPartInfoType);
    mChipInfoCtxt.PartInfo = AllocatePages (EFI_SIZE_TO_PAGES (Size));
    if (mChipInfoCtxt.PartInfo) {
      PartInfo = (PlatformInfoPartInfoType *)((UINTN *)Smem + Smem->PartInstancesOffset);
      CopyMem (mChipInfoCtxt.PartInfo, PartInfo, Size);
      mChipInfoCtxt.PartInfoLen = Smem->NumPartInstances;
    }
  }

  ChipInfoUnmapSmem ();
  mChipInfoCtxt.InitComplete = TRUE;
  return CHIPINFO_SUCCESS;
}
