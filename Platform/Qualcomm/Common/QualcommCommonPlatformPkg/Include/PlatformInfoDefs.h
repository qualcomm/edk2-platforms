/** @file
  Public definitions for the PlatformInfo driver.

  @par Glossary:
    - Id     - Identifier
    - Kvps   - Key-Value PairS
    - Oem    - Original Equipment Manufacturer
    - Pmic   - Power Management Integrated Circuit
    - Qfprom - Qualcomm Fuse Programmable Read-Only Memory
    - Smem   - Qualcomm Shared Memory

  Copyright (c) Qualcomm Technologies, Inc. and/or its subsidiaries.<BR>
  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#pragma once

/*=========================================================================
      Include Files
==========================================================================*/
#include <Base.h>

/*=========================================================================
      Type Definitions
==========================================================================*/

/**
    @{
*/

/**
 * Generate a platform version number.
 *
 * This macro generates the platform version number from the specified
 * major and minor numbers. For example, version 1.2 is represented by
 * @code PLATORMINFO_VERSION(1,2) @endcode
 *
*/
#define PLATFORMINFO_VERSION(major, minor)       (((major) << 16) | (minor))
#define PLATFORMINFO_GET_MAJOR_VERSION(version)  (((version) >> 16) & 0xF)
#define PLATFORMINFO_GET_MINOR_VERSION(version)  ((version) & 0xF)

/**
  Platform Info error type.
 */
typedef enum {
  PLATFORMINFO_SUCCESS                       = 0,
  PLATFORMINFO_ERROR                         = -1,
  PLATFORMINFO_ERROR_INVALID_PARAMETER       = -2,
  PLATFORMINFO_ERROR_INSUFFICIENT_MEMORY     = -3,
  PLATFORMINFO_ERROR_NOT_FOUND               = -4,
  PLATFORMINFO_ERROR_INTERNAL                = -5,
  PLATFORMINFO_ERROR_NOT_ALLOWED             = -6,
  PLATFORMINFO_ERROR_NOT_SUPPORTED           = -7,
  PLATFORMINFO_ERROR_NOT_INITIALIZED         = -8,
  PLATFORMINFO_ERROR_OUT_OF_RANGE_PARAMETER  = -9,
  PLATFORMINFO_ERROR_INVALID_ADDRESS         = -10,
  PLATFORMINFO_ERROR_INSUFFICIENT_BUFFER_LEN = -11,
} PlatformInfoResult;

/**
  Target type of the device on which the platform is running.
 */
