/** @file
  Platform Device Tree Setup.

  Provides device tree initialization and management functions for the platform,
  including DTB selection, chip/board information retrieval, and DTB extension
  interface setup.

  Copyright (c) Qualcomm Technologies, Inc. and/or its subsidiaries. All rights reserved.
  SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#include <Uefi.h>
#include <Pi/PiBootMode.h>
#include <Pi/PiHob.h>
#include <Library/ChipInfoLib.h>
#include <Library/DebugLib.h>
#include <Library/DtFrameworkLib.h>
#include <Library/PlatformInfoLib.h>
#include <Library/PrePiLib.h>
#include <BootSharedImemCookie.h>
#include <PlatformInfoDefs.h>

STATIC CONST VOID  *mDtbBlob = NULL;

/**
  Initialize root FDT node handle for driver.

  @param[in,out]  Node  Pointer to FDT node handle to initialize.

  @retval  0           Success.
  @retval  Non-zero    FDT error code.

**/
INT32
SecFdtInitRootHandleForDriver (
  IN OUT DT_NODE_HANDLE  *Node
  )
{
  if (mDtbBlob == NULL) {
    DEBUG ((DEBUG_ERROR, "SecFdtInitRootHandle: mDtbBlob NULL!\n"));
    return -FDT_ERR_QC_NULLPTR;
  }

  return DtFwInitRootHandleForDriverByBlob (Node, mDtbBlob);
}

/**
  Get FDT node handle by name.

  @param[in,out]  Node  Pointer to FDT node handle.
  @param[in]      Name  Node name to search for.

  @retval  0           Success.
  @retval  Non-zero    FDT error code.

**/
INT32
SecFdtGetNodeHandle (
  IN OUT DT_NODE_HANDLE  *Node,
  IN CHAR8               *Name
  )
{
  if (mDtbBlob == NULL) {
    DEBUG ((DEBUG_ERROR, "SecFdtGetNodeHandle: mDtbBlob NULL!\n"));
    return -FDT_ERR_QC_NULLPTR;
  }

  return DtFwGetNodeHandle (Node, mDtbBlob, Name);
}

/**
  Initialize chip platform information structure.

  @param[out]  ChipPlatInfoProp  Pointer to DT_CHIP_PLAT_INFO structure to initialize.

  @retval  EFI_SUCCESS             Structure initialized successfully.
  @retval  EFI_INVALID_PARAMETER   ChipPlatInfoProp is NULL.

**/
EFI_STATUS
InitChipPlatInfoStruct (
  OUT DT_CHIP_PLAT_INFO  *ChipPlatInfoProp
  )
{
  if (ChipPlatInfoProp == NULL) {
    DEBUG ((DEBUG_ERROR, "Null parameter passed\n"));
    return EFI_INVALID_PARAMETER;
  }

  ChipPlatInfoProp->ChipFamily                         = 0x0;
  ChipPlatInfoProp->ChipId                             = 0x0;
  ChipPlatInfoProp->ChipMajVersion                     = 0x0;
  ChipPlatInfoProp->ChipMinVersion                     = 0x0;
  ChipPlatInfoProp->PlatformType                       = 0x0;
  ChipPlatInfoProp->PlatformSubtype                    = 0x0;
  ChipPlatInfoProp->PlatformMajVersion                 = 0x0;
  ChipPlatInfoProp->PlatformMinVersion                 = 0x0;
  ChipPlatInfoProp->OemVar                             = 0x0;
  ChipPlatInfoProp->DtbCompatibleStringStartsWith      = NULL;
  ChipPlatInfoProp->SocDtboCompatibleStringStartsWith  = NULL;
  ChipPlatInfoProp->PlatDtboCompatibleStringStartsWith = NULL;
  return EFI_SUCCESS;
}

