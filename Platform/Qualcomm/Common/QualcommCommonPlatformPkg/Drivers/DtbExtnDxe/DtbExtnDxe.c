/** @file
  This DXE installs the Device Tree Extension protocol interface.

  Copyright (c) Qualcomm Technologies, Inc. and/or its subsidiaries.
  SPDX-License-Identifier: BSD-2-Clause-Patent

  @par Glossary:
    - DtbExtn - Device Tree Extension
**/

#include <Uefi.h>
#include <Library/BaseMemoryLib.h>
#include <Library/DebugLib.h>
#include <Library/HobLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Protocol/DeviceTreeExtension.h>

EFI_DTB_EXTN_PROTOCOL  gDtbExtnProtocol;

/**
  Retrieves the DTB Extension Protocol interface from a GUID HOB.

  This function searches for a GUID HOB identified by gEfiDtbExtnIntfHobGuid
  that contains a pointer to the EFI_DTB_EXTN_PROTOCOL interface. The protocol
  interface is typically created and stored in the HOB during the PEI phase,
  and this function allows DXE phase modules to retrieve and use it.

  @retval  EFI_DTB_EXTN_PROTOCOL*  Pointer to the DTB Extension Protocol interface
                                    if the HOB is found and contains a valid pointer.
  @retval  NULL                     If the GUID HOB is not found, or if the stored
                                    protocol pointer is NULL.

**/
EFI_DTB_EXTN_PROTOCOL *
GetDtbExtnIntf (
  VOID
  )
{
  EFI_HOB_GUID_TYPE      *GuidHob;
  UINTN                  **DataPtr;
  EFI_DTB_EXTN_PROTOCOL  *Result;

  GuidHob = GetFirstGuidHob (&gEfiDtbExtnIntfHobGuid);

  if (GuidHob == NULL) {
    return NULL;
  }

  DataPtr = GET_GUID_HOB_DATA (GuidHob);
  if (DataPtr == NULL) {
    return NULL;
  }

  Result = (EFI_DTB_EXTN_PROTOCOL *)*DataPtr;
  if (Result == NULL) {
    return NULL;
  }

  return Result;
}

/**
  Entry point for the DTB Extension DXE driver.

  This function is the entry point of the DTB Extension DXE driver. It retrieves
  the DTB Extension Protocol interface from a GUID HOB (created during PEI phase),
  copies the protocol structure to a global variable, and installs it as a protocol
  in the DXE phase so other drivers can locate and use it.

  @param[in]  ImageHandle  The firmware allocated handle for the EFI image.
  @param[in]  SystemTable  A pointer to the EFI System Table.

  @retval  EFI_SUCCESS           The protocol was successfully installed.
  @retval  EFI_NOT_FOUND         The DTB Extension interface HOB was not found.
  @retval  EFI_INVALID_PARAMETER A parameter was invalid (from InstallMultipleProtocolInterfaces).
  @retval  EFI_OUT_OF_RESOURCES  Insufficient resources to install the protocol.

**/
EFI_STATUS
EFIAPI
DtbExtnEntryPoint (
  IN EFI_HANDLE        ImageHandle,
  IN EFI_SYSTEM_TABLE  *SystemTable
  )
{
  EFI_DTB_EXTN_PROTOCOL  *DtbExtnIntfPtr;
  EFI_STATUS             Status;
  EFI_HANDLE             Handle;

  Handle = NULL;

  //
  // Retrieve the DTB Extension Protocol interface from HOB
  //
  DtbExtnIntfPtr = GetDtbExtnIntf ();
  if (DtbExtnIntfPtr == NULL) {
    DEBUG ((DEBUG_ERROR, "%a: Failed to locate DTB extension interface HOB\n", __func__));
    return EFI_NOT_FOUND;
  }

  //
  // Copy the protocol structure to global variable
  //
  CopyMem (&gDtbExtnProtocol, DtbExtnIntfPtr, sizeof (EFI_DTB_EXTN_PROTOCOL));

  //
  // Install the protocol so other drivers can locate it
  //
  Status = gBS->InstallMultipleProtocolInterfaces (
                  &Handle,
                  &gEfiDtbExtnProtocolGuid,
                  &gDtbExtnProtocol,
                  NULL
                  );

  if (EFI_ERROR (Status)) {
    DEBUG ((
      DEBUG_ERROR,
      "%a: Failed to install DTB extension protocol - %r\n",
      __func__,
      Status
      ));
    //
    // Clear the protocol structure on failure to maintain consistency
    //
    ZeroMem (&gDtbExtnProtocol, sizeof (EFI_DTB_EXTN_PROTOCOL));
    return Status;
  }

  return EFI_SUCCESS;
}
