/** @file
  BaseDtFrameworkLib - real implementation.

  UEFI-style wrappers for Qualcomm DTFramework Device Tree APIs.
  Each function delegates directly to the corresponding DTFramework
  fdt_* C function declared in DTBExtnLib.h.

  The DTFramework library (DtFrameworkExtnDtLib) must be present in the
  build for this implementation to link.

  Copyright (c) Qualcomm Technologies, Inc. and/or its subsidiaries.
  SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#include <Uefi.h>
#include <PiPei.h>
#include <DTBExtnLib.h>
#include <Library/DtFrameworkLib.h>
#include <Library/HobLib.h>
#include <Protocol/DeviceTreeExtension.h>
#include <get_dt.h>

/*
 * Compile-time layout verification: DT_CHIP_PLAT_INFO is a UEFI-typed mirror
 * of chip_plat_info_property.  DtFwGetDt casts between the two types, so
 * their sizes must be identical.  If DTFramework ever adds or reorders fields
 * in chip_plat_info_property this assertion will catch the mismatch at build
 * time rather than silently producing incorrect behaviour at run time.
 */
STATIC_ASSERT (
  sizeof (DT_CHIP_PLAT_INFO) == sizeof (chip_plat_info_property),
  "DT_CHIP_PLAT_INFO size mismatch with chip_plat_info_property - update DtFrameworkLib.h"
  );

/**
  Select the best-matching DTB from a packed image.

  Thin wrapper around the DTFramework get_dt() function.
  See DtFrameworkLib.h for full parameter and return value documentation.

  @param[in]   DtbsImageStartAddress  Base address of the packed DTB image.
  @param[in]   DtbsImageSize          Size of the packed DTB image.
  @param[in]   ChipPlatInfoProp       Chip and platform identification.
  @param[in]   PropList               Optional caller-defined property list.
  @param[in]   PropNumEntries         Number of entries in PropList.
  @param[out]  DtbAddr                Receives the selected DTB address.
  @param[out]  DtbSize                Receives the selected DTB size.

  @retval  0    DTB selected successfully.
  @retval  -1   Selection failed.

**/
INT32
EFIAPI
DtFwGetDt (
  IN  UINTN              DtbsImageStartAddress,
  IN  UINTN              DtbsImageSize,
  IN  DT_CHIP_PLAT_INFO  *ChipPlatInfoProp,
  IN  VOID               *PropList,
  IN  UINT32             PropNumEntries,
  OUT UINTN              *DtbAddr,
  OUT UINTN              *DtbSize
  )
{
  return get_dt (
           DtbsImageStartAddress,
           (size_t)DtbsImageSize,
           (chip_plat_info_property *)ChipPlatInfoProp,
           (property_list *)PropList,
           PropNumEntries,
           DtbAddr,
           DtbSize
           );
}

/**
  Register a DTB blob with the DTFramework blob manager.

  Thin wrapper around fdt_set_blob_handle().
  See DtFrameworkLib.h for full parameter and return value documentation.

  @param[in]  Blob    Pointer to the device tree blob.
  @param[in]  BSize   Size of the blob in bytes.
  @param[in]  BlobId  Slot index.

  @retval  FDT_ERR_QC_NOERROR  Handle registered successfully.
  @retval  Other               Error code from DTFramework.

**/
INT32
EFIAPI
DtFwSetBlobHandle (
  IN CONST VOID  *Blob,
  IN UINTN       BSize,
  IN INT32       BlobId
  )
{
  return fdt_set_blob_handle (Blob, (size_t)BSize, BlobId);
}

/**
  Get an FDT node handle by path name.

  Thin wrapper around fdt_get_node_handle().
  See DtFrameworkLib.h for full parameter and return value documentation.

  @param[in,out]  Node  Pointer to the DT_NODE_HANDLE to populate.
  @param[in]      Blob  Pointer to the device tree blob.
  @param[in]      Name  Full path of the node.

  @retval  FDT_ERR_QC_NOERROR  Node found and handle populated.
  @retval  Other               Error code from DTFramework.

**/
INT32
EFIAPI
DtFwGetNodeHandle (
  IN OUT DT_NODE_HANDLE  *Node,
  IN     CONST VOID      *Blob,
  IN     CHAR8           *Name
  )
{
  return fdt_get_node_handle (Node, Blob, Name);
}

