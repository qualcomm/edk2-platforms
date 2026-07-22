/** @file
 This file implements the Chip Info APIs common to both XBL Loader and Core

 PUBLIC CLASSES:
 ChipInfo_GetChipVersion
 ChipInfo_GetRawChipVersion
 ChipInfo_GetChipId
 ChipInfo_GetRawChipId
 ChipInfo_GetChipIdString
 ChipInfo_GetChipFamily
 ChipInfo_GetMarketingNameString
 ChipInfo_GetModemSupport
 ChipInfo_GetSerialNumber
 ChipInfo_GetFoundryId
 ChipInfo_GetRawDeviceFamily
 ChipInfo_GetRawDeviceNumber
 ChipInfo_GetQFPROMChipId
 ChipInfo_GetDisabledCPUs
 ChipInfo_GetSKU
 ChipInfo_GetDisabledFeatures
 ChipInfo_IsPartDisabled

 Copyright (c) Qualcomm Technologies, Inc. and/or its subsidiaries.
 SPDX-License-Identifier: BSD-2-Clause-Patent
**/

/*=========================================================================
 Include Files
 ==========================================================================*/

#include <Base.h>
#include "ChipInfoDefs.h"
#include "ChipInfoLocal.h"
#include "ChipInfoImage.h"

/*==============================================================================
        DEFINES
==============================================================================*/

#define UNUSED(x)  (VOID)(x)

/*=========================================================================
      Data Definitions
==========================================================================*/
extern ChipInfoCtxtType  ChipInfoCtxt;

/*=========================================================================
 Functions
 ==========================================================================*/
/**
  Returns the version of the chip as a ChipInfoVersionType.

  @return  Chip version if successful,
           CHIPINFO_VERSION_UNKNOWN if called before ChipInfo is initialized.
**/
ChipInfoVersionType
ChipInfo_GetChipVersion (
  VOID
  )
{
  return ChipInfoCtxt.nVersion;
} /* END ChipInfo_GetChipVersion */

/**
  Returns the version of the chip as read from fuses.

  This raw version is a strictly increasing integer counter incremented for
  each new HW version (e.g. v1.0 = 0, v1.1 = 1, v2.0 = 3, ...).

  The only use case for this API is for patch revisions, i.e., the 'z' in
  vX.Y.Z. Patch revisions are not reflected in the value returned from
  ChipInfo_GetChipVersion, but are included in the HW revision counter read
  by this function. Use ChipInfo_GetChipVersion for everything else.

  @return  Chip version as read from HW, or
           CHIPINFO_RAW_VERSION_UNKNOWN if called before ChipInfo is initialized.
**/
UINT32
ChipInfo_GetRawChipVersion (
  VOID
  )
{
  return ChipInfoCtxt.nRevNumber;
} /* END ChipInfo_GetRawChipVersion */

/**
  Returns the chip ID as a ChipInfoIdType.

  @return  Chip ID associated with the part number read from HW, or
           CHIPINFO_ID_UNKNOWN if no associated chip ID could be found,
           or if called before ChipInfo is initialized.
**/
ChipInfoIdType
ChipInfo_GetChipId (
  VOID
  )
{
  return ChipInfoCtxt.eChipInfoId;
} /*  END ChipInfo_GetChipId  */

/**
  Returns the chip ID as read from HW (i.e. the JTAG ID).

  Rarely needed, except as a workaround until ChipInfoIdType is updated
  with support for new chips. Use ChipInfo_GetChipId instead.

  @return  Chip ID as read from HW, or
           CHIPINFO_RAW_ID_UNKNOWN if called before ChipInfo is initialized.
**/
UINT32
ChipInfo_GetRawChipId (
  VOID
  )
{
  return ChipInfoCtxt.nRawPartNum;
} /*  END ChipInfo_GetRawChipId    */

