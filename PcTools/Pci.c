/*++ 

Copyright (c) 2008 - 2009, Solar Studio                                                         
         
Module Name:

  Pci.c

Abstract:

  This is an integrated system utility for IBM compatible PC.

--*/

extern "C" {
  #include "Efi.h"
  #include "EfiDriverLib.h"
  #include "Solar.h"
  #include "SolarCppLib.h"      // Library : SolarCppLib (Stardart C++ module implement)
  #include "SolarConsoleLib.h"	// Library : SolarConsoleLib
}
#include "PcTools.h"
#include "Pci.h"
#include "PciClass.h"
#ifdef __DEBUG__
  #include "../_DevelopAddition/_Debug.h"
#endif

EFI_PCI_ROOT_BRIDGE_IO_PROTOCOL *PciIo;
UINT64                          *PciAddrList;
UINT8                           PciAddrListAmt;

SOLAR_STATUS
SOLAR_API
PciGetNextBusRange (
  IN OUT EFI_ACPI_ADDRESS_SPACE_DESCRIPTOR **Descriptors,
  OUT    UINT16                            *MinBus, 
  OUT    UINT16                            *MaxBus, 
  OUT    BOOLEAN                           *IsEnd
  )        
{    
  *IsEnd = FALSE;

  if ((*Descriptors) == NULL) {
    *MinBus = 0;
    *MaxBus = PCI_MAX_BUS;
    return SOLAR_SUCCESS;
  }

  if ((*Descriptors)->Desc == ACPI_END_TAG_DESCRIPTOR) {
    *IsEnd = TRUE;
  }
  
  while ((*Descriptors)->Desc != ACPI_END_TAG_DESCRIPTOR) {
    if ((*Descriptors)->ResType == ACPI_ADDRESS_SPACE_TYPE_BUS) {
      *MinBus = (UINT16)(*Descriptors)->AddrRangeMin;
      *MaxBus = (UINT16)(*Descriptors)->AddrRangeMax;
    }

    (*Descriptors) ++;
  }

  return SOLAR_SUCCESS;
}

SOLAR_STATUS
SOLAR_API
PciInitiallize (void)
{
  SOLAR_STATUS                      Status = SOLAR_SUCCESS;
  UINT64                            PciAddress;
  PCI_CONFIG_SPACE                  PciConfigSpace;
  EFI_ACPI_ADDRESS_SPACE_DESCRIPTOR *Descriptors;
  UINT16                            Bus;
  UINT16                            Device;
  UINT16                            Func;
  UINT16                            MinBus;
  UINT16                            MaxBus;
  BOOLEAN                           IsEnd;
  UINT8                             count = 0;
  INTN                              i;

  Status = gBS->LocateProtocol (
    &gEfiPciRootBridgeIoProtocolGuid,
    NULL,
    (void **) &PciIo
    );

  if (EFI_ERROR(Status)) {
    wprintf (L"\n>> Locate PciRootBridgeIoProtocol is fail.");
    return Status;
  }

  Status = gBS->AllocatePool (
    EfiReservedMemoryType,
    0x100 * sizeof(UINT64),
    (void **) &PciAddrList
    );

  if (EFI_ERROR(Status)) {
    wprintf (L"\n>> Memory allocation is fail.");
    return Status;
  }

  for (i = 0; i < 0x100; i ++) {
    *(PciAddrList + i) = 0xffffffffffffffff;
  }

  while (TRUE) {
    Status = PciGetNextBusRange (&Descriptors, &MinBus, &MaxBus, &IsEnd);
    if (EFI_ERROR(Status)) {
      wprintf (L"\n>> Can't get next PCI bus.");
      return Status;
    }
    if (IsEnd) {
      break;
    }

    for (Bus = MinBus; Bus <= MaxBus; Bus++) {
      for (Device = 0; Device <= PCI_MAX_DEVICE; Device++) {
        for (Func = 0; Func <= PCI_MAX_FUNC; Func++) {
          PciAddress = CALC_EFI_PCI_ADDRESS (Bus, Device, Func, 0);
          PciIo->Pci.Read (
            PciIo, 
            EfiPciWidthUint16, 
            PciAddress, 
            1, 
            &PciConfigSpace.Common.VendorId
            );

          if (PciConfigSpace.Common.VendorId == 0xffff && Func == 0) {
            break;
          }
          
          if (PciConfigSpace.Common.VendorId != 0xffff) {
            *(PciAddrList + count ++) = PciAddress;
            if (Func == 0 && ((PciConfigSpace.Common.HeaderType & HEADER_TYPE_MULTI_FUNCTION) == 0x00)) {
              break;
            }
          }
        }
      }
    }
    if (Descriptors == NULL) {
      break;
    }
  }

  PciAddrListAmt = count - 1;

  return Status;
}