/**
  Initialize a root FDT node handle using the default registered blob.

  Uses DEFAULT_BLOB_ID to look up the blob registered via DtFwSetBlobHandle().
  See DtFrameworkLib.h for full parameter and return value documentation.

  @param[in,out]  Node  Pointer to the DT_NODE_HANDLE to initialise.

  @retval  FDT_ERR_QC_NOERROR  Handle initialised.
  @retval  Other               Error code from DTFramework.

**/
INT32
EFIAPI
DtFwInitRootHandleForDriver (
  IN OUT DT_NODE_HANDLE  *Node
  )
{
  /*
   * Use the by-ID variant with DEFAULT_BLOB_ID (0) to look up the blob
   * registered via DtFwSetBlobHandle().  Using the explicit by-ID API
   * makes the intent unambiguous.
   */
  return fdt_init_root_handle_for_driver_by_id (Node, DEFAULT_BLOB_ID);
}

/**
  Initialize a root FDT node handle using an explicit blob pointer.

  Thin wrapper around fdt_init_root_handle_for_driver().
  See DtFrameworkLib.h for full parameter and return value documentation.

  @param[in,out]  Node  Pointer to the DT_NODE_HANDLE to initialise.
  @param[in]      Blob  Pointer to the device tree blob.

  @retval  FDT_ERR_QC_NOERROR  Handle initialised.
  @retval  Other               Error code from DTFramework.

**/
INT32
EFIAPI
DtFwInitRootHandleForDriverByBlob (
  IN OUT DT_NODE_HANDLE  *Node,
  IN     CONST VOID      *Blob
  )
{
  return fdt_init_root_handle_for_driver (Node, Blob);
}

/**
  Get the number of direct subnodes of a node.

  Thin wrapper around fdt_get_count_of_subnodes().
  See DtFrameworkLib.h for full parameter and return value documentation.

  @param[in]   Node   Parent node handle.
  @param[out]  Count  Receives the subnode count.

  @retval  FDT_ERR_QC_NOERROR  Count returned.
  @retval  Other               Error code from DTFramework.

**/
INT32
EFIAPI
DtFwGetCountOfSubnodes (
  IN  DT_NODE_HANDLE  *Node,
  OUT UINT32          *Count
  )
{
  return fdt_get_count_of_subnodes (Node, Count);
}

/**
  Get a cached array of direct subnode handles.

  Thin wrapper around fdt_get_cache_of_subnodes().
  See DtFrameworkLib.h for full parameter and return value documentation.

  @param[in]   Node   Parent node handle.
  @param[out]  Cache  Array of DT_NODE_HANDLE to populate.
  @param[in]   Count  Number of entries in Cache.

  @retval  FDT_ERR_QC_NOERROR  Cache populated.
  @retval  Other               Error code from DTFramework.

**/
INT32
EFIAPI
DtFwGetCacheOfSubnodes (
  IN  DT_NODE_HANDLE  *Node,
  OUT DT_NODE_HANDLE  *Cache,
  IN  UINT32          Count
  )
{
  return fdt_get_cache_of_subnodes (Node, Cache, Count);
}