/**
  Returns the name of the chip (e.g. MSM8998, MDM9665).

  @param[out]  szChipIdStr  Pointer to a buffer to hold the chip's name. If
                            called before ChipInfo is initialized, or if the
                            chip ID string is unknown, "UNKNOWN" will be
                            copied into this buffer.
  @param[in]   nMaxLen      Length of the string to copy. Max supported size
                            is CHIPINFO_MAX_ID_LENGTH.

  @return  CHIPINFO_SUCCESS always.
**/
ChipInfoResult
ChipInfo_GetChipIdString (
  CHAR8   *szChipIdStr,
  UINT32  nMaxLen
  )
{
  UINT32  len;

  len = 0;

  if (nMaxLen < CHIPINFO_MAX_ID_LENGTH) {
    len = nMaxLen;
  } else {
    len = CHIPINFO_MAX_ID_LENGTH;
  }

  ChipInfo_strcpy (szChipIdStr, ChipInfoCtxt.szChipIdString, len);
  return CHIPINFO_SUCCESS;
} /*  END ChipInfo_GetChipIdString    */

/**
  Returns the chip's family as a ChipInfoFamilyType.

  @return  Chip family if successful, or
           CHIPINFO_FAMILY_UNKNOWN if no associated chip family can be found,
           or if called before ChipInfo is initialized.
**/
ChipInfoFamilyType
ChipInfo_GetChipFamily (
  VOID
  )
{
  return ChipInfoCtxt.eFamilyId;
} /* END Chipinfo_GetChipFamily */

/**
  Returns the marketing name of the processor.

  This is typically the same as the ChipIdString.

  @param[out]  pszMarketingNameStr  Pointer to a buffer to hold the
                                    processor's name. If called before
                                    ChipInfo is initialized, or if the
                                    marketing name is unknown, "UNKNOWN"
                                    will be copied into this buffer.
  @param[in]   nMaxlen              Length of the name buffer. At most
                                    CHIPINFO_MAX_NAME_LEN bytes will be
                                    copied.

  @return  CHIPINFO_SUCCESS always.
**/
ChipInfoResult
ChipInfo_GetMarketingNameString (
  CHAR8   *pszMarketingNameStr,
  UINT32  nMaxlen
  )
{
  ChipInfo_GetChipIdString (pszMarketingNameStr, nMaxlen);
  return CHIPINFO_SUCCESS;
} /*  END ChipInfo_GetMarketingNameString    */

/**
  Returns whether or not the chip supports a modem.

  The value returned should be treated as indicating the presence of a modem:
  0 = modem not supported, nonzero = modem supported.

  @return  Non-zero if modems are supported for this chip, or
           CHIPINFO_MODEM_UNKNOWN if modems are not supported or if called
           before ChipInfo is initialized.
**/
ChipInfoModemType
ChipInfo_GetModemSupport (
  VOID
  )
{
  return ChipInfoCtxt.nModemSupported;
} /* END Chipinfo_GetModemSupport */

/**
  Returns the chip's serial number.

  Serial numbers are only unique within a given commercial product family.
  Use ChipInfo_GetQFPROMChipId to find the commercial product family.
  Combining the two values gives a globally unique ID for this device.

  @return  Serial number as read from HW, or
           CHIPINFO_SERIAL_NUM_UNKNOWN if called before ChipInfo is initialized.
**/
ChipInfoSerialNumType
ChipInfo_GetSerialNumber (
  VOID
  )
{
  return ChipInfoCtxt.nSerialNum;
} /*  END ChipInfo_GetSerialNumber    */

/**
  Returns the foundry ID for the chip as a ChipInfoFoundryIdType.

  @return  Foundry ID if successful, or
           CHIPINFO_FOUNDRYID_UNKNOWN if called before ChipInfo is initialized.
**/
ChipInfoFoundryIdType
ChipInfo_GetFoundryId (
  VOID
  )
{
  return ChipInfoCtxt.eFoundryId;
} /*  END ChipInfo_GetFoundryId  */

/**
  Returns the chip's architecture family number as read from HW.

  @return  Architectural family as read from HW, or
           CHIPINFO_RAW_DEVICE_FAMILY_UNKNOWN if called before ChipInfo is
           initialized.
**/
UINT32
ChipInfo_GetRawDeviceFamily (
  VOID
  )
{
  return ChipInfoCtxt.nRawFamilyId;
} /* END ChipInfo_GetRawDeviceFamily */

/**
  Returns the chip's device ID within its architectural family.

  Functionally, this is a generation counter.

  @return  Device number as read from HW, or
           CHIPINFO_RAW_DEVICE_NUMBER_UNKNOWN if called before ChipInfo is
           initialized.
**/
UINT32
ChipInfo_GetRawDeviceNumber (
  VOID
  )
{
  return ChipInfoCtxt.nFamilyDeviceId;
} /* END ChipInfo_GetRawDeviceFamily */

