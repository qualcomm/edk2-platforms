/** @file
 This file implements the Chip Info APIs.

 @par Glossary:
 - Qfprom - Qualcomm Fuse Programmable Read-Only Memory

 Copyright (c) Qualcomm Technologies, Inc. and/or its subsidiaries.<BR>
 SPDX-License-Identifier: BSD-2-Clause-Patent
**/

/*=========================================================================
 Include Files
 ==========================================================================*/

#include <Base.h>
#include "ChipInfoDefs.h"
#include "ChipInfoImage.h"
#include "ChipInfoLocal.h"

/*=========================================================================
 Functions
 ==========================================================================*/
/**
  Returns the version of the chip as a ChipInfoVersionType.

  @return  Chip version if successful,
           CHIPINFO_VERSION_UNKNOWN if called before ChipInfo is initialized.
**/
ChipInfoVersionType
EFIAPI
ChipInfoGetChipVersion (
  VOID
  )
{
  return mChipInfoCtxt.Version;
}

/**
  Returns the version of the chip as read from fuses.

  This raw version is a strictly increasing integer counter incremented for
  each new HW version (e.g. v1.0 = 0, v1.1 = 1, v2.0 = 3, ...).

  The only use case for this API is for patch revisions, i.e., the 'z' in
  vX.Y.Z. Patch revisions are not reflected in the value returned from
  ChipInfoGetChipVersion, but are included in the HW revision counter read
  by this function. Use ChipInfoGetChipVersion for everything else.

  @return  Chip version as read from HW, or
           CHIPINFO_RAW_VERSION_UNKNOWN if called before ChipInfo is initialized.
**/
UINT32
EFIAPI
ChipInfoGetRawChipVersion (
  VOID
  )
{
  return mChipInfoCtxt.RevNumber;
}

/**
  Returns the chip ID as a ChipInfoIdType.

  @return  Chip ID associated with the part number read from HW, or
           CHIPINFO_ID_UNKNOWN if no associated chip ID could be found,
           or if called before ChipInfo is initialized.
**/
ChipInfoIdType
EFIAPI
ChipInfoGetChipId (
  VOID
  )
{
  return mChipInfoCtxt.ChipInfoId;
}

/**
  Returns the chip ID as read from HW (i.e. the JTAG ID).

  Rarely needed, except as a workaround until ChipInfoIdType is updated
  with support for new chips. Use ChipInfoGetChipId instead.

  @return  Chip ID as read from HW, or
           CHIPINFO_RAW_ID_UNKNOWN if called before ChipInfo is initialized.
**/
UINT32
EFIAPI
ChipInfoGetRawChipId (
  VOID
  )
{
  return mChipInfoCtxt.RawPartNum;
}

/**
  Returns the name of the chip (e.g. MSM8998, MDM9665).

  @param[out]  ChipIdStr  Pointer to a buffer to hold the chip's name. If
                           called before ChipInfo is initialized, or if the
                           chip ID string is unknown, "UNKNOWN" will be
                           copied into this buffer.
  @param[in]   MaxLength  Length of the string to copy. Max supported size
                           is CHIPINFO_MAX_ID_LENGTH.

  @return  CHIPINFO_SUCCESS always.
**/
ChipInfoResult
EFIAPI
ChipInfoGetChipIdString (
  OUT CHAR8   *ChipIdStr,
  IN  UINT32  MaxLength
  )
{
  UINT32  Len;

  Len = 0;

  if (MaxLength < CHIPINFO_MAX_ID_LENGTH) {
    Len = MaxLength;
  } else {
    Len = CHIPINFO_MAX_ID_LENGTH;
  }

  ChipInfoStrcpy (ChipIdStr, mChipInfoCtxt.ChipIdString, Len);
  return CHIPINFO_SUCCESS;
}