/**
  Get chip and board information.

  Retrieves chip family, ID, version and platform information from
  the ChipInfo and PlatformInfo libraries.

  @param[out]  ChipPlatInfoProp  Pointer to DT_CHIP_PLAT_INFO to receive chip/platform info.

  @retval  EFI_SUCCESS             Information retrieved successfully.
  @retval  EFI_INVALID_PARAMETER   ChipPlatInfoProp is NULL.
  @retval  Other                   Error occurred during retrieval.

**/
EFI_STATUS
GetChipAndBoardInfo (
  OUT DT_CHIP_PLAT_INFO  *ChipPlatInfoProp
  )
{
  UINT32                         ChipInfoVersion;
  UINT32                         BoardInfoVersion;
  EFI_STATUS                     Status;
  PlatformInfoResult             PlatformInfoStatus;
  PlatformInfoPlatformInfoType   PlatformInfo;

  if (ChipPlatInfoProp == NULL) {
    DEBUG ((DEBUG_ERROR, "Null parameter passed\n"));
    return EFI_INVALID_PARAMETER;
  }

  ChipInfoVersion  = 0x0;
  BoardInfoVersion = 0x0;
  Status           = InitChipPlatInfoStruct (ChipPlatInfoProp);

  if (Status != EFI_SUCCESS) {
    return Status;
  }

  ChipInfoVersion                  = ChipInfoGetChipVersion ();
  ChipPlatInfoProp->ChipFamily     = (UINT16)ChipInfoGetChipFamily ();
  ChipPlatInfoProp->ChipId         = (UINT16)ChipInfoGetChipId ();
  ChipPlatInfoProp->ChipMajVersion = (UINT16)(ChipInfoVersion >> (sizeof (UINT16) * 8));
  ChipPlatInfoProp->ChipMinVersion = (UINT16)(ChipInfoVersion);

  PlatformInfoStatus = PlatformInfoInit ();
  if (PlatformInfoStatus != PLATFORMINFO_SUCCESS) {
    DEBUG ((DEBUG_ERROR, "PlatformInfoInit failed: %d\n", PlatformInfoStatus));
    return EFI_DEVICE_ERROR;
  }

  PlatformInfoStatus = PlatformInfoGetPlatformInfo (&PlatformInfo);
  if (PlatformInfoStatus != PLATFORMINFO_SUCCESS) {
    DEBUG ((DEBUG_ERROR, "PlatformInfoGetPlatformInfo failed: %d\n", PlatformInfoStatus));
    return EFI_DEVICE_ERROR;
  }

  BoardInfoVersion                     = PlatformInfo.Version;
  ChipPlatInfoProp->PlatformType       = (UINT8)PlatformInfo.PlatformType;
  ChipPlatInfoProp->PlatformSubtype    = (UINT8)PlatformInfo.Subtype;
  ChipPlatInfoProp->PlatformMajVersion = (UINT8)(BoardInfoVersion >> (sizeof (UINT16) * 8));
  ChipPlatInfoProp->PlatformMinVersion = (UINT8)(BoardInfoVersion);
  ChipPlatInfoProp->OemVar             = PlatformInfo.OemVariantId;

  return EFI_SUCCESS;
}

