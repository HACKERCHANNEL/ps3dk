libdir = @PS3SDK_LIBDIR@

CC = @PPU_CC@
CCAS = $(CC) -mregnames
AR = @PPU_AR@
RANLIB = @PPU_RANLIB@

HV_OBJS = allocate_memory.o write_htab_entry.o			      \
    construct_virtual_address_space.o invalidate_htab_entries.o	      \
    get_virtual_address_space_id_of_pu.o			      \
    query_logical_partition_address_region_info.o		      \
    select_virtual_address_space.o pause.o			      \
    destruct_virtual_address_space.o configure_irq_state_bitmap.o     \
    connect_irq_plug.o release_memory.o put_iopte.o		      \
    disconnect_irq_plug.o construct_event_receive_port.o	      \
    destruct_event_receive_port.o send_event_locally.o		      \
    detect_pending_interrupts.o end_of_interrupt.o		      \
    construct_and_connect_irq_plug.o destruct_irq_plug.o	      \
    end_of_interrupt_ext.o did_update_interrupt_mask.o		      \
    shutdown_logical_partition.o destruct_logical_spu.o		      \
    construct_logical_spu.o set_spu_interrupt_mask.o		      \
    set_spu_transition_notifier.o disable_logical_spu.o		      \
    clear_spu_interrupt_status.o get_spu_interrupt_status.o	      \
    get_logical_pu_id.o set_interrupt_mask.o			      \
    get_logical_partition_id.o configure_execution_time_variable.o    \
    get_spu_irq_outlet.o set_spu_privilege_state_area_1_register.o    \
    create_repository_node.o get_repository_node_value.o	      \
    modify_repository_node_value.o remove_repository_node.o	      \
    read_htab_entries.o set_dabr.o set_vmx_graphics_mode.o	      \
    set_thread_switch_control_register.o get_total_execution_time.o   \
    allocate_io_segment.o release_io_segment.o allocate_ioid.o	      \
    release_ioid.o construct_io_irq_outlet.o destruct_io_irq_outlet.o \
    map_htab.o unmap_htab.o get_version_info.o insert_htab_entry.o    \
    read_virtual_uart.o write_virtual_uart.o set_virtual_uart_param.o \
    get_virtual_uart_param.o configure_virtual_uart_irq.o	      \
    open_device.o close_device.o map_device_mmio_region.o	      \
    unmap_device_mmio_region.o allocate_device_dma_region.o	      \
    free_device_dma_region.o map_device_dma_region.o		      \
    unmap_device_dma_region.o read_pci_config.o write_pci_config.o    \
    read_pci_io.o write_pci_io.o net_add_multicast_address.o	      \
    net_remove_multicast_address.o net_start_tx_dma.o		      \
    net_stop_tx_dma.o net_start_rx_dma.o net_stop_rx_dma.o	      \
    net_set_interrupt_status_indicator.o net_set_interrupt_mask.o     \
    net_control.o connect_interrupt_event_receive_port.o	      \
    disconnect_interrupt_event_receive_port.o			      \
    get_spu_all_interrupt_statuses.o deconfigure_virtual_uart_irq.o   \
    enable_logical_spu.o gpu_open.o gpu_close.o gpu_device_map.o      \
    gpu_device_unmap.o gpu_memory_allocate.o gpu_memory_free.o	      \
    gpu_context_allocate.o gpu_context_free.o gpu_context_iomap.o     \
    gpu_context_attribute.o gpu_context_intr.o gpu_attribute.o	      \
    get_rtc.o set_ppu_periodic_tracer_frequency.o		      \
    start_ppu_periodic_tracer.o stop_ppu_periodic_tracer.o	      \
    storage_read.o storage_write.o storage_send_device_command.o      \
    storage_get_async_status.o storage_check_async_status.o panic.o   \
    construct_lpm.o destruct_lpm.o start_lpm.o stop_lpm.o	      \
    copy_lpm_trace_buffer.o add_lpm_event_bookmark.o		      \
    delete_lpm_event_bookmark.o set_lpm_interrupt_mask.o	      \
    get_lpm_interrupt_status.o set_lpm_general_control.o	      \
    set_lpm_interval.o set_lpm_trigger_control.o		      \
    set_lpm_counter_control.o set_lpm_group_control.o		      \
    set_lpm_debug_bus_control.o set_lpm_counter.o set_lpm_signal.o    \
    set_lpm_spr_trigger.o


lib_LIBRARIES = libps3hv.a

libps3hv_a_SOURCES = hv.S
libps3hv_a_LIBADD = $(HV_OBJS)

libps3hvincludedir = @PS3SDK_INCLUDEDIR@
libps3hvinclude_HEADERS = ps3hv.h

noinst_HEADERS = lv1callarg.h lv1calltab.h

$(HV_OBJS): hv.S
	$(CPPASCOMPILE) -DF_$* $< -c -o $@