/**
  Returns the chip's family as a ChipInfoFamilyType.

  @return  Chip family if successful, or
           CHIPINFO_FAMILY_UNKNOWN if no associated chip family can be found,
           or if called before ChipInfo is initialized.
**/
ChipInfoFamilyType
EFIAPI
ChipInfoGetChipFamily (
  VOID
  )
{
  return mChipInfoCtxt.FamilyId;
}

/**
  Returns the marketing name of the processor.

  This is typically the same as the ChipIdString.

  @param[out]  MarketingNameStr  Pointer to a buffer to hold the
                                  processor's name. If called before
                                  ChipInfo is initialized, or if the
                                  marketing name is unknown, "UNKNOWN"
                                  will be copied into this buffer.
  @param[in]   MaxLength         Length of the name buffer. At most
                                  CHIPINFO_MAX_NAME_LENGTH bytes will be
                                  copied.

  @return  CHIPINFO_SUCCESS always.
**/
ChipInfoResult
EFIAPI
ChipInfoGetMarketingNameString (
  OUT CHAR8   *MarketingNameStr,
  IN  UINT32  MaxLength
  )
{
  ChipInfoGetChipIdString (MarketingNameStr, MaxLength);
  return CHIPINFO_SUCCESS;
}

/**
  Returns whether or not the chip supports a modem.

  The value returned should be treated as indicating the presence of a modem:
  0 = modem not supported, nonzero = modem supported.

  @return  Non-zero if modems are supported for this chip, or
           CHIPINFO_MODEM_UNKNOWN if modems are not supported or if called
           before ChipInfo is initialized.
**/
ChipInfoModemType
EFIAPI
ChipInfoGetModemSupport (
  VOID
  )
{
  return mChipInfoCtxt.ModemSupported;
}

/**
  Returns the chip's serial number.

  Serial numbers are only unique within a given commercial product family.
  Use ChipInfoGetQfpromChipId to find the commercial product family.
  Combining the two values gives a globally unique ID for this device.

  @return  Serial number as read from HW, or
           CHIPINFO_SERIAL_NUM_UNKNOWN if called before ChipInfo is initialized.
**/
ChipInfoSerialNumType
EFIAPI
ChipInfoGetSerialNumber (
  VOID
  )
{
  return mChipInfoCtxt.SerialNum;
}

/**
  Returns the foundry ID for the chip as a ChipInfoFoundryIdType.

  @return  Foundry ID if successful, or
           CHIPINFO_FOUNDRYID_UNKNOWN if called before ChipInfo is initialized.
**/
ChipInfoFoundryIdType
EFIAPI
ChipInfoGetFoundryId (
  VOID
  )
{
  return mChipInfoCtxt.FoundryId;
}

/**
  Returns the chip's architecture family number as read from HW.

  @return  Architectural family as read from HW, or
           CHIPINFO_RAW_DEVICE_FAMILY_UNKNOWN if called before ChipInfo is
           initialized.
**/
UINT32
EFIAPI
ChipInfoGetRawDeviceFamily (
  VOID
  )
{
  return mChipInfoCtxt.RawFamilyId;
}

/**
  Returns the chip's device ID within its architectural family.

  Functionally, this is a generation counter.

  @return  Device number as read from HW, or
           CHIPINFO_RAW_DEVICE_NUMBER_UNKNOWN if called before ChipInfo is
           initialized.
**/
UINT32
EFIAPI
ChipInfoGetRawDeviceNumber (
  VOID
  )
{
  return mChipInfoCtxt.FamilyDeviceId;
}

/**
  Returns the chip's QFPROM chip ID.

  Can be used along with the serial number to uniquely identify the chip.

  @return  Chip ID as read from QFPROM fuses if successful, or
           CHIPINFO_QFPROM_CHIPID_UNKNOWN if called before ChipInfo is
           initialized.
**/
ChipInfoQfpromChipIdType
EFIAPI
ChipInfoGetQfpromChipId (
  VOID
  )
{
  return mChipInfoCtxt.QfpromChipId;
}

