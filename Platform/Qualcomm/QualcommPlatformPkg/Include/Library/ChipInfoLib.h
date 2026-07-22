/** @file
 This is the interface specification for the chip driver/service.

 Copyright (c) Qualcomm Technologies, Inc. and/or its subsidiaries.
 SPDX-License-Identifier: BSD-2-Clause-Patent
 ============================================================================
**/

#pragma once

/*=========================================================================
 Includes
 ==========================================================================*/

#include <Base.h>
#include <ChipInfoDefs.h>


/**
 @{
 */

/**
 * Returns the version of the chip as a ChipInfoVersionType
 *
 * @return  Chip version if successful,
 *          CHIPINFO_VERSION_UNKNOWN if called before ChipInfo is initialized
 */
ChipInfoVersionType
ChipInfo_GetChipVersion (
  VOID
  );

/**
 * Returns the version of the chip as read from fuses.
 * This "raw" version is a strictly increasing integer counter,
 * incremented for each new HW version. e.g.:
 * v1.0 = 0
 * v1.1 = 1
 * v1.2 = 2
 * v2.0 = 3
 * ...
 *
 * The only use case for this API is for patch revisions, i.e., the 'z'
 * in vX.Y.Z. Patch revisions are not reflected in the value returned
 * from ChipInfo_GetChipVersion, but are included in the HW revision
 * counter read by this function.
 *
 * Use ChipInfo_GetChipVersion for everything else.
 *
 * @return  Chip version as read from HW,
 *          CHIPINFO_RAW_VERSION_UNKNOWN if called before ChipInfo is
 *            initialized
 */
UINT32
ChipInfo_GetRawChipVersion (
  VOID
  );

/**
 * Returns the chip ID as a ChipInfoIdType
 *
 * @return  Chip ID associated with the part number read from HW,
 *          CHIPINFO_ID_UNKNOWN if no associated chip id could be found,
 *            or if called before ChipInfo is initialized
 */
ChipInfoIdType
ChipInfo_GetChipId (
  VOID
  );

/**
 * Returns the chip ID as read from HW (i.e. the JTAG ID).
 * Rarely needed, except as a workaround until ChipInfoIdType is updated
 * with support for new chips. Use ChipInfo_GetChipId instead.
 *
 * @return  Chip ID as read from HW, or
 *          CHIPINFO_RAW_ID_UNKNOWN if called before ChipInfo is initialized
 */
UINT32
ChipInfo_GetRawChipId (
  VOID
  );

/**
 * Returns the name of the chip (e.g. MSM8998, MDM9665)
 *
 * @param[in,out] szChipIdStr   Pointer a buffer to hold the chip's name.
                                If called before ChipInfo is initialized, or if the
                                Chip ID string is unknown, the string "UNKNOWN" will
                                be copied into this buffer.
 * @param[in]     nMaxLength    Length of the string to copy. Max supported
 *                              size is CHIPINFO_MAX_ID_LENGTH
 *
 * @return  CHIPINFO_SUCCESS always
 */
ChipInfoResult
ChipInfo_GetChipIdString (
  CHAR8   *szChipIdStr,
  UINT32  nMaxLength
  );

/**
 * Returns the chip's family as a ChipInfoFamilyType. This has no connection
 * to the architectural family - it's used to group all revisions of a specific chip
 *
 * @return  Chip Family if successful,
 *          CHIPINFO_FAMILY_UNKNOWN if no associated chip family can be found,
                or if called before ChipInfo is initialized
 *
 */
ChipInfoFamilyType
ChipInfo_GetChipFamily (
  VOID
  );

/**
 * Returns the marketing name of the processor. This is typically the same
 * as the ChipIdString.
 *
 * @param [in,out]  pszProcessorNameStr   Pointer to a buffer to hold the processor's name.
                                          If called before ChipInfo is initialized, or if the
                                          marketing name is unknown, the string "UNKNOWN" will
                                          be copied into this buffer.
 * @param [in]      nMaxLen               Length of the name buffer. At most CHIPINFO_MAX_NAME_LEN
 *                                        bytes will be copied.
 * @return  CHIPINFO_SUCCESS always
 */
ChipInfoResult
ChipInfo_GetMarketingNameString (
  CHAR8   *pszMarketingNameStr,
  UINT32  nMaxLen
  );

