/* Copyright Statement:
 *
 * This software/firmware and related documentation ("MediaTek Software") are
 * protected under relevant copyright laws. The information contained herein
 * is confidential and proprietary to MediaTek Inc. and/or its licensors.
 * Without the prior written permission of MediaTek inc. and/or its licensors,
 * any reproduction, modification, use or disclosure of MediaTek Software,
 * and information contained herein, in whole or in part, shall be strictly prohibited.
 */
/* MediaTek Inc. (C) 2010. All rights reserved.
 *
 * BY OPENING THIS FILE, RECEIVER HEREBY UNEQUIVOCALLY ACKNOWLEDGES AND AGREES
 * THAT THE SOFTWARE/FIRMWARE AND ITS DOCUMENTATIONS ("MEDIATEK SOFTWARE")
 * RECEIVED FROM MEDIATEK AND/OR ITS REPRESENTATIVES ARE PROVIDED TO RECEIVER ON
 * AN "AS-IS" BASIS ONLY. MEDIATEK EXPRESSLY DISCLAIMS ANY AND ALL WARRANTIES,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE OR NONINFRINGEMENT.
 * NEITHER DOES MEDIATEK PROVIDE ANY WARRANTY WHATSOEVER WITH RESPECT TO THE
 * SOFTWARE OF ANY THIRD PARTY WHICH MAY BE USED BY, INCORPORATED IN, OR
 * SUPPLIED WITH THE MEDIATEK SOFTWARE, AND RECEIVER AGREES TO LOOK ONLY TO SUCH
 * THIRD PARTY FOR ANY WARRANTY CLAIM RELATING THERETO. RECEIVER EXPRESSLY ACKNOWLEDGES
 * THAT IT IS RECEIVER'S SOLE RESPONSIBILITY TO OBTAIN FROM ANY THIRD PARTY ALL PROPER LICENSES
 * CONTAINED IN MEDIATEK SOFTWARE. MEDIATEK SHALL ALSO NOT BE RESPONSIBLE FOR ANY MEDIATEK
 * SOFTWARE RELEASES MADE TO RECEIVER'S SPECIFICATION OR TO CONFORM TO A PARTICULAR
 * STANDARD OR OPEN FORUM. RECEIVER'S SOLE AND EXCLUSIVE REMEDY AND MEDIATEK'S ENTIRE AND
 * CUMULATIVE LIABILITY WITH RESPECT TO THE MEDIATEK SOFTWARE RELEASED HEREUNDER WILL BE,
 * AT MEDIATEK'S OPTION, TO REVISE OR REPLACE THE MEDIATEK SOFTWARE AT ISSUE,
 * OR REFUND ANY SOFTWARE LICENSE FEES OR SERVICE CHARGE PAID BY RECEIVER TO
 * MEDIATEK FOR SUCH MEDIATEK SOFTWARE AT ISSUE.
 *
 * The following software/firmware and/or related documentation ("MediaTek Software")
 * have been modified by MediaTek Inc. All revisions are subject to any receiver's
 * applicable license agreements with MediaTek Inc.
 */

#include <linux/mtd/mtd.h>
#include <linux/mtd/nand.h>
#include <linux/mtd/partitions.h>

#define RECONFIG_PARTITION_SIZE 1

#define MTD_BOOT_PART_SIZE  0x80000
#define MTD_CONFIG_PART_SIZE    0x20000
#define MTD_FACTORY_PART_SIZE   0x20000

extern unsigned int  CFG_BLOCKSIZE;
#define LARGE_MTD_BOOT_PART_SIZE       (CFG_BLOCKSIZE<<2)
#define LARGE_MTD_CONFIG_PART_SIZE     (CFG_BLOCKSIZE<<2)
#define LARGE_MTD_FACTORY_PART_SIZE    (CFG_BLOCKSIZE<<1)

#include "../maps/ralink-flash.h"

/*=======================================================================*/
/* NAND PARTITION Mapping                                                  */
/*=======================================================================*/
//#ifdef CONFIG_MTD_PARTITIONS
static struct mtd_partition g_pasStatic_Partition[] = {
         {
                 name:           "Whole", /* mtdblock0 */
                 size:           FLASHMAP_WHOLE_SIZE,
                 offset:         FLASHMAP_WHOLE_OFFSET,
         }
         , {
                 name:           "Bootloader",  /* mtdblock1 */
                 size:           FLASHMAP_BOOT_SIZE,  /* 64K */
                 offset:         FLASHMAP_BOOT_OFFSET,
         }
         , {
                 name:           "Kernel1", /* mtdblock2 */
                 size:           FLASHMAP_KERNEL_SIZE,
                 offset:         FLASHMAP_KERNEL_OFFSET,
         }
         , {
                 name:           "RootFS1", /* mtdblock3 */
                 size:           FLASHMAP_ROOTFS_SIZE,
                 offset:         FLASHMAP_ROOTFS_OFFSET,
         }
         , {
                 name:           "Kernel2", /* mtdblock4 */
                 size:           FLASHMAP_KERNEL2_SIZE,
                 offset:         FLASHMAP_KERNEL2_OFFSET,
         }
         , {
                 name:           "RootFS2", /* mtdblock5 */
                 size:           FLASHMAP_ROOTFS2_SIZE,
                 offset:         FLASHMAP_ROOTFS2_OFFSET,
         }
         , {
                 name:           "USER", /* mtdblock6 */
                 size:           FLASHMAP_STORAGE_SIZE,
                 offset:         FLASHMAP_STORAGE_OFFSET,
         }
         , {
                 name:           "CSID", /* mtdblock7 */
                 size:           FLASHMAP_CONFIG_SIZE,
                 offset:         FLASHMAP_CONFIG_OFFSET,
         }
	, {
                 name:           "ODM",
                 size:           FLASHMAP_ODM_SIZE,
                 offset:         FLASHMAP_ODM_OFFSET,
        }
};
#define NUM_PARTITIONS ARRAY_SIZE(g_pasStatic_Partition)
extern int part_num;	// = NUM_PARTITIONS;
//#endif
#undef RECONFIG_PARTITION_SIZE

