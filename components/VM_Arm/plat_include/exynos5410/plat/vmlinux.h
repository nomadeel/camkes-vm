/*
 * Copyright 2017, Data61
 * Commonwealth Scientific and Industrial Research Organisation (CSIRO)
 * ABN 41 687 119 230.
 *
 * This software may be distributed and modified according to the terms of
 * the BSD 2-Clause license. Note that NO WARRANTY is provided.
 * See "LICENSE_BSD2.txt" for details.
 *
 * @TAG(DATA61_BSD)
 */
#pragma once

#define VUSB_ADDRESS         0x33330000
#define VUSB_IRQ             198
#define VUSB_NINDEX          5
#define VUSB_NBADGE          0x123
#define IRQ_SPI_OFFSET 32
#define GIC_IRQ_PHANDLE 0x1

#define LINUX_RAM_BASE    0x40000000
#define LINUX_RAM_PADDR_BASE LINUX_RAM_BASE
#define LINUX_RAM_SIZE    0x40000000
#define PLAT_RAM_END      0xc0000000
#define LINUX_RAM_OFFSET  0
#define DTB_ADDR          (LINUX_RAM_BASE + 0x0F000000)
#define INITRD_MAX_SIZE   0x1900000 //25 MB
#define INITRD_ADDR       (DTB_ADDR - INITRD_MAX_SIZE) //0x4D700000

static const int linux_pt_irqs[] = {
};

static const int free_plat_interrupts[] =  { 92 + IRQ_SPI_OFFSET,
                                             93 + IRQ_SPI_OFFSET,
                                             101 + IRQ_SPI_OFFSET,
                                             102 + IRQ_SPI_OFFSET
                                           };

void vusb_notify(void);

static const char *plat_keep_devices[] = {
    "/soc/chipid@10000000",
    "/soc/interrupt-controller@10481000",
    "/soc/interrupt-controller@10440000",
    "/soc/usb@12110000",
    "/soc/phy@12130000",
    "/soc/serial@12c00000",
    "/soc/serial@12c10000",
    "/soc/serial@12c20000",
    "/xxti",
    "/cpus/cpu@0",
    "/soc/clock-controller@10010000",
    "/soc/syscon@10050000",
    "/soc/memory-controller@12250000",
};
static const char *plat_keep_device_and_disable[] = {};
static const char *plat_keep_device_and_subtree[] = {
    "/soc/mmc@12200000",
    "/soc/mmc@12220000",
    "/soc/system-controller@10040000",
    "/soc/sysram@2020000",

};
static const char *plat_keep_device_and_subtree_and_disable[] = {};
