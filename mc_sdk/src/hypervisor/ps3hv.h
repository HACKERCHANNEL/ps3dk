/*
 * PS3 Software Development Kit
 * -----------------------------------------------------------------------
 * Licensed under the BSD license, see LICENSE in PS3SDK root for details.
 *
 * ps3hv.h - Prototypes for the Hypervisor library
 *
 * Copyright (c) 2008 Marcus Comstedt <marcus@mc.pp.se>
 */
#ifndef __PS3HV_H__
#define __PS3HV_H__ 1

#include <ps3types.h>

#ifdef __cplusplus
extern "C" {
#endif

/*
 * #1: Write an entry to the hash page table.
 */
extern int64_t lv1_write_htab_entry(uint64_t vas_id, uint64_t slot,
				    uint64_t va, uint64_t pa);

/*
 * #2: Construct a PPE virtual address space.
 */
extern int64_t lv1_construct_virtual_address_space(uint64_t htab_size,
						   uint64_t number_of_sizes,
						   uint64_t page_sizes,
						   uint64_t *vas_id,
						   uint64_t *act_htab_size);

/*
 * #7: Select an alternative virtual address space.
 */
extern int64_t lv1_select_virtual_address_space(uint64_t vas_id);

/*
 * #10: Destruct a virtual address space.
 */
extern int64_t lv1_destruct_virtual_address_space(uint64_t vas_id);

/*
 * #11: Register the address of a HV plug-outlet bitmap with the Hypervisor.
 */
extern int64_t lv1_configure_irq_state_bitmap(uint64_t ppe_id, uint64_t cpu_id,
					      lpar_addr_t bmp_addr);

/*
 * #69: Returns the logical PPE id.
 */
extern int64_t lv1_get_logical_pu_id(uint64_t *ppe_id);

/*
 * #74: Get the id of this LPAR.
 */
extern int64_t lv1_get_logical_partition_id(uint64_t *lpar_id);

/*
 * #91: Get information from the HV repository.
 */
extern int64_t lv1_get_repository_node_value(uint64_t lpar_id, uint64_t l1key,
					     uint64_t l2key, uint64_t l3key,
					     uint64_t l4key, uint64_t *val1,
					     uint64_t *val2);

/*
 * #162: Read data from a VUART port into a provided buffer.
 */
extern int64_t lv1_read_virtual_uart(int64_t port_number, lpar_addr_t buffer,
				     uint64_t bytes, uint64_t *bytes_read);

/*
 * #163: Write a buffer of data to a VUART port.
 */
extern int64_t lv1_write_virtual_uart(int64_t port_number, lpar_addr_t buffer,
				      uint64_t bytes, uint64_t *bytes_written);

/*
 * #170: Open the device for a given bus and device id.
 */
extern int64_t lv1_open_device(uint64_t bus_id, uint64_t dev_id, uint64_t flags);

/*
 * #171: Close the device for a given bus and device id.
 */
extern int64_t lv1_close_device(uint64_t bus_id, uint64_t dev_id);

/*
 * #174: Allocate a DMA region for the specified device.
 */
extern int64_t lv1_allocate_device_dma_region(uint64_t bus_id, uint64_t dev_id,
					      uint64_t io_size, uint64_t io_pagesize,
					      uint64_t flag, dma_addr_t *dma_region);

/*
 * #176: Map a DMA region for the specified device.
 */
extern int64_t lv1_map_device_dma_region(uint64_t bus_id, uint64_t dev_id,
					 lpar_addr_t lpar_addr, dma_addr_t dma_region,
					 uint64_t size, uint64_t flags );

/*
 * #187: Start DMA transmit on the specified network device.
 */
extern int64_t lv1_net_start_tx_dma(uint64_t bus_id, uint64_t dev_id,
				    dma_addr_t bus_addr, uint64_t flags);

/*
 * #188: Stop DMA transmit on the specified network device.
 */
extern int64_t lv1_net_stop_tx_dma(uint64_t bus_id, uint64_t dev_id,
				   uint64_t flags);

/*
 * #189: Start DMA receive on the specified network device.
 */
extern int64_t lv1_net_start_rx_dma(uint64_t bus_id, uint64_t dev_id,
				    dma_addr_t bus_addr, uint64_t flags);

/*
 * #190: Stop DMA receive on the specified network device.
 */
extern int64_t lv1_net_stop_rx_dma(uint64_t bus_id, uint64_t dev_id,
				   uint64_t flags);

/*
 * #191: Set the interrupt status indicator for the specified network device.
 */
extern int64_t lv1_net_set_interrupt_status_indicator(uint64_t bus_id, uint64_t dev_id,
						      lpar_addr_t irq_status_addr, uint64_t flags);

/*
 * #193: Sets the interrupt mask for specified network device.
 */
extern int64_t lv1_net_set_interrupt_mask(uint64_t bus_id, uint64_t dev_id,
					  uint64_t mask, uint64_t flags);

/*
 * #194: Send a control command to the specified network device.
 */
extern int64_t lv1_net_control(uint64_t bus_id, uint64_t dev_id,
			       uint64_t p1, uint64_t p2,
			       uint64_t p3, uint64_t p4,
			       uint64_t *v1, uint64_t *v2);

/*
 * #210: Open the GPU.
 */
extern int64_t lv1_gpu_open(int64_t unknown);

/*
 * #211: Close the GPU
 */
extern int64_t lv1_gpu_close(void);

/*
 * #214: Allocate GPU memory.
 */
extern int64_t lv1_gpu_memory_allocate(uint64_t ddr_size, uint64_t unknown1,
				       uint64_t unknown2, uint64_t unknown3,
				       uint64_t unknown4,
				       gpu_memory_handle_t *memory_handle,
				       lpar_addr_t *ddr_lpar);

/*
 * #216: Free memory handle returned by lv1_gpu_memory_allocate.
 * Must be called to dispose of the handle returned by lv1_gpu_memory_allocate.
 */
extern int64_t lv1_gpu_memory_free(gpu_memory_handle_t memory_handle);

/*
 * #225: Perform an operation on a GPU context
 */
extern int64_t lv1_gpu_context_attribute(gpu_context_handle_t context_handle,
					 uint64_t opcode,
					 uint64_t param1, uint64_t param2,
					 uint64_t param3, uint64_t param4);


/*
 * #232: Gets the current value of the PS3's real time clock and
 *       time base value.
 */
extern int64_t lv1_get_rtc(uint64_t *rtc_val, uint64_t *tb_val);

/*
 * #255: Halt OtherOS
 */
extern void lv1_panic(uint64_t reboot_flag) __attribute__((noreturn));

#ifdef __cplusplus
}
#endif

#endif
