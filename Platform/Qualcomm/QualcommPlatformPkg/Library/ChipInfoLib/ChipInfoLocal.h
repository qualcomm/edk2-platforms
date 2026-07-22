/** @file
  Internal function and data structure declarations for ChipInfo
  shared across all images.

  Copyright (c) Qualcomm Technologies, Inc. and/or its subsidiaries.
  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#pragma once

#include <Base.h>
#include <Library/ChipInfoLib.h>
#include <ChipInfoDefs.h>
#include <PlatformInfoDefs.h>

/**
  ChipInfoType

  Container for all chip info
**/
typedef struct {
  BOOLEAN                     bInitComplete;                          /*  Driver has been initialized */
  UINT32                      nRawPartNum;                            /*  JTAG ID/Device Number */
  ChipInfoIdType              eChipInfoId;                            /*  Sequential ID, unique across all chips */
  ChipInfoVersionType         nVersion;                               /*  Major and Minor versions   */
  UINT32                      nRevNumber;                             /*  HW Revision number. e.g. 0 = v1.0, 1 = v1.1, 2 = v2.0 */
  ChipInfoFamilyType          eFamilyId;                              /*  Internal ChipInfo family (not architectural family)  */
  UINT32                      nFamilyDeviceId;                        /*  Device ID within this family, unique within the family */
  UINT32                      nRawFamilyId;                           /*  Architectural family number  */
  ChipInfoFoundryIdType       eFoundryId;                             /*  The foundry in which the chip was manufactured  */
  ChipInfoSerialNumType       nSerialNum;                             /*  Unique (within chip family) serial number  */
  ChipInfoQFPROMChipIdType    nQFPROMChipId;                          /*  Chip ID read from QFPROM fuses  */
  ChipInfoModemType           nModemSupported;                        /*  Modem hardware is/isnt' supported on this platform */
  CHAR8                       szChipIdString[CHIPINFO_MAX_ID_LENGTH]; /*  The chip's name as a string, e.g. "MSM8998" */
  UINT32                      nNumClusters;
  UINT32                      *aCPUClusters;                          /*  Bitmasks of disabled CPU cores for each of nClusters clusters */
  UINT32                      aDisabledFeatures[CHIPINFO_NUM_PARTS];  /*  Parts specific fuse data for supported parts (cam/video/modem etc) */
  ChipInfoFeatureCodeType     eFeatureCode;                           /*  Feature Code enum for this device */
  ChipInfoPCodeType           ePCode;                                 /*  PCode enum for this device */
  UINT32                      nNumFunctionalClusters;                 /* Number of clusters with at least 1 functional core */
  UINT32                      nBootCluster;                           /*  zero-indexed boot cluster */
  UINT32                      nBootCore;                              /*  zero-indexed boot core */
  UINT32                      nRawPackageType;                        /*  Raw package type */
  UINT32                      aPartialFeatures[CHIPINFO_NUM_PARTS];   /* Qultivate Fuse value for each part */
  UINT32                      *aNumCores;                             /*  Number of CPU cores for each of nClusters clusters */
  PlatformInfoPartInfoType    *aPartInfo;                             /* Per-instance Qultivate information for all parts in the Qultivate plan */
  UINT32                      nPartInfoLen;                           /* Number of entries in aPartInfo */
} ChipInfoCtxtType;
