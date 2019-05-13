/*
 * Copyright 2017, Data61
 * Commonwealth Scientific and Industrial Research Organisation (CSIRO)
 * ABN 41 687 119 230.
 *
 * This software may be distributed and modified according to the terms of
 * the GNU General Public License version 2. Note that NO WARRANTY is provided.
 * See "LICENSE_GPLv2.txt" for details.
 *
 * @TAG(DATA61_GPL)
 */

#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <autoconf.h>

#include <sel4platsupport/arch/io.h>
#include <sel4utils/vspace.h>
#include <sel4utils/iommu_dma.h>
#include <simple/simple_helpers.h>
#include <vka/capops.h>
#include <utils/util.h>

#include <camkes.h>
#include <camkes/dataport.h>

#include <ethdrivers/virtio/virtio_pci.h>
#include <ethdrivers/virtio/virtio_net.h>
#include <ethdrivers/virtio/virtio_ring.h>

#include <sel4vm/guest_vm.h>
#include <sel4vm/guest_memory.h>

#include "sel4vm/vmm.h"
#include "sel4vm/driver/pci_helper.h"
#include "sel4vm/driver/virtio_emul.h"
#include "sel4vm/platform/ioports.h"

#include "vm.h"
#include "i8259.h"
#include "virtio_net.h"

#define VIRTIO_VID 0x1af4
#define VIRTIO_DID_START 0x1000

#define QUEUE_SIZE 128

volatile Buf*__attribute__((weak)) ethdriver_buf;

int __attribute__((weak)) ethdriver_tx(int len) {
    ZF_LOGF("should not be here");
    return 0;
}

int __attribute__((weak)) ethdriver_rx(int *len) {
    ZF_LOGF("should not be here");
    return 0;
}

void __attribute__((weak)) ethdriver_mac(uint8_t *b1, uint8_t *b2, uint8_t *b3, uint8_t *b4, uint8_t *b5, uint8_t *b6) {
    ZF_LOGF("should not be here");
}

int __attribute__((weak)) eth_rx_ready_reg_callback(void (*proc)(void*),void *blah) {
    ZF_LOGF("should not be here");
    return 0;
}


static virtio_net_t *virtio_net = NULL;


static int emul_raw_tx(struct eth_driver *driver, unsigned int num, uintptr_t *phys, unsigned int *len, void *cookie) {
    size_t tot_len = 0;
    char *p = (char*)ethdriver_buf;
    /* copy to the data port */
    for (int i = 0; i < num; i++) {
        memcpy(p + tot_len, (void*)phys[i], len[i]);
        tot_len += len[i];
    }
    ethdriver_tx(tot_len);
    return ETHIF_TX_COMPLETE;
}


static void emul_low_level_init(struct eth_driver *driver, uint8_t *mac, int *mtu) {
    ethdriver_mac(&mac[0], &mac[1], &mac[2], &mac[3], &mac[4], &mac[5]);
    *mtu = 1500;
}


void virtio_net_notify(vm_t *vm) {
    int len;
    int status;
    status = ethdriver_rx(&len);
    while (status != -1) {
        void *cookie;
        void *emul_buf = (void*)virtio_net->emul_driver->i_cb.allocate_rx_buf(virtio_net->emul_driver->cb_cookie, len, &cookie);
        if (emul_buf) {
            memcpy(emul_buf, (void*)ethdriver_buf, len);
            virtio_net->emul_driver->i_cb.rx_complete(virtio_net->emul_driver->cb_cookie, 1, &cookie, (unsigned int*)&len);
        }
        if (status == 1) {
            status = ethdriver_rx(&len);
        } else {
            /* if status is 0 we already saw the last packet */
            assert(status == 0);
            status = -1;
        }
    }
}

void make_virtio_net(vm_t *vm) {
    /* drain any existing packets */
    struct raw_iface_funcs backend = virtio_net_default_backend();
    backend.raw_tx = emul_raw_tx;
    backend.low_level_init = emul_low_level_init;
    virtio_net = common_make_virtio_net(vm, 0x9000, backend);
    assert(virtio_net);
    int len;
    while (ethdriver_rx(&len) != -1);
}
