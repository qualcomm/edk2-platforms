/** @file PlatformInfo.c

  Qualcomm Platform Information Library.

  Copyright (c) Qualcomm Technologies, Inc. and/or its subsidiaries.
  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#include <Base.h>
#include "PlatformInfoInternal.h"

/*
 * Driver context.
 */
PlatformInfoDrvCtxt  PlatformInfo_DriverCtxt;

/**
  Returns the platform type.

  @retval PlatformInfoPlatformType  The platform type.
**/
PlatformInfoPlatformType
PlatformInfo_Platform (
  VOID
  )
{
  PlatformInfoDrvCtxt  *pDrvCtxt;

  pDrvCtxt = PlatformInfo_GetDrvCtxt ();

  return pDrvCtxt->PlatformInfo.platform;
}

/**
  Returns the platform subtype.

  @retval UINT32  The platform subtype.
**/
UINT32
PlatformInfo_PlatformSubtype (
  VOID
  )
{
  PlatformInfoDrvCtxt  *pDrvCtxt;

  pDrvCtxt = PlatformInfo_GetDrvCtxt ();

  return pDrvCtxt->PlatformInfo.subtype;
}

/**
  Returns the platform version.

  @retval UINT32  The platform version.
**/
UINT32
PlatformInfo_PlatformVersion (
  VOID
  )
{
  PlatformInfoDrvCtxt  *pDrvCtxt;

  pDrvCtxt = PlatformInfo_GetDrvCtxt ();

  return pDrvCtxt->PlatformInfo.version;
}

/**
  Returns whether the platform is a fusion platform.

  @retval BOOLEAN  TRUE if fusion platform, FALSE otherwise.
**/
BOOLEAN
PlatformInfo_IsFusion (
  VOID
  )
{
  PlatformInfoDrvCtxt  *pDrvCtxt;

  pDrvCtxt = PlatformInfo_GetDrvCtxt ();

  return pDrvCtxt->PlatformInfo.fusion;
}

/**
  Retrieves the platform information.

  @param[out] pInfo  Pointer to the platform info structure to populate.

  @retval PLATFORMINFO_SUCCESS  Platform info retrieved successfully.
  @retval PLATFORMINFO_ERROR    pInfo is NULL.
**/
PlatformInfoResult
PlatformInfo_GetPlatformInfo (
  PlatformInfoPlatformInfoType  *pInfo
  )
{
  PlatformInfoDrvCtxt  *pDrvCtxt;

  pDrvCtxt = PlatformInfo_GetDrvCtxt ();

  if (pInfo != NULL) {
    *pInfo = pDrvCtxt->PlatformInfo;
    return PLATFORMINFO_SUCCESS;
  }

  return PLATFORMINFO_ERROR;
}

/**
  Retrieves the value associated with a platform key.

  @param[in]  eKey    The platform info key to look up.
  @param[out] nValue  Pointer to store the retrieved value.

  @retval PLATFORMINFO_SUCCESS  Key value retrieved successfully.
  @retval PLATFORMINFO_ERROR    Key not found or invalid parameters.
**/
PlatformInfoResult
PlatformInfo_GetKeyValue (
  PlatformInfoKeyType  eKey,
  UINT32               *nValue
  )
{
  PlatformInfoDrvCtxt  *pDrvCtxt;
  UINT32               Index;

  pDrvCtxt = PlatformInfo_GetDrvCtxt ();

  if ((eKey > PLATFORMINFO_NUM_KEYS) || (nValue == NULL)) {
    return PLATFORMINFO_ERROR;
  }

  for (Index = 0; Index < pDrvCtxt->nNumKVPS; Index++) {
    if (eKey == pDrvCtxt->aKVPS[Index].nKey) {
      *nValue = pDrvCtxt->aKVPS[Index].nValue;
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
PlatformInfo_OEMVariant (
  VOID
  )
{
  PlatformInfoDrvCtxt  *pDrvCtxt;

  pDrvCtxt = PlatformInfo_GetDrvCtxt ();

  return pDrvCtxt->PlatformInfo.OEMVariantID;
}

/**
  Returns a pointer to the driver context, initializing it if necessary.

  @retval PlatformInfoDrvCtxt*  Pointer to the driver context.
**/
PlatformInfoDrvCtxt *
PlatformInfo_GetDrvCtxt (
  VOID
  )
{
  STATIC BOOLEAN  bInitialized = FALSE;

  if (!bInitialized) {
    bInitialized = TRUE;
    PlatformInfo_Init ();
  }

  return &PlatformInfo_DriverCtxt;
}