/**
  Retrieves a mask of CPUs in the specified cluster marked as disabled in
  PTE fuses. If the cluster has fewer than 32 CPUs, the additional bits are
  treated as enabled.

  @param[in]   CpuCluster  CPU cluster to check.
  @param[out]  Mask        Pointer to hold the mask of disabled CPUs in the
                            selected cluster.

  @return  CHIPINFO_SUCCESS if successful.
  @return  CHIPINFO_ERROR_OUT_OF_RANGE_PARAMETER if CpuCluster exceeds the
           number of clusters present.
  @return  CHIPINFO_ERROR for other general errors.
**/
ChipInfoResult
EFIAPI
ChipInfoGetDisabledCpus (
  IN  UINT32  CpuCluster,
  OUT UINT32  *Mask
  )
{
  if (Mask == NULL) {
    return CHIPINFO_ERROR;
  }

  if ((CpuCluster == 0) && !mChipInfoCtxt.NumClusters) {
    //
    // If this target doesn't have a binning plan, we don't know how many
    // CPU clusters there are for this target, and ChipInfo.NumClusters
    // will be left at its default value of 0. This is the only case where
    // NumClusters will be 0.
    //
    // Some clients call this function even if a binning plan isn't defined,
    // because they have target-agnostic code. These clients expect it to
    // return successfully instead of OUT_OF_RANGE for the first cluster (0)
    // when PTE hasn't blown fuses, since there's always at least 1 core
    // that's working (the one that's running this function).
    //
    // mChipInfoCtxt.CpuClusters will be NULL until it's initialized by
    // the target-specific function. That function will only be defined
    // if we have a binning plan, so it'll still be NULL if we get to this
    // point. We therefore need an explicit case here, where we return
    // successfully with Mask = "enabled/unknown", instead of
    // just accessing CpuClusters like the general case below.
    //
    *Mask = 0;
    return CHIPINFO_SUCCESS;
  }

  if (CpuCluster >= mChipInfoCtxt.NumClusters) {
    return CHIPINFO_ERROR_OUT_OF_RANGE_PARAMETER;
  }

  *Mask = mChipInfoCtxt.CpuClusters[CpuCluster];
  return CHIPINFO_SUCCESS;
}

/**
  Gets SKU and product code information for the current device.

  If the SKU_ID and/or P_CODE fuses are not blown, the corresponding
  CHIPINFO_*_UNKNOWN value will be returned.

  @param[out]  Info  Pointer to a caller-allocated buffer where SKU
                      information will be stored.

  @return  CHIPINFO_SUCCESS if successful.
  @return  CHIPINFO_ERROR_INVALID_PARAMETER if Info is NULL.
  @return  CHIPINFO_ERROR_NOT_SUPPORTED if this target does not support
           SKU information.
**/
ChipInfoResult
EFIAPI
ChipInfoGetSKU (
  OUT ChipInfoSKUType  *Info
  )
{
  if (Info == NULL) {
    return CHIPINFO_ERROR_INVALID_PARAMETER;
  }

  Info->eFeatureCode = mChipInfoCtxt.FeatureCode;
  Info->ePCode       = mChipInfoCtxt.PCode;

  if ((mChipInfoCtxt.FeatureCode == CHIPINFO_FEATURE_CODE_UNKNOWN) &&
      (mChipInfoCtxt.PCode == CHIPINFO_P_CODE_UNKNOWN))
  {
    return CHIPINFO_ERROR_NOT_SUPPORTED;
  }

  return CHIPINFO_SUCCESS;
}