/**
 * Returns whether or not the chip supports a modem.
 *
 * This isn't intended to to be an exhaustive list of modems supported by this
 * chip, since there are far to many configurations to keep track of. The value
 * returned here should be treated as indicating the presence of a modem,
 * i.e. 0 = Modem not supported (e.g. SDA), nonzero = Modem supported (e.g. SDM)
 *
 * @return  Non-zero if modems are supported for this chip,
 *          CHIPINFO_MODEM_UNKNOWN if modems are not supported, or
 *            if called before ChipInfo is initialized
 */
ChipInfoModemType
ChipInfo_GetModemSupport (
  VOID
  );

/**
 * Returns the chip's serial number.
 *
 * On its own, this is not a globally unique identifier. Serial numbers
 * are only unique within a given commercial product family.
 * Use ChipInfo_GetQFPROMChipId to find the commercial product family
 * for this device. Combining the two values gives a globally unique ID
 * for this device.
 *
 * @return  Serial number as read from HW, or
 *          CHIPINFO_SERIAL_NUM_UNKNOWN if called before ChipInfo is initialized
 */
ChipInfoSerialNumType
ChipInfo_GetSerialNumber (
  VOID
  );

/**
 * Returns the foundry ID for the chip as a ChipInfoFoundryIdType
 *
 * @return  Foundry ID if successful,
 *          CHIPINFO_FOUNDRYID_UNKNOWN if called before ChipInfo is initialized
 */
ChipInfoFoundryIdType
ChipInfo_GetFoundryId (
  VOID
  );

/**
 * Returns the chip's architecture family number as read from HW
 *
 * @return  Architectural family as read from HW, or
 *          CHIPINFO_RAW_DEVICE_FAMILY_UNKNOWN if called before ChipInfo is initialized
 */
UINT32
ChipInfo_GetRawDeviceFamily (
  VOID
  );

/**
 * Returns the chip's device ID within its architectural family.
 * Functionally, this is a generation counter.
 *
 * @return  Device number as read from HW,
 *          CHIPINFO_RAW_DEVICE_NUMBER_UNKNOWN if called before ChipInfo is initialized
 */
UINT32
ChipInfo_GetRawDeviceNumber (
  VOID
  );

/**
 * Returns the chip's QFPROM chip ID, which can be used along with the
 * serial number to uniquely identify the chip
 *
 * @return  Chip ID as read from QFPROM fuses, if successful
 *          CHIPINFO_QFPROM_CHIPID_UNKNOWN if called before ChipInfo is initialized
 */
ChipInfoQFPROMChipIdType
ChipInfo_GetQFPROMChipId (
  VOID
  );

/**
 * Retrieve a mask of CPUs in the current cluster which are marked as
 * Disabled in the PTE fuses. If the cluster has fewer than 32 CPUs,
 * the additional CPUs are marked as "Enabled"
 *
 * @param[in] nCPUCluster CPU cluster to check.
 * @param[out] nMask      Pointer to uint32 to hold a mask of disabled
 *                          CPUs in the selected cluster
 *
 * @return  CHIPINFO_ERROR_OUT_OF_RANGE_PARAMETER  if nClusterIdx > number of clusters present
 *          CHIPINFO_ERROR for othe general errors
 *          CHIPINFO_SUCCESS otherwise
 */
ChipInfoResult
ChipInfo_GetDisabledCPUs (
  UINT32  nCPUCluster,
  UINT32  *pnMask
  );

/**
 * Initialize the ChipInfo driver
 *
 * @return  CHIPINFO_SUCCESS always. Any missing/unknown information will be
 *            indicated as specified in the API documentation above.
 */
ChipInfoResult
ChipInfo_Init (
  VOID
  );

/**
 * Get SKU and Product Code information for the current device.
 *
 * This information can be used to make functional decisions, e.g.,
 * selecting different DDR voltage plans.
 *
 * If the SKU_ID and/or P_CODE fuses are not blown on this device, the
 * value(s) returned by this function will be the corresponding
 * CHIPINFO_*_UNKNOWN value.
 *
 * @param[out] pInfo pointer to a caller-allocated buffer where SKU
 *                    information will be stored.
 *
 * @return CHIPINFO_ERROR_INVALID_PARAMETER if pInfo is NULL
 *         CHIPINFO_ERROR_NOT_SUPPORTED if this target doesn't support
 *          SKU information.
 *         CHIPINFO_SUCCESS otherwise
 */