/**
  Get DTB base address and size.

  Retrieves the DTB blob address and size from either the boot shared IMEM
  cookie or the PCD values, depending on whether the cookie pointer is valid.

  @param[in]   BootSharedImemCookiePtr  Pointer to boot shared IMEM cookie structure.
  @param[out]  BlobSize                 Pointer to receive the DTB size.

  @retval  EFI_SUCCESS            DTB information retrieved successfully.
  @retval  EFI_INVALID_PARAMETER  BlobSize is NULL.
  @retval  EFI_DEVICE_ERROR       DTB address or size is invalid.

**/
EFI_STATUS
GetDtbInfo (
  IN  QUALCOMM_BOOT_SHARED_IMEM_COOKIE_TYPE  *BootSharedImemCookiePtr,
  OUT UINT32                                 *BlobSize
  )
{
  UINTN   CandidateBlob;
  UINTN   CandidatePcdBlob;
  UINT32  CandidateSize;
  UINT64  WindowBase;
  UINT64  WindowSize;
  UINT64  DtSize;

  if (BlobSize == NULL) {
    DEBUG ((DEBUG_ERROR, "GetDtbInfo: BlobSize is NULL\n"));
    return EFI_INVALID_PARAMETER;
  }

  if (BootSharedImemCookiePtr == NULL) {
    CandidatePcdBlob = PcdGet64 (PcdBootDtBase);
    DtSize           = PcdGet64 (PcdBootDtSize);
    if (CandidatePcdBlob == 0) {
      DEBUG ((DEBUG_ERROR, "GetDtbInfo: PcdBootDtBase is NULL\n"));
      return EFI_DEVICE_ERROR;
    }

    if (DtSize == 0) {
      DEBUG ((DEBUG_ERROR, "GetDtbInfo: PcdBootDtSize is zero\n"));
      return EFI_DEVICE_ERROR;
    }

    if (DtSize > MAX_UINT32) {
      DEBUG ((DEBUG_ERROR, "GetDtbInfo: PcdBootDtSize 0x%lx exceeds UINT32 max\n", DtSize));
      return EFI_DEVICE_ERROR;
    }

    mDtbBlob  = (CONST VOID *)CandidatePcdBlob;
    *BlobSize = (UINT32)DtSize;
  } else {
    /*
     * Read candidate values from IMEM and validate the range against the
     * PCD-defined DTB window BEFORE committing to the global DtbBlob pointer.
     * This prevents DtbBlob from being left pointing to an invalid address
     * if the range check fails.
     */
    CandidateBlob = BootSharedImemCookiePtr->BootDevtreeAddr;
    CandidateSize = BootSharedImemCookiePtr->BootDevtreeSize;

    if (CandidateSize == 0) {
      DEBUG ((DEBUG_ERROR, "GetDtbInfo: IMEM cookie BootDevtreeSize is zero\n"));
      return EFI_DEVICE_ERROR;
    }

    /*
     * Cache the PCD window values once so they are consistent across the
     * range check and the debug message, and to avoid recomputing them.
     */
    WindowBase = PcdGet64 (PcdBootDtBase);
    WindowSize = PcdGet64 (PcdBootDtSize);

    /*
     * Overflow-safe range check: avoid UINT64 addition which can wrap.
     * Condition 1: blob starts below the window base.
     * Condition 2: size alone exceeds the window (also prevents underflow
     *              in condition 3 by ensuring WindowSize >= CandidateSize).
     * Condition 3: blob end exceeds the window - expressed via subtraction
     *              only, which cannot overflow given conditions 1 and 2 pass.
     */
    if (((UINT64)CandidateBlob < WindowBase) ||
        (CandidateSize > WindowSize) ||
        ((UINT64)CandidateBlob - WindowBase > WindowSize - CandidateSize))
    {
      /*
       * Print base and size separately to avoid the overflow-prone addition
       * WindowBase + WindowSize in the format string.
       */
      DEBUG ((
        DEBUG_ERROR,
        "GetDtbInfo: IMEM cookie DTB 0x%lx size 0x%x out of range [0x%lx, 0x%lx+0x%lx)\n",
        (UINT64)CandidateBlob,
        CandidateSize,
        WindowBase,
        WindowBase,
        WindowSize
        ));
      return EFI_DEVICE_ERROR;
    }

    mDtbBlob  = (CONST VOID *)CandidateBlob;
    *BlobSize = CandidateSize;
  }

  /*
   * Both the PCD path and the IMEM path validate that the size is non-zero
   * before assigning *BlobSize, so this point is only reachable with a valid
   * non-zero size.  Assert the invariant rather than returning a runtime
   * error that can never fire.
   */
  ASSERT (*BlobSize != 0);
  return EFI_SUCCESS;
}