/**
  Gets the number of functional clusters.

  A cluster is deemed functional if it has at least one functional core.
  There will always be at least 1 functional cluster.

  @param[out]  NumClusters  Buffer to store the number of functional clusters.

  @return  CHIPINFO_SUCCESS if successful.
  @return  CHIPINFO_ERROR_INVALID_PARAMETER if NumClusters is NULL.
  @return  CHIPINFO_ERROR_NOT_INITIALIZED if called before ChipInfoInit.
**/
ChipInfoResult
EFIAPI
ChipInfoGetNumFunctionalClusters (
  OUT UINT32  *NumClusters
  )
{
  if (NumClusters == NULL) {
    return CHIPINFO_ERROR_INVALID_PARAMETER;
  }

  if (!mChipInfoCtxt.InitComplete) {
    return CHIPINFO_ERROR_NOT_INITIALIZED;
  }

  *NumClusters = mChipInfoCtxt.NumFunctionalClusters;
  return CHIPINFO_SUCCESS;
}

/**
  Gets the boot cluster and core indices.

  @param[out]  Cluster  Buffer to store the boot cluster index (zero-indexed).
  @param[out]  Core     Buffer to store the boot core index (zero-indexed).

  @return  CHIPINFO_SUCCESS if both pointers were filled correctly.
  @return  CHIPINFO_ERROR_INVALID_PARAMETER if either pointer is NULL.
  @return  CHIPINFO_ERROR_NOT_INITIALIZED if called before ChipInfoInit.
**/
ChipInfoResult
EFIAPI
ChipInfoGetBootClusterAndCore (
  OUT UINT32  *Cluster,
  OUT UINT32  *Core
  )
{
  if ((Cluster == NULL) || (Core == NULL)) {
    return CHIPINFO_ERROR_INVALID_PARAMETER;
  }

  if (!mChipInfoCtxt.InitComplete) {
    return CHIPINFO_ERROR_NOT_INITIALIZED;
  }

  *Cluster = mChipInfoCtxt.BootCluster;
  *Core    = mChipInfoCtxt.BootCore;
  return CHIPINFO_SUCCESS;
}

/**
  Gets the HW part-level disabled feature details from fuses.

  The bit-level interpretation of Mask is specific to the client.

  @param[in]   Part  The ChipInfoPartType being queried.
  @param[in]   Idx   Hardware instance of the selected part.
  @param[out]  Mask  Used to store the part-specific details read from the
                      QTV/PTE region.

  @return  CHIPINFO_SUCCESS if successful.
  @return  CHIPINFO_ERROR_INVALID_PARAMETER if Mask is NULL or Part is unknown.
  @return  CHIPINFO_ERROR_OUT_OF_RANGE_PARAMETER if Part is out of range.
  @return  CHIPINFO_ERROR_NOT_INITIALIZED if called before ChipInfoInit.
**/
ChipInfoResult
EFIAPI
ChipInfoGetDisabledFeatures (
  IN  ChipInfoPartType  Part,
  IN  UINT32            Idx,
  OUT UINT32            *Mask
  )
{
  UINT32  Index;

  PlatformInfoPartInfoType  *PartInfo;

  if ((Mask == NULL) || (Part == CHIPINFO_PART_UNKNOWN)) {
    return CHIPINFO_ERROR_INVALID_PARAMETER;
  }

  if (Part >= CHIPINFO_NUM_PARTS) {
    return CHIPINFO_ERROR_OUT_OF_RANGE_PARAMETER;
  }

  if (!mChipInfoCtxt.InitComplete) {
    return CHIPINFO_ERROR_NOT_INITIALIZED;
  }

  if (Idx == 0) {
    *Mask = mChipInfoCtxt.PartialFeatures[Part];
    return CHIPINFO_SUCCESS;
  }

  for (Index = 0; Index < mChipInfoCtxt.PartInfoLen; Index++) {
    PartInfo = &mChipInfoCtxt.PartInfo[Index];
    if ((PartInfo->Part == Part) && (PartInfo->Instance == Idx)) {
      *Mask = PartInfo->DisabledFeatures;
      return CHIPINFO_SUCCESS;
    }
  }

  return CHIPINFO_ERROR_OUT_OF_RANGE_PARAMETER;
}

