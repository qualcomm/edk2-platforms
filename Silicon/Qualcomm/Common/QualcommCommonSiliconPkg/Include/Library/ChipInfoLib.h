/** @file
  This is the interface specification for the chip driver/service.

  @par Glossary:
    - Qfprom - Qualcomm Fuse Programmable Read-Only Memory

  Copyright (c) Qualcomm Technologies, Inc. and/or its subsidiaries.<BR>
  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#pragma once

/*=========================================================================
 Includes
 ==========================================================================*/

#include <Base.h>
#include <ChipInfoDefs.h>

/**
  Returns the version of the chip as a ChipInfoVersionType.

  @return  Chip version if successful,
           CHIPINFO_VERSION_UNKNOWN if called before ChipInfo is initialized.
**/
ChipInfoVersionType
EFIAPI
ChipInfoGetChipVersion (
  VOID
  );

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
  );

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
  );

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
  );

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
  );

/**
  Returns the chip's family as a ChipInfoFamilyType.

  This has no connection to the architectural family - it's used to group
  all revisions of a specific chip.

  @return  Chip family if successful, or
           CHIPINFO_FAMILY_UNKNOWN if no associated chip family can be found,
           or if called before ChipInfo is initialized.
**/
ChipInfoFamilyType
EFIAPI
ChipInfoGetChipFamily (
  VOID
  );

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
  );

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
  );

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
  );

/**
  Returns the foundry ID for the chip as a ChipInfoFoundryIdType.

  @return  Foundry ID if successful, or
           CHIPINFO_FOUNDRYID_UNKNOWN if called before ChipInfo is initialized.
**/
ChipInfoFoundryIdType
EFIAPI
ChipInfoGetFoundryId (
  VOID
  );

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
  );

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
  );

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
  );

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
  );

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
  );

/**
  Gets SKU and product code information for the current device.

  This information can be used to make functional decisions, e.g.,
  selecting different DDR voltage plans.

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
  );

/**
  Gets the number of functional clusters.

  This is the total number of functional clusters based on partial
  binning and SKUing. A cluster is deemed functional if it has at least
  one functional core. There will always be at least 1 functional
  cluster: the one that's running this code.

  @param[out]  NumClusters  Buffer to store the number of functional clusters.

  @return  CHIPINFO_SUCCESS if successful.
  @return  CHIPINFO_ERROR_INVALID_PARAMETER if NumClusters is NULL.
  @return  CHIPINFO_ERROR_NOT_INITIALIZED if called before ChipInfoInit.
**/
ChipInfoResult
EFIAPI
ChipInfoGetNumFunctionalClusters (
  OUT UINT32  *NumClusters
  );

/**
  Gets the boot cluster and core indices.

  This function returns an error type instead of returning the queried
  values directly because there's no sentinel value it can use: while the
  other APIs can use a return value of 0 to mean "UNKNOWN", 0 is a valid
  core/cluster index.

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
  );

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
  );

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
  );

/**
  Returns the raw package type as read from HW.

  @return  Raw package type as read from HW.
**/
UINT32
EFIAPI
ChipInfoGetRawPackageType (
  VOID
  );

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
  );