/**
  Returns the chip's QFPROM chip ID.

  Can be used along with the serial number to uniquely identify the chip.

  @return  Chip ID as read from QFPROM fuses if successful, or
           CHIPINFO_QFPROM_CHIPID_UNKNOWN if called before ChipInfo is
           initialized.
**/
ChipInfoQFPROMChipIdType
ChipInfo_GetQFPROMChipId (
  VOID
  )
{
  return ChipInfoCtxt.nQFPROMChipId;
} /* END ChipInfo_GetQFPROMChipId */

/**
  Retrieves a mask of CPUs in the specified cluster marked as disabled in
  PTE fuses. If the cluster has fewer than 32 CPUs, the additional bits are
  treated as enabled.

  @param[in]   nCPUCluster  CPU cluster to check.
  @param[out]  pnMask       Pointer to hold the mask of disabled CPUs in the
                            selected cluster.

  @return  CHIPINFO_SUCCESS if successful.
  @return  CHIPINFO_ERROR_OUT_OF_RANGE_PARAMETER if nCPUCluster exceeds the
           number of clusters present.
  @return  CHIPINFO_ERROR for other general errors.
**/
ChipInfoResult
ChipInfo_GetDisabledCPUs (
  UINT32  nCPUCluster,
  UINT32  *pnMask
  )
{
  if (pnMask == NULL) {
    return CHIPINFO_ERROR;
  }

  if ((nCPUCluster == 0) && !ChipInfoCtxt.nNumClusters) {
    /*
     * If this target doesn't have a binning plan, we don't know how many
     * CPU clusters there are for this target, and ChipInfo.nNumClusters
     * will be left at its default value of 0. This is the only case where
     * nNumClusters will be 0.
     *
     * Some clients call this function even if a binning plan isn't defined,
     * because they have target-agnostic code. These clients expect it to
     * return successfully instead of OUT_OF_RANGE for the first cluster (0)
     * when PTE hasn't blown fuses, since there's always at least 1 core
     * that's working (the one that's running this function).
     *
     * ChipInfoCtxt.aCPUClusters will be NULL until it's initialized by
     * the target-specific function. That function will only be defined
     * if we have a binning plan, so it'll still be NULL if we get to this
     * point. We therefore need an explicit case here, where we return
     * successfully with pnMask = "enabled/unknown", instead of
     * just accessing aCPUClusters like the general case below.
     */
    *pnMask = 0;
    return CHIPINFO_SUCCESS;
  }

  if (nCPUCluster >= ChipInfoCtxt.nNumClusters) {
    return CHIPINFO_ERROR_OUT_OF_RANGE_PARAMETER;
  }

  *pnMask = ChipInfoCtxt.aCPUClusters[nCPUCluster];
  return CHIPINFO_SUCCESS;
} /* END ChipInfo_GetDisabledCPUs */

/**
  Gets SKU and product code information for the current device.

  If the SKU_ID and/or P_CODE fuses are not blown, the corresponding
  CHIPINFO_*_UNKNOWN value will be returned.

  @param[out]  pInfo  Pointer to a caller-allocated buffer where SKU
                      information will be stored.

  @return  CHIPINFO_SUCCESS if successful.
  @return  CHIPINFO_ERROR_INVALID_PARAMETER if pInfo is NULL.
  @return  CHIPINFO_ERROR_NOT_SUPPORTED if this target does not support
           SKU information.
**/
ChipInfoResult
ChipInfo_GetSKU (
  ChipInfoSKUType  *pInfo
  )
{
  if (pInfo == NULL) {
    return CHIPINFO_ERROR_INVALID_PARAMETER;
  }

  pInfo->eFeatureCode = ChipInfoCtxt.eFeatureCode;
  pInfo->ePCode       = ChipInfoCtxt.ePCode;

  if ((ChipInfoCtxt.eFeatureCode == CHIPINFO_FEATURE_CODE_UNKNOWN) &&
      (ChipInfoCtxt.ePCode == CHIPINFO_P_CODE_UNKNOWN))
  {
    return CHIPINFO_ERROR_NOT_SUPPORTED;
  }

  return CHIPINFO_SUCCESS;
} /* END ChipInfo_GetSKU */

