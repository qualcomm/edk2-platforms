/** @file

  Qualcomm Platform Information Library.

  Copyright (c) Qualcomm Technologies, Inc. and/or its subsidiaries.<BR>
  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#include <Base.h>
#include "PlatformInfoInternal.h"

/*
 * Driver context.
 */
PlatformInfoDrvCtxt  mPlatformInfoDrvCtxt;

/**
  Returns the platform type.

  @retval PlatformInfoPlatformType  The platform type.
**/
PlatformInfoPlatformType
EFIAPI
PlatformInfoGetPlatformType (
  VOID
  )
{
  PlatformInfoDrvCtxt  *DrvCtxt;

  DrvCtxt = PlatformInfoGetDrvCtxt ();

  return DrvCtxt->PlatformInfo.PlatformType;
}

/**
  Returns the platform subtype.

  @retval UINT32  The platform subtype.
**/
UINT32
EFIAPI
PlatformInfoGetPlatformSubtype (
  VOID
  )
{
  PlatformInfoDrvCtxt  *DrvCtxt;

  DrvCtxt = PlatformInfoGetDrvCtxt ();

  return DrvCtxt->PlatformInfo.Subtype;
}

/**
  Returns the platform version.

  @retval UINT32  The platform version.
**/
UINT32
EFIAPI
PlatformInfoGetPlatformVersion (
  VOID
  )
{
  PlatformInfoDrvCtxt  *DrvCtxt;

  DrvCtxt = PlatformInfoGetDrvCtxt ();

  return DrvCtxt->PlatformInfo.Version;
}

/**
  Returns whether the platform is a fusion platform.

  @retval BOOLEAN  TRUE if fusion platform, FALSE otherwise.
**/
BOOLEAN
EFIAPI
PlatformInfoIsFusion (
  VOID
  )
{
  PlatformInfoDrvCtxt  *DrvCtxt;

  DrvCtxt = PlatformInfoGetDrvCtxt ();

  return DrvCtxt->PlatformInfo.Fusion;
}

/**
  Retrieves the platform information.

  @param[out] Info  Pointer to the platform info structure to populate.

  @retval PLATFORMINFO_SUCCESS  Platform info retrieved successfully.
  @retval PLATFORMINFO_ERROR    Info is NULL.
**/
PlatformInfoResult
EFIAPI
PlatformInfoGetPlatformInfo (
  OUT PlatformInfoPlatformInfoType  *Info
  )
{
  PlatformInfoDrvCtxt  *DrvCtxt;

  DrvCtxt = PlatformInfoGetDrvCtxt ();

  if (Info != NULL) {
    *Info = DrvCtxt->PlatformInfo;
    return PLATFORMINFO_SUCCESS;
  }

  return PLATFORMINFO_ERROR;
}

/**
  Retrieves the value associated with a platform key.

  @param[in]  Key    The platform info key to look up.
  @param[out] Value  Pointer to store the retrieved value.

  @retval PLATFORMINFO_SUCCESS  Key value retrieved successfully.
  @retval PLATFORMINFO_ERROR    Key not found or invalid parameters.
**/
PlatformInfoResult
EFIAPI
PlatformInfoGetKeyValue (
  IN  PlatformInfoKeyType  Key,
  OUT UINT32               *Value
  )
{
  PlatformInfoDrvCtxt  *DrvCtxt;
  UINT32               Index;

  DrvCtxt = PlatformInfoGetDrvCtxt ();

  if ((Key >= PLATFORMINFO_NUM_KEYS) || (Value == NULL)) {
    return PLATFORMINFO_ERROR;
  }

  for (Index = 0; Index < DrvCtxt->NumKvps; Index++) {
    if (Key == DrvCtxt->Kvps[Index].Key) {
      *Value = DrvCtxt->Kvps[Index].Value;
      return PLATFORMINFO_SUCCESS;
    }
  }

  return PLATFORMINFO_ERROR;
}

/**
  Returns the OEM variant ID.

  @retval UINT32  The OEM variant ID.
**/
UINT32
EFIAPI
PlatformInfoGetOemVariant (
  VOID
  )
{
  PlatformInfoDrvCtxt  *DrvCtxt;

  DrvCtxt = PlatformInfoGetDrvCtxt ();

  return DrvCtxt->PlatformInfo.OemVariantId;
}

/**
  Returns a pointer to the driver context, initializing it if necessary.

  @retval PlatformInfoDrvCtxt*  Pointer to the driver context.
**/
PlatformInfoDrvCtxt *
EFIAPI
PlatformInfoGetDrvCtxt (
  VOID
  )
{
  STATIC BOOLEAN  Initialized = FALSE;

  if (!Initialized) {
    Initialized = TRUE;
    PlatformInfoInit ();
  }

  return &mPlatformInfoDrvCtxt;
}
