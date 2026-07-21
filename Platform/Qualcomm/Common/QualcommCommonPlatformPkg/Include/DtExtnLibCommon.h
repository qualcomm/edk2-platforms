/** @file
  DtExtnLib public type definitions and constants.

  This file defines DtExtnLib error codes, blob identifiers,
  common macros, and node handle types.

  Copyright (c) Qualcomm Technologies, Inc. and/or its subsidiaries.
  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#pragma once

/* FdtLib error codes end here: #define FDT_ERR_MAX 17 (see libfdt.h) */
/* QC added error codes */
#define FDT_ERR_QC_NOERROR         0
#define FDT_ERR_QC_NULLPTR         1000
#define FDT_ERR_QC_TRUNCATED       1001
#define FDT_ERR_QC_BUF2SMALL       1002
#define FDT_ERR_QC_NILVALUE        1003
#define FDT_ERR_QC_SDNULL          1004
#define FDT_ERR_QC_BADFORMAT       1005
#define FDT_ERR_QC_BADID           1006
#define FDT_ERR_QC_INPUT_ARG_ERR   1007
#define FDT_ERR_QC_REGIDX          1008
#define FDT_ERR_QC_TARGETIDX       1009
#define FDT_ERR_QC_MEMALLOC        1010
#define FDT_ERR_QC_BLOBID          1011
#define FDT_ERR_QC_NOTSUPPORTED    1012
#define FDT_ERR_QC_NOTREADY        1013
#define FDT_ERR_QC_NODE_DIFFERENT  1014
#define FDT_ERR_QC_SLICE_RANGE     1015
#define FDT_ERR_QC_SLICE_COUNT     1016
#define FDT_ERR_QC_BAD_SELECTOR    1017
#define FDT_ERR_QC_FDTLIB_ERROR    1018
#define FDT_ERR_QC_OVERFLOW        1019
#define FDT_ERR_QC_UNDERFLOW       1020

/* Blob ID */
#define DEFAULT_BLOB_ID  0
#define MAX_BLOB_ID      5

/* Init Value for fdt_node_handle struct */
#define INIT_FDT_NODE_HANDLE  {NULL,0}

/* fdt_get_reg() size code */
#define SIZE_NIL   0
#define SIZE_32    1
#define SIZE_64    2
#define SIZE_BLOB  -1

typedef struct {
  const void    *blob;          /* pointer to DTB blob, opaque to client */
  int           offset;         /* offset of this node within DTB blob */
} fdt_node_handle;
