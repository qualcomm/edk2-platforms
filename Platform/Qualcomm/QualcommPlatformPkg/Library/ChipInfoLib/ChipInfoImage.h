/** @file
 Internal ChipInfo header with common APIs to acc

 Copyright (c) Qualcomm Technologies, Inc. and/or its subsidiaries.
 SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#pragma once

#include <Base.h>
#include <PlatformInfoDefs.h>

/*
 * Common way to access the socinfo SMEM region,
 * since not all images have an SMEM driver for accessing socinfo.
 *
 * @return  A pointer to the socinfo structure if successful,
 *          NULL if not successful
 */
PlatformInfoSMemType *
ChipInfo_GetSocInfo (
  VOID
  );

/*
 * Unmap the SMEM region once it's no longer needed.
 */
VOID
ChipInfo_UnmapSMem (
  VOID
  );

/*
 * Common way to dynamically allocate memory on all images, since
 * different images might use different memory allocation functions.
 *
 * @param[in] nSize  The size in bytes to allocate
 *
 * @return  Pointer to allocated memory, if successful
 *          NULL if not successful.
 */
VOID *
ChipInfo_malloc (
  UINT32  nSize
  );

/*
 * Common way to copy memory on all images, since not all images
 * use the same safe-memcpy function.
 *
 * This is a wrapper around the image's safe memcpy, and doesn't
 * provide any additional functionality other than some minor
 * input validation.
 *
 * @param[in,out] pDest The destination buffer
 * @param[in] nDestLen  The size of the destination buffer
 * @param[in] pSrc      The source buffer
 * @param[in] nSrcLen   The number of bytes to copy from pSrc into pDest
 *
 * @return  A pointer to the destination buffer, if successful
 *          NULL if any of the inputs is NULL or 0, or if the
 *            safe memcpy indicates an error
 */
VOID *
ChipInfo_memcpy (
  VOID *restrict        pDest,
  UINT32                nDestLen,
  CONST VOID *restrict  pSrc,
  UINT32                nSrcLen
  );

/*
 * Common way to copy strings on all images, since different images
 * use different versions of a safe string copy. This function is a
 * wrapper around the image's safe string copy, and doesn't
 * implement anything extra other than some minor input validation.
 *
 * @param[in,out] szDest    The destination buffer
 * @param[in]     szSrc     The NULL-terminated source buffer
 * @param[in]     mDestLen  The size of the destination buffer.
 *                          At most nDestLen-1 characters will
 *                          be copied into szDest, and the buffer
 *                          will be NULL-terminated.
 *
 * @return  A pointer to the (possibly truncated) destination buffer,
 *            if successful
 *          NULL if one of the inputs is NULL or 0
 */
CHAR8 *
ChipInfo_strcpy (
  CHAR8 *restrict        szDest,
  CONST CHAR8 *restrict  szSrc,
  UINT32                 nDestLen
  );