typedef enum {
  PLATFORMINFO_TYPE_UNKNOWN     = 0x00,                   /**< Unknown target device. */
  PLATFORMINFO_TYPE_SURF        = 0x01,                   /**< Target is a SURF device. */
  PLATFORMINFO_TYPE_CDP         = PLATFORMINFO_TYPE_SURF, /**< Target is a CDP (aka SURF) device. */
  PLATFORMINFO_TYPE_FFA         = 0x02,                   /**< Target is an FFA device. */
  PLATFORMINFO_TYPE_FLUID       = 0x03,                   /**< Target is a FLUID device. */
  PLATFORMINFO_TYPE_FUSION      = 0x04,                   /**< Target is a FUSION device. */
  PLATFORMINFO_TYPE_OEM         = 0x05,                   /**< Target is an OEM device. */
  PLATFORMINFO_TYPE_QT          = 0x06,                   /**< Target is a QT device. */
  PLATFORMINFO_TYPE_MTP         = 0x08,                   /**< Target is a MTP device. */
  PLATFORMINFO_TYPE_MTP_MDM     = PLATFORMINFO_TYPE_MTP,  /**< Target is a MDM MTP device. */
  PLATFORMINFO_TYPE_MTP_MSM     = PLATFORMINFO_TYPE_MTP,  /**< Target is a MSM MTP device. */
  PLATFORMINFO_TYPE_LIQUID      = 0x09,                   /**< Target is a LiQUID device. */
  PLATFORMINFO_TYPE_DRAGONBOARD = 0x0A,                   /**< Target is a DragonBoard device. */
  PLATFORMINFO_TYPE_QRD         = 0x0B,                   /**< Target is a QRD device. */
  PLATFORMINFO_TYPE_EVB         = 0x0C,                   /**< Target is an EVB device. */
  PLATFORMINFO_TYPE_HRD         = 0x0D,                   /**< Target is a HRD device. */
  PLATFORMINFO_TYPE_DTV         = 0x0E,                   /**< Target is a DTV device. */
  PLATFORMINFO_TYPE_RUMI        = 0x0F,                   /**< Target is on RUMI. */
  PLATFORMINFO_TYPE_VIRTIO      = 0x10,                   /**< Target is on VIRTIO. */
  PLATFORMINFO_TYPE_GOBI        = 0x11,                   /**< Target is a GOBI device. */
  PLATFORMINFO_TYPE_CBH         = 0x12,                   /**< Target is a CBH device. */
  PLATFORMINFO_TYPE_BTS         = 0x13,                   /**< Target is a BTS device. */
  PLATFORMINFO_TYPE_XPM         = 0x14,                   /**< Target is a XPM device. */
  PLATFORMINFO_TYPE_RCM         = 0x15,                   /**< Target is a RCM device. */
  PLATFORMINFO_TYPE_DMA         = 0x16,                   /**< Target is a DMA device. */
  PLATFORMINFO_TYPE_STP         = 0x17,                   /**< Target is a STP device. */
  PLATFORMINFO_TYPE_SBC         = 0x18,                   /**< Target is a SBC device. */
  PLATFORMINFO_TYPE_ADP         = 0x19,                   /**< Target is a ADP device. */
  PLATFORMINFO_TYPE_CHI         = 0x1A,                   /**< Target is a CHI device. */
  PLATFORMINFO_TYPE_SDP         = 0x1B,                   /**< Target is a SDP device. */
  PLATFORMINFO_TYPE_RRP         = 0x1C,                   /**< Target is a RRP device. */
  PLATFORMINFO_TYPE_CLS         = 0x1D,                   /**< Target is a CLS device. */
  PLATFORMINFO_TYPE_TTP         = 0x1E,                   /**< Target is a TTP device. */
  PLATFORMINFO_TYPE_HDK         = 0x1F,                   /**< Target is a HDK device. */
  PLATFORMINFO_TYPE_IOT         = 0x20,                   /**< Target is a IOT device. */
  PLATFORMINFO_TYPE_ATP         = 0x21,                   /**< Target is a ATP device. */
  PLATFORMINFO_TYPE_IDP         = 0x22,                   /**< Target is a IDP device. */
  PLATFORMINFO_TYPE_AEDK        = 0x23,                   /**< Target is a AEDK device. */
  PLATFORMINFO_TYPE_WDP         = 0x24,                   /**< Target is a WDP device. */
  PLATFORMINFO_TYPE_QAM         = 0x25,                   /**< Target is a QAM device. */
  PLATFORMINFO_TYPE_QXR         = 0x26,                   /**< Target is a QXR device. */
  PLATFORMINFO_TYPE_X100        = 0x27,                   /**< Target is a PCIe X100 card */
  PLATFORMINFO_TYPE_CRD         = 0x28,                   /**< Compute Reference Device */
  PLATFORMINFO_TYPE_QQVP        = 0x29,                   /**< Compute Reference Device */
  PLATFORMINFO_TYPE_DCP         = 0x2A,                   /**< Data-Center Platform */
  PLATFORMINFO_TYPE_QCB         = 0x2B,                   /**< Qualcomm Compute Board */
  PLATFORMINFO_TYPE_QAR         = 0x2C,                   /**< Qualcomm Augmented Reality */
  PLATFORMINFO_TYPE_WRD         = 0x2D,                   /**< Wearable Reference Design */
  PLATFORMINFO_TYPE_COME        = 0x2E,                   /**< COM Express */
  PLATFORMINFO_TYPE_EVK         = 0x2F,                   /**< Evaluation Kit */
  PLATFORMINFO_TYPE_TDP         = 0x30,                   /**< Telematics Development */
  PLATFORMINFO_TYPE_OMTP        = 0x31,                   /**< Target is a OMTP device. */
  PLATFORMINFO_TYPE_ITPS        = 0x32,                   /**< Target is a ITPS(IOT Test Platform Socketed) device. */
  PLATFORMINFO_TYPE_ITP         = 0x33,                   /**< Target is a ITP(IOT Test Platform - Open Platform) device. */
  PLATFORMINFO_TYPE_EITP        = 0x34,                   /**< Target is a eITP(enclosed IOT Test Platform) device. */

  PLATFORMINFO_NUM_TYPES, /**< Number of known targets (including unknown). */
  PLATFORMINFO_TYPE_32BITS = 0x7FFFFFFF
} PlatformInfoPlatformType;