/**
  Checks if the specified part is completely disabled.

  FALSE indicates the part is either fully or partially enabled.
  TRUE indicates the part is completely disabled.
  Use ChipInfoGetDisabledFeatures to obtain partial disable information.

  @param[in]   Part  The ChipInfoPartType being queried.
  @param[in]   Idx   Hardware instance of the selected part.
  @param[out]  Mask  Pointer to a BOOLEAN to store the disabled status.

  @return  CHIPINFO_SUCCESS if successful.
  @return  CHIPINFO_ERROR_INVALID_PARAMETER if Mask is NULL or Part is unknown.
  @return  CHIPINFO_ERROR_OUT_OF_RANGE_PARAMETER if Part is out of range.
  @return  CHIPINFO_ERROR_NOT_INITIALIZED if called before ChipInfoInit.
**/
ChipInfoResult
EFIAPI
ChipInfoIsPartDisabled (
  IN  ChipInfoPartType  Part,
  IN  UINT32            Idx,
  OUT BOOLEAN           *Mask
  )
{
  UINT32  Index;

  PlatformInfoPartInfoType  *PartInfo;

  if ((Mask == NULL) || (Part == CHIPINFO_PART_UNKNOWN)) {
    return CHIPINFO_ERROR_INVALID_PARAMETER;
  }

  if (Part >= CHIPINFO_NUM_PARTS) {
    return CHIPINFO_ERROR_OUT_OF_RANGE_PARAMETER;
  }

  if (!mChipInfoCtxt.InitComplete) {
    return CHIPINFO_ERROR_NOT_INITIALIZED;
  }

  if (Idx == 0) {
    *Mask = ((mChipInfoCtxt.DisabledFeatures[Part] & 0x1) ? TRUE : FALSE);
    return CHIPINFO_SUCCESS;
  }

  for (Index = 0; Index < mChipInfoCtxt.PartInfoLen; Index++) {
    PartInfo = &mChipInfoCtxt.PartInfo[Index];
    if ((PartInfo->Part == Part) && (PartInfo->Instance == Idx)) {
      *Mask = PartInfo->Disabled;
      return CHIPINFO_SUCCESS;
    }
  }

  return CHIPINFO_ERROR_OUT_OF_RANGE_PARAMETER;
}

/**
  Returns the raw package type as read from HW.

  @return  Raw package type as read from HW.
**/
UINT32
EFIAPI
ChipInfoGetRawPackageType (
  VOID
  )
{
  return mChipInfoCtxt.RawPackageType;
}

/**
  Gets the number of CPU cores in the specified cluster.

  @param[in]   CpuCluster  CPU cluster to query.
  @param[out]  Cores       Pointer to store the number of cores.

  @return  CHIPINFO_SUCCESS if successful.
  @return  CHIPINFO_ERROR_INVALID_PARAMETER if Cores is NULL.
  @return  CHIPINFO_ERROR_NOT_INITIALIZED if called before ChipInfoInit.
  @return  CHIPINFO_ERROR_OUT_OF_RANGE_PARAMETER if CpuCluster is out of range.
**/
ChipInfoResult
EFIAPI
ChipInfoGetNumCpuCores (
  IN  UINT32  CpuCluster,
  OUT UINT32  *Cores
  )
{
  if (Cores == NULL) {
    return CHIPINFO_ERROR_INVALID_PARAMETER;
  }

  if (!mChipInfoCtxt.InitComplete) {
    return CHIPINFO_ERROR_NOT_INITIALIZED;
  }

  if (CpuCluster >= mChipInfoCtxt.NumClusters) {
    return CHIPINFO_ERROR_OUT_OF_RANGE_PARAMETER;
  }

  *Cores = mChipInfoCtxt.NumCores[CpuCluster];

  return CHIPINFO_SUCCESS;
}
