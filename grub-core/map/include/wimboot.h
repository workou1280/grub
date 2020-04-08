 /*
 *  GRUB  --  GRand Unified Bootloader
 *  Copyright (C) 2019  Free Software Foundation, Inc.
 *
 *  GRUB is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  GRUB is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with GRUB.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#ifndef WIMBOOT_PRIVATE_DATA_H
#define WIMBOOT_PRIVATE_DATA_H

/** Base segment address
 *
 * We place everything at 2000:0000, since this region is used by the
 * Microsoft first-stage loaders (e.g. pxeboot.n12, etfsboot.com).
 */
#define BASE_SEG 0x2000

/** Base linear address */
#define BASE_ADDRESS ( BASE_SEG << 4 )

/** 64 bit long mode code segment */
#define LM_CS 0x10

/** 32 bit protected mode flat code segment */
#define FLAT_CS 0x20

/** 32 bit protected mode flat data segment */
#define FLAT_DS 0x30

/** 16 bit real mode code segment */
#define REAL_CS 0x50

/** 16 bit real mode data segment */
#define REAL_DS 0x60

#ifndef ASSEMBLY

#ifdef GRUB_MACHINE_EFI
#include <grub/efi/api.h>
#include <grub/efi/efi.h>
#include <efiapi.h>
#include <private.h>
#endif

#include <grub/misc.h>
#include <grub/disk.h>
#include <grub/file.h>
#include <maplib.h>
#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <vfat.h>

#define PAGE_SIZE 4096

#define MBR_TYPE_PCAT 0x01
#define SIGNATURE_TYPE_MBR 0x01

struct wimboot_cmdline
{
  grub_uint8_t gui;
  grub_uint8_t rawbcd;
  grub_uint8_t rawwim;
  grub_uint32_t index;
  grub_uint8_t pause;
  wchar_t inject[256];
};

struct grub_vfatdisk_file
{
  const char *name;
  grub_file_t file;
  void *addr;
  struct grub_vfatdisk_file *next;
};
extern struct grub_vfatdisk_file *vfat_file_list;

extern struct wimboot_cmdline wimboot_cmd;
extern struct vfat_file *bootmgfw;
extern vdisk_t wimboot_disk, wimboot_part;
#ifdef GRUB_MACHINE_EFI
void wimboot_boot (struct vfat_file *file);
grub_efi_status_t wimboot_install (void);
#endif
void
disk_read_file (struct vfat_file *vfile, void *data, size_t offset, size_t len);
void
mem_read_file (struct vfat_file *file, void *data, size_t offset, size_t len);
int
add_file (const char *name, void *data, size_t len,
    void (* read) (struct vfat_file *file, void *data, size_t offset, size_t len));
void grub_extract (void);
void grub_wimboot_init (int argc, char *argv[]);
/* vfat */
void print_vfat_help (void);
void create_vfat (void);
void ls_vfat (void);
void print_hex (char *addr, grub_size_t offset, const char *prefix,
                grub_size_t len, int hex);
grub_size_t
replace_hex (char *addr, grub_size_t addr_len,
             const char *search, grub_size_t search_len,
             const char *replace, grub_size_t replace_len, int count);
void
patch_vfat_offset (const char *file, grub_size_t offset, const char *replace);
void
patch_vfat_search (const char *file, const char *search,
                   const char *replace, int count);
void
append_vfat_list (grub_file_t file, const char *file_name, void *addr, int mem);

#endif
#endif
