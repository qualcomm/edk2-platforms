/** @file
  Public interface include file for accessing the PlatformInfo driver.

  Copyright (c) Qualcomm Technologies, Inc. and/or its subsidiaries.
  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#pragma once

#include <Base.h>
#include <PlatformInfoDefs.h>

/**
  Return the platform type.
*/
PlatformInfoPlatformType
PlatformInfo_Platform (
  VOID
  );

/**
  Return the platform subtype.
*/
UINT32
PlatformInfo_PlatformSubtype (
  VOID
  );

/**
  Return the platform version.
*/
UINT32
PlatformInfo_PlatformVersion (
  VOID
  );

/**
  Return if the platform is a Fusion variant.
*/
BOOLEAN
PlatformInfo_IsFusion (
  VOID
  );

/**
  Get a key value.

  This function returns a key value stored in the CDT.

  @param[in]  eKey   Key to get the value for.
  @param[out] pValue Key value.

  @return
  PLATFORMINFO_SUCCESS -- Key found and value stored in pValue. \n
  PLATFORMINFO_ERROR -- Key not found.
*/
PlatformInfoResult
PlatformInfo_GetKeyValue (
  PlatformInfoKeyType  eKey,
  UINT32               *pValue
  );

/**
  Return the OEM Variant.

  OEM variants are OEM-specific customizations of a specific platform
  type, subtype, and version.

  Variant ID 0 => Qualcomm platform without any modifications.
  IDs 1-255 are for OEM use.

  OEM variants serve a different purpose from the OEM platform type:
    OEM variants identify modifications to one of the platform configurations
    that Qualcomm provides, e.g. MTP subtype 1 v1.0, OEM Variant 1.

    The OEM platform type identifies platform types that aren't provided by
    Qualcomm (i.e. not MTP, QRD, TTP, etc.)

  With the exception of Variant 0, OEM variants are specific to each OEM
  and are not tracked by Qualcomm.

  Added with CDT Format v5. Earlier formats will return Variant 0.
*/
UINT32
PlatformInfo_OEMVariant (
  VOID
  );

/**
  Returns information on the current platform.

  This function returns information about the current platform in the parameter
  pInfo.

  @param[out] pInfo Variable to return to the caller.

  @return
  PLATFORMINFO_SUCCESS -- Platform identified and placed in pType. \n
  PLATFORMINFO_ERROR   -- Invalid parameter.
*/
PlatformInfoResult
PlatformInfo_GetPlatformInfo (
  PlatformInfoPlatformInfoType  *pInfo
  );

/**
  Initialize the PlatformInfo driver.
*/
PlatformInfoResult
PlatformInfo_Init (
  VOID
  );