/////////////////////////////////////////////////////////////////////////////
//
//  PciViewer () - PCI register viewer.
//
//  ARGUMENTS:
//    Segment               - PCI segment number.
//    Bus                   - PCI bus number.
//    Device                - PCI device number.
//    Function              - PCI function number.
//
//  RETURNS:
//    Status                - EFI status value.
//
/////////////////////////////////////////////////////////////////////////////
SOLAR_STATUS
SOLAR_API
PciViewer (
  UINT8         Segment,
  UINT8         Bus,
  UINT8         Device,
  UINT8         Function
  )
{
  SOLAR_STATUS      Status = SOLAR_SUCCESS;
  UINT64            PciAddress;
  PCI_CONFIG_SPACE  PciConfigSpace;
  UINT32            PciClassCode;
  PCI_CLASS_STRINGS PciClassStrings;
  UINT8             *PciReg;
  UINT8             PciRegVal;
  UINT16            i, j;

  // Get PCI Register Vaule.
  PciAddress = CALC_EFI_PCI_ADDRESS (Bus, Device, Function, 0x00);
  PciIo->Pci.Read (
    PciIo,
    EfiPciWidthUint8,
    PciAddress,
    sizeof(PCI_CONFIG_SPACE),
    &PciConfigSpace
    );

  PciClassCode = 0;
  PciClassCode |= PciConfigSpace.Common.ClassCode[0];
  PciClassCode |= (PciConfigSpace.Common.ClassCode[1] << 8);
  PciClassCode |= (PciConfigSpace.Common.ClassCode[2] << 16);

  // Set the main information line for this function.
  PAWPrintf (0, MAIN_FUNCTION_INFO_LINE_Y, EFI_LIGHTGRAY, EFI_RED, SPACE_LINE);
  PciGetClassStrings (PciClassCode, &PciClassStrings);
  PAWPrintf (1, MAIN_FUNCTION_INFO_LINE_Y, EFI_LIGHTGRAY, EFI_RED, L"%s: ", PciClassStrings.BaseClass);
  AWPrintf (EFI_WHITE, EFI_RED, L"%s", PciClassStrings.SubClass);
  PAWPrintf (45, MAIN_FUNCTION_INFO_LINE_Y, EFI_LIGHTGRAY, EFI_RED, L"SEG: XX  BUS: XX  DEV: XX  FUN: XX");
  PAWPrintf (50, MAIN_FUNCTION_INFO_LINE_Y, EFI_WHITE, EFI_RED, L"%02x", Segment);
  PAWPrintf (59, MAIN_FUNCTION_INFO_LINE_Y, EFI_WHITE, EFI_RED, L"%02x", Bus);
  PAWPrintf (68, MAIN_FUNCTION_INFO_LINE_Y, EFI_WHITE, EFI_RED, L"%02x", Device);
  PAWPrintf (77, MAIN_FUNCTION_INFO_LINE_Y, EFI_WHITE, EFI_RED, L"%02x", Function);

  // Display PCI viewer.
  for (i = 0; i < 18; i ++) {
    PAWPrintf (4, 4 + i, EFI_WHITE, EFI_BLUE, L"                                                   ");
  }
  for (i = 0; i < 18; i ++) {
    PAWPrintf (55, 5 + i, EFI_WHITE, EFI_BLACK, L" ");
  }
  PAWPrintf (5, 22, EFI_WHITE, EFI_BLACK, L"                                                   ");

  for (i = 0; i < 18; i ++) {
    PAWPrintf (58, 4 + i, EFI_WHITE, EFI_BLUE, L"                  ");
  }
  for (i = 0; i < 18; i ++) {
    PAWPrintf (76, 5 + i, EFI_WHITE, EFI_BLACK, L" ");
  }
  PAWPrintf (59, 22, EFI_WHITE, EFI_BLACK, L"                  ");

  // Display PCI register.
  PciReg = (UINT8 *) &PciConfigSpace;
  for (i = 0x00; i < 0x10; i ++) {
    for (j = 0x00; j < 0x10; j ++) {
      PciRegVal = PciReg[i * 0x10 + j];
      PAWPrintf (6 + j * 3, 5 + i, EFI_WHITE, EFI_BLUE, L"%02x", PciRegVal);
      PAWPrintf (59 + j, 5 + i, EFI_WHITE, EFI_BLUE, L"%c", (PciRegVal >= 0x20 && PciRegVal < 0x80) ? PciRegVal : L'.');
    }
  }

  return Status;
}