/**
  Gets the number of functional clusters.

  A cluster is deemed functional if it has at least one functional core.
  There will always be at least 1 functional cluster.

  @param[out]  pnNumClusters  Buffer to store the number of functional clusters.

  @return  CHIPINFO_SUCCESS if successful.
  @return  CHIPINFO_ERROR_INVALID_PARAMETER if pnNumClusters is NULL.
  @return  CHIPINFO_ERROR_NOT_INITIALIZED if called before ChipInfo_Init.
**/
ChipInfoResult
ChipInfo_GetNumFunctionalClusters (
  UINT32  *pnNumClusters
  )
{
  if (pnNumClusters == NULL) {
    return CHIPINFO_ERROR_INVALID_PARAMETER;
  }

  if (ChipInfoCtxt.bInitComplete == FALSE) {
    return CHIPINFO_ERROR_NOT_INITIALIZED;
  }

  *pnNumClusters = ChipInfoCtxt.nNumFunctionalClusters;
  return CHIPINFO_SUCCESS;
}

/**
  Gets the boot cluster and core indices.

  @param[out]  pnCluster  Buffer to store the boot cluster index (zero-indexed).
  @param[out]  pnCore     Buffer to store the boot core index (zero-indexed).

  @return  CHIPINFO_SUCCESS if both pointers were filled correctly.
  @return  CHIPINFO_ERROR_INVALID_PARAMETER if either pointer is NULL.
  @return  CHIPINFO_ERROR_NOT_INITIALIZED if called before ChipInfo_Init.
**/
ChipInfoResult
ChipInfo_GetBootClusterAndCore (
  UINT32  *pnCluster,
  UINT32  *pnCore
  )
{
  if ((pnCluster == NULL) || (pnCore == NULL)) {
    return CHIPINFO_ERROR_INVALID_PARAMETER;
  }

  if (ChipInfoCtxt.bInitComplete == FALSE) {
    return CHIPINFO_ERROR_NOT_INITIALIZED;
  }

  *pnCluster = ChipInfoCtxt.nBootCluster;
  *pnCore    = ChipInfoCtxt.nBootCore;
  return CHIPINFO_SUCCESS;
}

/**
  Gets the HW part-level disabled feature details from fuses.

  The bit-level interpretation of pnMask is specific to the client.

  @param[in]   ePart   The ChipInfoPartType being queried.
  @param[in]   nIdx    Hardware instance of the selected part.
  @param[out]  pnMask  Used to store the part-specific details read from the
                       QTV/PTE region.

  @return  CHIPINFO_SUCCESS if successful.
  @return  CHIPINFO_ERROR_INVALID_PARAMETER if pnMask is NULL or ePart is unknown.
  @return  CHIPINFO_ERROR_OUT_OF_RANGE_PARAMETER if ePart is out of range.
  @return  CHIPINFO_ERROR_NOT_INITIALIZED if called before ChipInfo_Init.
**/
ChipInfoResult
ChipInfo_GetDisabledFeatures (
  ChipInfoPartType  ePart,
  UINT32            nIdx,
  UINT32            *pnMask
  )
{
  UINT32  Index;

  PlatformInfoPartInfoType  *pPartInfo;

  if ((pnMask == NULL) || (ePart == CHIPINFO_PART_UNKNOWN)) {
    return CHIPINFO_ERROR_INVALID_PARAMETER;
  }

  if (ePart >= CHIPINFO_NUM_PARTS) {
    return CHIPINFO_ERROR_OUT_OF_RANGE_PARAMETER;
  }

  if (ChipInfoCtxt.bInitComplete == FALSE) {
    return CHIPINFO_ERROR_NOT_INITIALIZED;
  }

  if (nIdx == 0) {
    *pnMask = ChipInfoCtxt.aPartialFeatures[ePart];
    return CHIPINFO_SUCCESS;
  }

  for (Index = 0; Index < ChipInfoCtxt.nPartInfoLen; Index++) {
    pPartInfo = &ChipInfoCtxt.aPartInfo[Index];
    if ((pPartInfo->ePart == ePart) && (pPartInfo->nInstance == nIdx)) {
      *pnMask = pPartInfo->nDisabledFeatures;
      return CHIPINFO_SUCCESS;
    }
  }

  return CHIPINFO_ERROR_OUT_OF_RANGE_PARAMETER;
} /* END ChipInfo_GetDisabledFeatures */