/**
  Get a register (address/size pair) property from a node.

  Thin wrapper around fdt_get_reg().
  See DtFrameworkLib.h for full parameter and return value documentation.

  @param[in]   Node      Node handle.
  @param[in]   RegName   Name of the reg-names entry, or NULL.
  @param[in]   RegIndex  Zero-based index into the reg array.
  @param[in]   AddrCode  Address cell size code.
  @param[in]   SizeCode  Size cell size code.
  @param[out]  RegPaddr  Receives the physical base address.
  @param[out]  RegSize   Receives the region size.

  @retval  FDT_ERR_QC_NOERROR  Register entry returned.
  @retval  Other               Error code from DTFramework.

**/
INT32
EFIAPI
DtFwGetReg (
  IN  DT_NODE_HANDLE  *Node,
  IN  CHAR8           *RegName,
  IN  INT32           RegIndex,
  IN  INT32           AddrCode,
  IN  INT32           SizeCode,
  OUT UINT64          *RegPaddr,
  OUT UINT64          *RegSize
  )
{
  return fdt_get_reg (Node, RegName, RegIndex, AddrCode, SizeCode, RegPaddr, RegSize);
}

/**
  Get the byte size of a named property.

  Thin wrapper around fdt_get_prop_size().
  See DtFrameworkLib.h for full parameter and return value documentation.

  @param[in]   Node      Node handle.
  @param[in]   PropName  Property name.
  @param[out]  Size      Receives the property size in bytes.

  @retval  FDT_ERR_QC_NOERROR  Size returned.
  @retval  Other               Error code from DTFramework.

**/
INT32
EFIAPI
DtFwGetPropSize (
  IN  DT_NODE_HANDLE  *Node,
  IN  CONST CHAR8     *PropName,
  OUT UINT32          *Size
  )
{
  return fdt_get_prop_size (Node, PropName, Size);
}

/**
  Get a UINT8 property value from a node.

  Thin wrapper around fdt_get_uint8_prop().
  See DtFrameworkLib.h for full parameter and return value documentation.

  @param[in]   Node      Node handle.
  @param[in]   PropName  Property name.
  @param[out]  Value     Receives the UINT8 value.

  @retval  FDT_ERR_QC_NOERROR  Value returned.
  @retval  Other               Error code from DTFramework.

**/
INT32
EFIAPI
DtFwGetUint8Prop (
  IN  DT_NODE_HANDLE  *Node,
  IN  CONST CHAR8     *PropName,
  OUT UINT8           *Value
  )
{
  return fdt_get_uint8_prop (Node, PropName, Value);
}

/**
  Get a UINT32 property value from a node.

  Thin wrapper around fdt_get_uint32_prop().
  See DtFrameworkLib.h for full parameter and return value documentation.

  @param[in]   Node      Node handle.
  @param[in]   PropName  Property name.
  @param[out]  Value     Receives the UINT32 value.

  @retval  FDT_ERR_QC_NOERROR  Value returned.
  @retval  Other               Error code from DTFramework.

**/
INT32
EFIAPI
DtFwGetUint32Prop (
  IN  DT_NODE_HANDLE  *Node,
  IN  CONST CHAR8     *PropName,
  OUT UINT32          *Value
  )
{
  return fdt_get_uint32_prop (Node, PropName, Value);
}

/**
  Get a list of UINT32 property values from a node.

  Thin wrapper around fdt_get_uint32_prop_list().
  See DtFrameworkLib.h for full parameter and return value documentation.

  @param[in]   Node      Node handle.
  @param[in]   PropName  Property name.
  @param[out]  PropList  Buffer to receive the UINT32 values.
  @param[in]   Size      Size of PropList in bytes.

  @retval  FDT_ERR_QC_NOERROR  Values returned.
  @retval  Other               Error code from DTFramework.

**/
INT32
EFIAPI
DtFwGetUint32PropList (
  IN  DT_NODE_HANDLE  *Node,
  IN  CONST CHAR8     *PropName,
  OUT UINT32          *PropList,
  IN  UINT32          Size
  )
{
  return fdt_get_uint32_prop_list (Node, PropName, PropList, Size);
}

/**
  Get a UINT64 property value from a node.

  Thin wrapper around fdt_get_uint64_prop().
  See DtFrameworkLib.h for full parameter and return value documentation.

  @param[in]   Node      Node handle.
  @param[in]   PropName  Property name.
  @param[out]  Value     Receives the UINT64 value.

  @retval  FDT_ERR_QC_NOERROR  Value returned.
  @retval  Other               Error code from DTFramework.

**/
INT32
EFIAPI
DtFwGetUint64Prop (
  IN  DT_NODE_HANDLE  *Node,
  IN  CONST CHAR8     *PropName,
  OUT UINT64          *Value
  )
{
  return fdt_get_uint64_prop (Node, PropName, Value);
}