ChipInfoResult
ChipInfo_GetSKU (
  ChipInfoSKUType  *pInfo
  );

/**
 * Get the number of functional clusters.
 *
 * This is the total number of functional clusters based on partial
 * binning and SKUing. A cluster is deemed functional if it has at least
 * one functional core. There will always be at least 1 functional
 * cluster: the one that's running this code.
 *
 * @param[out] pnNumClusters    buffer to store the number of clusters
 *
 * @return  CHIPINFO_SUCCESS if the number of clusters was successfully
 *            retrieved and stored in pnNumClusters;
 *          CHIPINFO_ERROR_INVALID_PARAMETER if pnNumClusters is invalid;
 *          CHIPINFO_ERROR_NOT_INITIALIZED if this function was called
 *            before ChipInfo_Init was invoked. *pnNumClusters will not
 *            be updated.
 */
ChipInfoResult
ChipInfo_GetNumFunctionalClusters (
  UINT32  *pnNumClusters
  );

/**
 * Get the boot cluster and core.
 *
 * This function returns an error type instead of returning the queried values directly
 * because there's no sentinel value it can use: while the other APIs can use a return
 * value of 0 to mean "UNKNOWN", 0 is a valid core/cluster index.
 *
 * @param[out] pnCluster   buffer to store the boot cluster index (zero-indexed).
 * @param[out] pnCore      buffer to store the boot core index (zero-indexed).
 *
 * @return  CHIPINFO_SUCCESS if both pointers were filled correctly;
 *          CHIPINFO_ERROR_INVALID_PARAMETER if either pointer is invalid;
 *          CHIPINFO_ERROR_NOT_INITIALIZED if this function was called before ChipInfo_Init was invoked.
 */
ChipInfoResult
ChipInfo_GetBootClusterAndCore (
  UINT32  *pnCluster,
  UINT32  *pnCore
  );

/**
 * Gets the HW Part(IP) level  Specific details from the fuse
 * such as which all features are enabled/disabled in a HW Part.
 *
 * @param[in] ePart     The ChipInfoPartType being queried
 * @param[in] nIdx      Hardware instance of the selected part
 * @param[out] pnMask   Used to store the part specific details read from the QTV/PTE region.
 *                      The bit level interpretation of pnMask is specific to client and
 *                      client needs to decode accordingly.
 *
 * @return CHIPINFO_ERROR_INVALID_PARAMETER if pnMask is NULL or ePart is Unknown.
 *         CHIPINFO_ERROR_OUT_OF_RANGE_PARAMETER if ePart is out of the defined range of Parts.
 *         CHIPINFO_ERROR_NOT_INITIALIZED if ChipInfoCtxt is not Initialized
 *         CHIPINFO_SUCCESS otherwise
 */
ChipInfoResult
ChipInfo_GetDisabledFeatures (
  ChipInfoPartType  ePart,
  UINT32            nIdx,
  UINT32            *pnMask
  );

/**
 * Check if the specified Part is enabled or Disabled
 *
 * @param[in] ePart     The ChipInfoPartType being queried
 * @param[in] nIdx      Hardware instance of the selected part
 * @param[out] pnMask   Pointer to bool used to store the status of the selected part.
 *                      FALSE => part is either completely enabled or partially enabled,
 *                      TRUE => Disabled completely.
 *                      Clients can make use of ChipInfo_GetDisabledFeatures API
 *                      to obtain disabled features info.
 *
 * @return CHIPINFO_ERROR_INVALID_PARAMETER if pnMask is NULL or ePart is Unknown.
 *         CHIPINFO_ERROR_OUT_OF_RANGE_PARAMETER if ePart is out of the defined range of Parts.
 *         CHIPINFO_ERROR_NOT_INITIALIZED if ChipInfoCtxt is not Initialized
 *         CHIPINFO_SUCCESS otherwise
 */
ChipInfoResult
ChipInfo_IsPartDisabled (
  ChipInfoPartType  ePart,
  UINT32            nIdx,
  BOOLEAN           *pnMask
  );

/**
 * @}
 */
