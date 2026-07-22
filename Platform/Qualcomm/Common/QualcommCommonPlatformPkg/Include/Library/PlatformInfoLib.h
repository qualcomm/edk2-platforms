/** @file
  Public interface include file for accessing the PlatformInfo driver.

  Copyright (c) Qualcomm Technologies, Inc. and/or its subsidiaries.<BR>
  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#pragma once

#include <Base.h>
#include <PlatformInfoDefs.h>

/**
  Return the platform type.
*/
PlatformInfoPlatformType
EFIAPI
PlatformInfoGetPlatformType (
  VOID
  );

/**
  Return the platform subtype.
*/
UINT32
EFIAPI
PlatformInfoGetPlatformSubtype (
  VOID
  );

/**
  Return the platform version.
*/
UINT32
EFIAPI
PlatformInfoGetPlatformVersion (
  VOID
  );

/**
  Return if the platform is a Fusion variant.
*/
BOOLEAN
EFIAPI
PlatformInfoIsFusion (
  VOID
  );

/**
  Get a key value.

  This function returns a key value stored in the CDT.

  @param[in]  Key    Key to get the value for.
  @param[out] Value  Key value.

  @return
  PLATFORMINFO_SUCCESS -- Key found and value stored in Value. \n
  PLATFORMINFO_ERROR -- Key not found.
*/
PlatformInfoResult
EFIAPI
PlatformInfoGetKeyValue (
  IN  PlatformInfoKeyType  Key,
  OUT UINT32               *Value
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
EFIAPI
PlatformInfoGetOemVariant (
  VOID
  );

/**
  Returns information on the current platform.

  This function returns information about the current platform in the parameter
  Info.

  @param[out] Info  Variable to return to the caller.

  @return
  PLATFORMINFO_SUCCESS -- Platform identified and placed in Info. \n
  PLATFORMINFO_ERROR   -- Invalid parameter.
*/
PlatformInfoResult
EFIAPI
PlatformInfoGetPlatformInfo (
  OUT PlatformInfoPlatformInfoType  *Info
  );

/**
  Initialize the PlatformInfo driver.
*/
PlatformInfoResult
EFIAPI
PlatformInfoInit (
  VOID
  );