/**
  Get a string (or string list) property from a node.

  Thin wrapper around fdt_get_string_prop_list().
  See DtFrameworkLib.h for full parameter and return value documentation.

  @param[in]   Node        Node handle.
  @param[in]   PropName    Property name.
  @param[out]  StringList  Buffer to receive the string data.
  @param[in]   Size        Size of StringList in bytes.

  @retval  FDT_ERR_QC_NOERROR  Strings returned.
  @retval  Other               Error code from DTFramework.

**/
INT32
EFIAPI
DtFwGetStringPropList (
  IN  DT_NODE_HANDLE  *Node,
  IN  CONST CHAR8     *PropName,
  OUT CHAR8           *StringList,
  IN  UINT32          Size
  )
{
  return fdt_get_string_prop_list (Node, PropName, StringList, Size);
}

/**
  Get the total byte size needed to hold all property names of a node.

  Thin wrapper around fdt_get_prop_names_size_of_node().
  See DtFrameworkLib.h for full parameter and return value documentation.

  @param[in]   Node  Node handle.
  @param[out]  Size  Receives the required buffer size in bytes.

  @retval  FDT_ERR_QC_NOERROR  Size returned.
  @retval  Other               Error code from DTFramework.

**/
INT32
EFIAPI
DtFwGetPropNamesSizeOfNode (
  IN  DT_NODE_HANDLE  *Node,
  OUT UINT32          *Size
  )
{
  return fdt_get_prop_names_size_of_node (Node, Size);
}

/**
  Get all property names of a node into a caller-supplied buffer.

  Thin wrapper around fdt_get_prop_names_of_node().
  See DtFrameworkLib.h for full parameter and return value documentation.

  @param[in]   Node       Node handle.
  @param[out]  PropNames  Buffer to receive the packed name strings.
  @param[in]   Size       Size of PropNames in bytes.

  @retval  FDT_ERR_QC_NOERROR  Names returned.
  @retval  Other               Error code from DTFramework.

**/
INT32
EFIAPI
DtFwGetPropNamesOfNode (
  IN  DT_NODE_HANDLE  *Node,
  OUT CHAR8           *PropNames,
  IN  UINT32          Size
  )
{
  return fdt_get_prop_names_of_node (Node, PropNames, Size);
}

/**
  Static wrapper: FDT_INIT_ROOT_HANDLE_FOR_DRIVER takes only (Node) but
  fdt_init_root_handle_for_driver_by_id takes (Node, BlobId).

  @param[in,out]  Node  Pointer to the FDT node handle to initialise.

  @retval  FDT_ERR_QC_NOERROR  Handle initialised using DEFAULT_BLOB_ID.
  @retval  Other               Error code from DTFramework.

**/
STATIC INT32
DtbExtnInitRootHandleForDriver (
  IN OUT FDT_NODE_HANDLE  *Node
  )
{
  return fdt_init_root_handle_for_driver_by_id (Node, DEFAULT_BLOB_ID);
}

/**
  Static wrapper: FDT_GET_NODE_HANDLE takes (Node, Name) but
  fdt_get_node_handle takes (Node, Blob, Name).  Retrieve the default
  blob via fdt_get_blob_handle and forward the call.

  @param[in,out]  Node  Pointer to the FDT node handle to populate.
  @param[in]      Name  Full path of the node.

  @retval  FDT_ERR_QC_NOERROR  Node found and handle populated.
  @retval  Other               Error code from DTFramework.

**/
STATIC INT32
DtbExtnGetNodeHandle (
  IN OUT FDT_NODE_HANDLE  *Node,
  IN CHAR8                *Name
  )
{
  CONST VOID  *Blob;
  INT32       Ret;

  Blob = NULL;
  Ret  = fdt_get_blob_handle (&Blob, DEFAULT_BLOB_ID);
  if (Ret != 0) {
    return Ret;
  }

  return fdt_get_node_handle (Node, Blob, Name);
}

