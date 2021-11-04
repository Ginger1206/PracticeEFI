/*++ 

Copyright (c) 2008 - 2009, Solar Studio                                                         
         
Module Name:

  PciClass.h

Abstract:

  This is an integrated system utility for IBM compatible PC.

--*/

#ifndef  _PCI_CLASS_H_
#define  _PCI_CLASS_H_

extern "C" {
  #include "efi.h"  // for UINT32 etc.
}

#define PCI_CLASS_STRING_LIMIT    54
//
// Printable strings for Pci class code
//
typedef struct {
  CHAR16* BaseClass;  // Pointer to the PCI base class string
  CHAR16* SubClass;   // Pointer to the PCI sub class string
  CHAR16* PIFClass;   // Pointer to the PCI programming interface string
} PCI_CLASS_STRINGS;

//
// a structure holding a single entry, which also points to its lower level
// class
//
typedef struct PCI_CLASS_ENTRY_TAG {
  UINT8                       Code;             // Class, subclass or I/F code
  CHAR16                      *DescText;        // Description string
  struct PCI_CLASS_ENTRY_TAG  *LowerLevelClass; // Subclass or I/F if any
} PCI_CLASS_ENTRY;

VOID
PciGetClassStrings (
  IN      UINT32             ClassCode,
  IN OUT  PCI_CLASS_STRINGS  *ClassStrings
  );
  
#endif // _PCI_CLASS_H_