/**
 * Keys to get data out of the CDT.
 */
typedef enum {
  PLATFORMINFO_KEY_UNKNOWN     = 0x00,
  PLATFORMINFO_KEY_DDR_FREQ    = 0x01,
  PLATFORMINFO_KEY_GFX_FREQ    = 0x02,
  PLATFORMINFO_KEY_CAMERA_FREQ = 0x03,
  PLATFORMINFO_KEY_FUSION      = 0x04,
  PLATFORMINFO_KEY_CUST        = 0x05,
  PLATFORMINFO_KEY_NAND_SCRUB  = 0x06,
  PLATFORMINFO_KEY_SLT         = 0x07,
  PLATFORMINFO_KEY_PMIC        = 0x08,
  PLATFORMINFO_KEY_POWER_GRID  = 0x09,

  PLATFORMINFO_NUM_KEYS,

  PLATFORMINFO_KEY_32BITS = 0x7FFFFFFF
} PlatformInfoKeyType;

/**
  Stores the target platform, the platform version, and the
  platform subtype.
 */
typedef struct {
  PlatformInfoPlatformType    PlatformType;  /**< Type of the current target. */
  UINT32                      Version;       /**< Version of the platform in use. */
  UINT32                      Subtype;       /**< Sub-type of the platform. */
  BOOLEAN                     Fusion;        /**< TRUE if Fusion; FALSE otherwise. */
  UINT32                      OemVariantId;  /**< OEM variant ID. */
} PlatformInfoPlatformInfoType;

/**
  Format of the PlatformInfoSmemType structure. Minor revision ticks
  are backwards compatible.
 */
#define PLATFORMINFO_SMEM_FORMAT  PLATFORMINFO_VERSION(0, 23)

/**
  Length of the build ID buffer in PlatformInfoSmemType.
 */
#define PLATFORMINFO_SMEM_BUILD_ID_LENGTH  32

/**
 * Length of the chip ID buffer in PlatformInfoSmemType
 */
#define PLATFORMINFO_SMEM_MAX_CHIP_ID_LENGTH  32

/**
  Maximum number of PMIC devices in PlatformInfoSmemType.
 */
#define PLATFORMINFO_SMEM_MAX_PMIC_DEVICES  3

/**
  SMEM structure for PMIC information.
**/
typedef struct {
  UINT32    PmicModel;    /**< PMIC device model type, for Badger matches the revision id subtype */
  UINT32    PmicVersion;  /**< PMIC version, same format as Platform version */
} PlatformInfoSmemPmicType;

/**
  Qultivate information for a part instance.
**/
typedef struct {
  UINT16    Part;
  UINT8     Instance;
  UINT8     Disabled;
  UINT32    DisabledFeatures;
} PlatformInfoPartInfoType;

/**
  Structure for the shared memory location which is used to store
  platform, chip, build, and pmic information.
 */