/**
  Build a PEI GUID HOB containing a pointer to the populated
  EFI_DTB_EXTN_PROTOCOL interface.

  @retval  EFI_SUCCESS  HOB built successfully.

**/
EFI_STATUS
EFIAPI
DtFwPublishDtbExtnIntfHob (
  VOID
  )
{
  STATIC EFI_DTB_EXTN_PROTOCOL  DtbExtnProtocol;
  EFI_DTB_EXTN_PROTOCOL         *pDtbExtnProtocol;

  //
  // Populate the protocol vtable with DTFramework C function pointers.
  // Wrapper functions are used where the protocol signature differs from
  // the underlying DTFramework function signature.
  //
  DtbExtnProtocol.Version                           = EFI_DTB_EXTN_PROTOCOL_VERSION;
  DtbExtnProtocol.FdtInitRootHandleForDriver        = (FDT_INIT_ROOT_HANDLE_FOR_DRIVER)DtbExtnInitRootHandleForDriver;
  DtbExtnProtocol.FdtGetNameIndex                   = (FDT_GET_NAME_INDEX)fdt_get_name_index;
  DtbExtnProtocol.FdtGetNextNodeHandleForCompatible = (FDT_GET_NEXT_NODE_HANDLE_FOR_COMPATIBLE)fdt_get_next_node_handle_for_compatible;
  DtbExtnProtocol.FdtGetReg                         = (FDT_GET_REG)fdt_get_reg;
  DtbExtnProtocol.FdtGetCountOfSubnodes             = (FDT_GET_COUNT_OF_SUBNODES)fdt_get_count_of_subnodes;
  DtbExtnProtocol.FdtGetNodeHandle                  = (FDT_GET_NODE_HANDLE)DtbExtnGetNodeHandle;
  DtbExtnProtocol.FdtGetParentNode                  = (FDT_GET_PARENT_NODE)fdt_get_parent_node;
  DtbExtnProtocol.FdtGetPhandleNode                 = (FDT_GET_PHANDLE_NODE)fdt_get_phandle_node;
  DtbExtnProtocol.FdtGetPropNamesOfNode             = (FDT_GET_PROP_NAMES_OF_NODE)fdt_get_prop_names_of_node;
  DtbExtnProtocol.FdtGetPropNamesSizeOfNode         = (FDT_GET_PROP_NAMES_SIZE_OF_NODE)fdt_get_prop_names_size_of_node;
  DtbExtnProtocol.FdtGetPropValuesOfNode            = (FDT_GET_PROP_VALUES_OF_NODE)fdt_get_prop_values_of_node;
  DtbExtnProtocol.FdtGetPropValuesSizeOfNode        = (FDT_GET_PROP_VALUES_SIZE_OF_NODE)fdt_get_prop_values_size_of_node;
  DtbExtnProtocol.FdtGetSizeOfSubnodeNames          = (FDT_GET_SIZE_OF_SUBNODE_NAMES)fdt_get_size_of_subnode_names;
  DtbExtnProtocol.FdtGetSubnodeNames                = (FDT_GET_SUBNODE_NAMES)fdt_get_subnode_names;
  DtbExtnProtocol.FdtNodeCmp                        = (FDT_NODE_CMP)fdt_node_cmp;
  DtbExtnProtocol.FdtNodeCopy                       = (FDT_NODE_COPY)fdt_node_copy;
  DtbExtnProtocol.FdtGetBooleanProp                 = (FDT_GET_BOOLEAN_PROP)fdt_get_boolean_prop;
  DtbExtnProtocol.FdtGetPropSize                    = (FDT_GET_PROP_SIZE)fdt_get_prop_size;
  DtbExtnProtocol.FdtGetStringPropList              = (FDT_GET_STRING_PROP_LIST)fdt_get_string_prop_list;
  DtbExtnProtocol.FdtGetUint32Prop                  = (FDT_GET_UINT32_PROP)fdt_get_uint32_prop;
  DtbExtnProtocol.FdtGetUint32PropList              = (FDT_GET_UINT32_PROP_LIST)fdt_get_uint32_prop_list;
  DtbExtnProtocol.FdtGetUint32PropListSlice         = (FDT_GET_UINT32_PROP_LIST_SLICE)fdt_get_uint32_prop_list_slice;
  DtbExtnProtocol.FdtGetUint64Prop                  = (FDT_GET_UINT64_PROP)fdt_get_uint64_prop;
  DtbExtnProtocol.FdtGetUint64PropList              = (FDT_GET_UINT64_PROP_LIST)fdt_get_uint64_prop_list;
  DtbExtnProtocol.FdtGetUint8Prop                   = (FDT_GET_UINT8_PROP)fdt_get_uint8_prop;
  DtbExtnProtocol.FdtGetUint8PropList               = (FDT_GET_UINT8_PROP_LIST)fdt_get_uint8_prop_list;
  DtbExtnProtocol.FdtGetUint8PropListSlice          = (FDT_GET_UINT8_PROP_LIST_SLICE)fdt_get_uint8_prop_list_slice;
  DtbExtnProtocol.FdtGetUint16Prop                  = (FDT_GET_UINT16_PROP)fdt_get_uint16_prop;
  DtbExtnProtocol.FdtGetUint16PropList              = (FDT_GET_UINT16_PROP_LIST)fdt_get_uint16_prop_list;
  DtbExtnProtocol.FdtGetUint16PropListSlice         = (FDT_GET_UINT16_PROP_LIST_SLICE)fdt_get_uint16_prop_list_slice;
  DtbExtnProtocol.FdtGetCacheOfSubnodes             = (FDT_GET_CACHE_OF_SUBNODES)fdt_get_cache_of_subnodes;
  DtbExtnProtocol.FdtGetUint64PropListSlice         = (FDT_GET_UINT64_PROP_LIST_SLICE)fdt_get_uint64_prop_list_slice;
  DtbExtnProtocol.FdtSetBlobHandle                  = (FDT_SET_BLOB_HANDLE)fdt_set_blob_handle;
  DtbExtnProtocol.FdtCheckValidBlob                 = (FDT_CHECK_VALID_BLOB)fdt_check_for_valid_blob;
  DtbExtnProtocol.FdtMergeOverlay                   = (FDT_MERGE_OVERLAY)fdt_merge_overlay;
  DtbExtnProtocol.FdtGetBlobHandle                  = (FDT_GET_BLOB_HANDLE)fdt_get_blob_handle;
  DtbExtnProtocol.FdtGetBlobSize                    = (FDT_GET_BLOB_SIZE)fdt_get_blob_size;
  DtbExtnProtocol.FdtInitRootHandleForDriverById    = (FDT_INIT_ROOT_HANDLE_FOR_DRIVER_BY_ID)fdt_init_root_handle_for_driver_by_id;
  DtbExtnProtocol.FdtGetNodeHandleByBlob            = (FDT_GET_NODE_HANDLE_BY_BLOB)fdt_get_node_handle;
  DtbExtnProtocol.FdtInitRootHandleForDriverByBlob  = (FDT_INIT_ROOT_HANDLE_FOR_DRIVER_BY_BLOB)fdt_init_root_handle_for_driver;
  DtbExtnProtocol.FdtGetBoolProp                    = (FDT_GET_BOOL_PROP)fdt_get_bool_prop;

  //
  // The HOB data is a pointer to the protocol structure.  DtbExtnDxe reads
  // the HOB data as a UINTN * and dereferences it to get the protocol pointer.
  //
  pDtbExtnProtocol = &DtbExtnProtocol;
  BuildGuidDataHob (&gEfiDtbExtnIntfHobGuid, &pDtbExtnProtocol, sizeof (pDtbExtnProtocol));

  return EFI_SUCCESS;
}