/**
  Checks if the specified part is completely disabled.

  FALSE indicates the part is either fully or partially enabled.
  TRUE indicates the part is completely disabled.
  Use ChipInfo_GetDisabledFeatures to obtain partial disable information.

  @param[in]   ePart   The ChipInfoPartType being queried.
  @param[in]   nIdx    Hardware instance of the selected part.
  @param[out]  pnMask  Pointer to a BOOLEAN to store the disabled status.

  @return  CHIPINFO_SUCCESS if successful.
  @return  CHIPINFO_ERROR_INVALID_PARAMETER if pnMask is NULL or ePart is unknown.
  @return  CHIPINFO_ERROR_OUT_OF_RANGE_PARAMETER if ePart is out of range.
  @return  CHIPINFO_ERROR_NOT_INITIALIZED if called before ChipInfo_Init.
**/
ChipInfoResult
ChipInfo_IsPartDisabled (
  ChipInfoPartType  ePart,
  UINT32            nIdx,
  BOOLEAN           *pnMask
  )
{
  UINT32  Index;

  PlatformInfoPartInfoType  *pPartInfo;

  if ((pnMask == NULL) || (ePart == CHIPINFO_PART_UNKNOWN)) {
    return CHIPINFO_ERROR_INVALID_PARAMETER;
  }

  if (ePart >= CHIPINFO_NUM_PARTS) {
    return CHIPINFO_ERROR_OUT_OF_RANGE_PARAMETER;
  }

  if (!ChipInfoCtxt.bInitComplete) {
    return CHIPINFO_ERROR_NOT_INITIALIZED;
  }

  if (nIdx == 0) {
    *pnMask = ChipInfoCtxt.aDisabledFeatures[ePart] & 0x1 ? TRUE : FALSE;
    return CHIPINFO_SUCCESS;
  }

  for (Index = 0; Index < ChipInfoCtxt.nPartInfoLen; Index++) {
    pPartInfo = &ChipInfoCtxt.aPartInfo[Index];
    if ((pPartInfo->ePart == ePart) && (pPartInfo->nInstance == nIdx)) {
      *pnMask = pPartInfo->bDisabled;
      return CHIPINFO_SUCCESS;
    }
  }

  return CHIPINFO_ERROR_OUT_OF_RANGE_PARAMETER;
} /* END ChipInfo_IsPartDisabled */

/**
  Returns the raw package type as read from HW.

  @return  Raw package type as read from HW.
**/
UINT32
ChipInfo_GetRawPackageType (
  VOID
  )
{
  return ChipInfoCtxt.nRawPackageType;
} /*  END ChipInfo_GetRawPackageType */

/**
  Gets the number of CPU cores in the specified cluster.

  @param[in]   nCPUCluster  CPU cluster to query.
  @param[out]  pnCores      Pointer to store the number of cores.

  @return  CHIPINFO_SUCCESS if successful.
  @return  CHIPINFO_ERROR_INVALID_PARAMETER if pnCores is NULL.
  @return  CHIPINFO_ERROR_NOT_INITIALIZED if called before ChipInfo_Init.
  @return  CHIPINFO_ERROR_OUT_OF_RANGE_PARAMETER if nCPUCluster is out of range.
**/
ChipInfoResult
ChipInfo_GetNumCPUCores (
  UINT32  nCPUCluster,
  UINT32  *pnCores
  )
{
  if (pnCores == NULL) {
    return CHIPINFO_ERROR_INVALID_PARAMETER;
  }

  if (!ChipInfoCtxt.bInitComplete) {
    return CHIPINFO_ERROR_NOT_INITIALIZED;
  }

  if (nCPUCluster >= ChipInfoCtxt.nNumClusters) {
    return CHIPINFO_ERROR_OUT_OF_RANGE_PARAMETER;
  }

  *pnCores = ChipInfoCtxt.aNumCores[nCPUCluster];

  return CHIPINFO_SUCCESS;
} /* END ChipInfo_GetNumCPUCores */