typedef struct {
  UINT32                      Format;               /**< Format of the structure. */
  UINT32                      ChipId;               /**< Chip ID. */
  UINT32                      ChipVersion;          /**< Chip version. */
  CHAR8                       BuildId[PLATFORMINFO_SMEM_BUILD_ID_LENGTH];
  /**< Build ID. */
  UINT32                      RawChipId;             /**< Raw chip ID. */
  UINT32                      RawChipVersion;        /**< Raw chip version. */
  PlatformInfoPlatformType    PlatformType;          /**< Platform type. */
  UINT32                      PlatformVersion;       /**< Platform version. */
  UINT32                      Fusion;                /**< TRUE if Fusion; FALSE otherwise. */
  UINT32                      PlatformSubtype;       /**< Platform subtype. */
  PlatformInfoSmemPmicType    PmicInfo[PLATFORMINFO_SMEM_MAX_PMIC_DEVICES];

  /**< DEPRECATED. May not contain the full
       list. Use array at PmicArrayOffset instead. */
  UINT32                      FoundryId;             /**< Chip foundry ID. */
  UINT32                      ChipSerial;            /**< Chip serial number. */
  UINT32                      NumPmics;              /**< Number of PMICs in array. */
  UINT32                      PmicArrayOffset;       /**< Offset from base of structure to
                                                        array of PlatformInfoSmemPmicType. */
  UINT32                      ChipFamily;            /**< Chip family. */
  UINT32                      RawDeviceFamily;       /**< Raw device family. */
  UINT32                      RawDeviceNumber;       /**< Raw device number. */

  UINT32                      QfpromChipId;                                     /**< QFPROM Chip ID   */
  CHAR8                       ChipIdStr[PLATFORMINFO_SMEM_MAX_CHIP_ID_LENGTH];   /**< Chip name */
  UINT32                      NumClusters;
  /**< Number of clusters used by ChipInfo_GetDisabledCPUs API */
  UINT32                      ClusterArrayOffset;
  /**< Offset from base of structure to UINT32 array of disabled CPU
       clusters. Used by ChipInfo_GetDisabledCPUs API*/
  UINT32                      NumParts;
  /**< Number of parts supported by ChipInfo_GetDisabledFeatures API*/
  UINT32                      DisabledFeaturesArrayOffset;
  /**< Offset from base of structure to UINT32 array of disabled parts */
  UINT32                      ModemSupported;
  /**< 0 if not supported, nonzero if supported */
  UINT32                      FeatureCode;                                     /**< Feature Code enum for this device */
  UINT32                      PCode;                                            /**< PCode for this device */
  UINT32                      PartNameMapOffset;                                /**< Offset of the part name
                                                              strings for the current chip,
                                                              from the base of socinfo. */
  UINT32                      NumPartNameMappings;                              /**< Number of part name strings. */
  UINT32                      OemVariantId;                                     /**< OEM Variant ID */
  UINT32                      NumKvps;                                          /**< Number of KVPS */
  UINT32                      KvpsOffset;                                       /**< Offset of the KVPS */
  UINT32                      NumFunctionalClusters;
  /**< Number of clusters with >=1 enabled core */
  UINT32                      BootCluster;                                      /**< Boot cluster index */
  UINT32                      BootCore;                                         /**< Boot core index */
  UINT32                      RawPackageType;                                   /**< Raw package type. */
  UINT32                      PartialFeaturesArrayOffset;
  /**< Offset from base of this structure to an array of UINT32s indexed
       by ChipInfoPartType. Each bit corresponds to a component of that
       part, with 0 = enabled/unknown and 1 = disabled. */
  UINT32                      CpuCoresArrayOffset;
  /**< Offset from base of structure to UINT32 array of no.of CPU cores
       per cluster for all clusters. Used by ChipInfo_GetNumCPUCores API*/

  /**
    Offset from socinfo base, to an array of PlatformInfoPartInfoType
    structures. NOT indexed by ChipInfoPartType; iterate and look for
    a matching .Part and .Instance
  **/
  UINT32                      PartInstancesOffset;

  /**
    Length of the array at PartInstancesOffset
  **/
  UINT32                      NumPartInstances;
} PlatformInfoSmemType;

/** @} */ /* end_addtogroup platform_info */