/**
  Initialize device tree blob.

  Selects appropriate DTB based on chip/platform information and sets up
  the DTB blob for use by the system.

  @retval  EFI_SUCCESS        DTB initialized successfully.
  @retval  EFI_DEVICE_ERROR   Error occurred during DTB initialization.

**/
EFI_STATUS
DtbInit (
  VOID
  )
{
  UINT32                                 DTBConfig[3];
  DT_NODE_HANDLE                         Node;
  EFI_STATUS                             Status;
  ChipInfoResult                         ChipInfoStatus;
  INTN                                   ReturnCode;
  UINT32                                 BlobSize;
  VOID                                   *DtbAddress;
  UINTN                                  DtbSize;
  QUALCOMM_BOOT_SHARED_IMEM_COOKIE_TYPE  *BootSharedImemCookiePtr;
  DT_CHIP_PLAT_INFO                      ChipPlatInfoProp;

  Status                  = EFI_DEVICE_ERROR;
  ReturnCode              = -1;
  DtbAddress              = NULL;
  DtbSize                 = 0x0;
  BootSharedImemCookiePtr = NULL;

  BootSharedImemCookiePtr = (QUALCOMM_BOOT_SHARED_IMEM_COOKIE_TYPE *)((UINT64)FixedPcdGet64 (PcdIMemCookiesBase));
  Status                  = GetDtbInfo (BootSharedImemCookiePtr, &BlobSize);
  if (Status != EFI_SUCCESS) {
    DEBUG ((DEBUG_ERROR, "GetDtbInfo failed: %r\n", Status));
    goto Exit;
  }

  ChipInfoStatus = ChipInfoInit ();
  if (ChipInfoStatus != CHIPINFO_SUCCESS) {
    DEBUG ((DEBUG_ERROR, "ChipInfoInit failed: %d\n", ChipInfoStatus));
    Status = EFI_DEVICE_ERROR;
    goto Exit;
  }

  Status = GetChipAndBoardInfo (&ChipPlatInfoProp);
  if (Status != EFI_SUCCESS) {
    goto Exit;
  }

  DEBUG ((
    DEBUG_INFO,
    "chip_family=0x%x chip_id=0x%x maj=0x%x min=0x%x\n",
    ChipPlatInfoProp.ChipFamily,
    ChipPlatInfoProp.ChipId,
    ChipPlatInfoProp.ChipMajVersion,
    ChipPlatInfoProp.ChipMinVersion
    ));

  Status     = EFI_DEVICE_ERROR; // Resetting Status back to Error
  ReturnCode = DtFwGetDt (
                 (UINTN)mDtbBlob,
                 (UINTN)0x0,
                 &ChipPlatInfoProp,
                 NULL,
                 0x0,
                 (UINTN *)&DtbAddress,
                 &DtbSize
                 );

  if ((ReturnCode != 0) || (DtbAddress == NULL) || (DtbSize == 0)) {
    DEBUG ((DEBUG_ERROR, "Status from get_dt: %d\n", ReturnCode));
    goto Exit;
  }

  if (BootSharedImemCookiePtr != NULL) {
    /*
     * BootDevtreeSize is a 32-bit field in QUALCOMM_BOOT_SHARED_IMEM_COOKIE_TYPE.
     * Validate the size before writing either field so the IMEM cookie is
     * never left in a partially-updated state (address written, size not).
     */
    if (DtbSize > MAX_UINT32) {
      DEBUG ((DEBUG_ERROR, "DtbInit: DtbSize 0x%lx exceeds UINT32 max\n", DtbSize));
      goto Exit;
    }

    BootSharedImemCookiePtr->BootDevtreeAddr = (UINT64)DtbAddress;
    BootSharedImemCookiePtr->BootDevtreeSize = (UINT32)DtbSize;
  }

  mDtbBlob   = DtbAddress;
  ReturnCode = DtFwSetBlobHandle (DtbAddress, DtbSize, DEFAULT_BLOB_ID);
  if (ReturnCode != 0) {
    DEBUG ((DEBUG_ERROR, "DtFwSetBlobHandle: %d\n", ReturnCode));
    goto Exit;
  }

  ReturnCode = DtFwGetNodeHandle (&Node, DtbAddress, "/sw/uefi");
  if (ReturnCode != 0) {
    DEBUG ((DEBUG_ERROR, "DtFwGetNodeHandle: %d\n", ReturnCode));
    goto Exit;
  }

  ReturnCode = DtFwGetUint32PropList (&Node, "config", (VOID *)&DTBConfig, sizeof (DTBConfig));
  if (ReturnCode != 0) {
    DEBUG ((DEBUG_ERROR, "DtFwGetUint32PropList: %d\n", ReturnCode));
    goto Exit;
  }

  // DTBconfig - /sw/uefi/config
  DEBUG (
    (DEBUG_WARN, "DTB config    : client[%x]..trace[%x]..verbose[%x]\n",
     DTBConfig[0], DTBConfig[1], DTBConfig[2])
    );

  Status = EFI_SUCCESS;

Exit:

  /*
   * On any failure path, reset mDtbBlob to NULL so that
   * SecFdtInitRootHandleForDriver and SecFdtGetNodeHandle correctly
   * return -FDT_ERR_QC_NULLPTR rather than operating on a stale or
   * partially-initialised blob.
   */
  if (Status != EFI_SUCCESS) {
    mDtbBlob = NULL;
  }

  return Status;
}
