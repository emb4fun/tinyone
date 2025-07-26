MEMORY
{
  UNPLACED_SECTIONS (wx) : ORIGIN = 0x100000000, LENGTH = 0
  SDRAM (wx) : ORIGIN = 0x80000000, LENGTH = 0x20000000
  BOOT_RAM (wx) : ORIGIN = 0x402f0400, LENGTH = 0x0000fc00
}

SECTIONS
{
  __SDRAM_segment_start__ = 0x80000000;
  __SDRAM_segment_end__ = 0xa0000000;
  __SDRAM_segment_size__ = 0x20000000;
  __BOOT_RAM_segment_start__ = 0x402f0400;
  __BOOT_RAM_segment_end__ = 0x40300000;
  __BOOT_RAM_segment_size__ = 0x0000fc00;

  __HEAPSIZE__ = 1024;
  __SEGGER_RTL_vfprintf = __SEGGER_RTL_vfprintf_short_float_long;
  __SEGGER_RTL_vfscanf = __SEGGER_RTL_vfscanf_int;
  __STACKSIZE_ABT__ = 512;
  __STACKSIZE_FIQ__ = 512;
  __STACKSIZE_IRQ__ = 512;
  __STACKSIZE_SVC__ = 512;
  __STACKSIZE_UND__ = 512;
  __STACKSIZE__ = 1024;

  __vectors_load_start__ = __BOOT_RAM_segment_start__;
  .vectors __BOOT_RAM_segment_start__ : AT(__BOOT_RAM_segment_start__)
  {
    __vectors_start__ = .;
    *(.vectors .vectors.*)
  }
  __vectors_end__ = __vectors_start__ + SIZEOF(.vectors);
  __vectors_size__ = SIZEOF(.vectors);
  __vectors_load_end__ = __vectors_end__;

  . = ASSERT(__vectors_start__ == __vectors_end__ || (__vectors_end__ - __BOOT_RAM_segment_start__) <= __BOOT_RAM_segment_size__ , "error: .vectors is too large to fit in BOOT_RAM memory segment");

  __fast_load_start__ = ALIGN(__vectors_end__ , 4);
  .fast ALIGN(__vectors_end__ , 4) : AT(ALIGN(__vectors_end__ , 4))
  {
    __fast_start__ = .;
    *(.fast .fast.*)
  }
  __fast_end__ = __fast_start__ + SIZEOF(.fast);
  __fast_size__ = SIZEOF(.fast);
  __fast_load_end__ = __fast_end__;

  . = ASSERT(__fast_start__ == __fast_end__ || (__fast_end__ - __BOOT_RAM_segment_start__) <= __BOOT_RAM_segment_size__ , "error: .fast is too large to fit in BOOT_RAM memory segment");

  __init_load_start__ = ALIGN(__fast_end__ , 4);
  .init ALIGN(__fast_end__ , 4) : AT(ALIGN(__fast_end__ , 4))
  {
    __init_start__ = .;
    *(.init .init.*)
  }
  __init_end__ = __init_start__ + SIZEOF(.init);
  __init_size__ = SIZEOF(.init);
  __init_load_end__ = __init_end__;

  . = ASSERT(__init_start__ == __init_end__ || (__init_end__ - __BOOT_RAM_segment_start__) <= __BOOT_RAM_segment_size__ , "error: .init is too large to fit in BOOT_RAM memory segment");

  __init_rodata_load_start__ = ALIGN(__init_end__ , 4);
  .init_rodata ALIGN(__init_end__ , 4) : AT(ALIGN(__init_end__ , 4))
  {
    __init_rodata_start__ = .;
    *(.init_rodata .init_rodata.*)
  }
  __init_rodata_end__ = __init_rodata_start__ + SIZEOF(.init_rodata);
  __init_rodata_size__ = SIZEOF(.init_rodata);
  __init_rodata_load_end__ = __init_rodata_end__;

  . = ASSERT(__init_rodata_start__ == __init_rodata_end__ || (__init_rodata_end__ - __BOOT_RAM_segment_start__) <= __BOOT_RAM_segment_size__ , "error: .init_rodata is too large to fit in BOOT_RAM memory segment");

  __text_load_start__ = ALIGN(__init_rodata_end__ , 4);
  .text ALIGN(__init_rodata_end__ , 4) : AT(ALIGN(__init_rodata_end__ , 4))
  {
    __text_start__ = .;
    *(.text .text.* .gnu.linkonce.t.* .glue_7t .glue_7 .gcc_except_table .ARM.extab* .gnu.linkonce.armextab.*)
  }
  __text_end__ = __text_start__ + SIZEOF(.text);
  __text_size__ = SIZEOF(.text);
  __text_load_end__ = __text_end__;

  . = ASSERT(__text_start__ == __text_end__ || (__text_end__ - __BOOT_RAM_segment_start__) <= __BOOT_RAM_segment_size__ , "error: .text is too large to fit in BOOT_RAM memory segment");

  __dtors_load_start__ = ALIGN(__text_end__ , 4);
  .dtors ALIGN(__text_end__ , 4) : AT(ALIGN(__text_end__ , 4))
  {
    __dtors_start__ = .;
    KEEP (*(SORT(.dtors.*))) KEEP (*(.dtors)) KEEP (*(.fini_array)) KEEP (*(SORT(.fini_array.*)))
  }
  __dtors_end__ = __dtors_start__ + SIZEOF(.dtors);
  __dtors_size__ = SIZEOF(.dtors);
  __dtors_load_end__ = __dtors_end__;

  . = ASSERT(__dtors_start__ == __dtors_end__ || (__dtors_end__ - __BOOT_RAM_segment_start__) <= __BOOT_RAM_segment_size__ , "error: .dtors is too large to fit in BOOT_RAM memory segment");

  __ctors_load_start__ = ALIGN(__dtors_end__ , 4);
  .ctors ALIGN(__dtors_end__ , 4) : AT(ALIGN(__dtors_end__ , 4))
  {
    __ctors_start__ = .;
    KEEP (*(SORT(.ctors.*))) KEEP (*(.ctors)) KEEP (*(.init_array)) KEEP (*(SORT(.init_array.*)))
  }
  __ctors_end__ = __ctors_start__ + SIZEOF(.ctors);
  __ctors_size__ = SIZEOF(.ctors);
  __ctors_load_end__ = __ctors_end__;

  . = ASSERT(__ctors_start__ == __ctors_end__ || (__ctors_end__ - __BOOT_RAM_segment_start__) <= __BOOT_RAM_segment_size__ , "error: .ctors is too large to fit in BOOT_RAM memory segment");

  __data_load_start__ = ALIGN(__ctors_end__ , 4);
  .data ALIGN(__ctors_end__ , 4) : AT(ALIGN(__ctors_end__ , 4))
  {
    __data_start__ = .;
    *(.data .data.* .gnu.linkonce.d.*)
  }
  __data_end__ = __data_start__ + SIZEOF(.data);
  __data_size__ = SIZEOF(.data);
  __data_load_end__ = __data_end__;

  . = ASSERT(__data_start__ == __data_end__ || (__data_end__ - __BOOT_RAM_segment_start__) <= __BOOT_RAM_segment_size__ , "error: .data is too large to fit in BOOT_RAM memory segment");

  __rodata_load_start__ = ALIGN(__data_end__ , 4);
  .rodata ALIGN(__data_end__ , 4) : AT(ALIGN(__data_end__ , 4))
  {
    __rodata_start__ = .;
    *(.rodata .rodata.* .gnu.linkonce.r.*)
  }
  __rodata_end__ = __rodata_start__ + SIZEOF(.rodata);
  __rodata_size__ = SIZEOF(.rodata);
  __rodata_load_end__ = __rodata_end__;

  . = ASSERT(__rodata_start__ == __rodata_end__ || (__rodata_end__ - __BOOT_RAM_segment_start__) <= __BOOT_RAM_segment_size__ , "error: .rodata is too large to fit in BOOT_RAM memory segment");

  __ARM.exidx_load_start__ = ALIGN(__rodata_end__ , 4);
  .ARM.exidx ALIGN(__rodata_end__ , 4) : AT(ALIGN(__rodata_end__ , 4))
  {
    __ARM.exidx_start__ = .;
    __exidx_start =   __ARM.exidx_start__;
    *(.ARM.exidx .ARM.exidx.*)
  }
  __ARM.exidx_end__ = __ARM.exidx_start__ + SIZEOF(.ARM.exidx);
  __ARM.exidx_size__ = SIZEOF(.ARM.exidx);
  __exidx_end =   __ARM.exidx_end__;
  __ARM.exidx_load_end__ = __ARM.exidx_end__;

  . = ASSERT(__ARM.exidx_start__ == __ARM.exidx_end__ || (__ARM.exidx_end__ - __BOOT_RAM_segment_start__) <= __BOOT_RAM_segment_size__ , "error: .ARM.exidx is too large to fit in BOOT_RAM memory segment");

  __bss_load_start__ = ALIGN(__ARM.exidx_end__ , 4);
  .bss ALIGN(__ARM.exidx_end__ , 4) (NOLOAD) : AT(ALIGN(__ARM.exidx_end__ , 4))
  {
    __bss_start__ = .;
    *(.bss .bss.* .gnu.linkonce.b.*) *(COMMON)
  }
  __bss_end__ = __bss_start__ + SIZEOF(.bss);
  __bss_size__ = SIZEOF(.bss);
  __bss_load_end__ = __bss_end__;

  . = ASSERT(__bss_start__ == __bss_end__ || (__bss_end__ - __BOOT_RAM_segment_start__) <= __BOOT_RAM_segment_size__ , "error: .bss is too large to fit in BOOT_RAM memory segment");

  __non_init_load_start__ = ALIGN(__bss_end__ , 4);
  .non_init ALIGN(__bss_end__ , 4) (NOLOAD) : AT(ALIGN(__bss_end__ , 4))
  {
    __non_init_start__ = .;
    *(.non_init .non_init.*)
  }
  __non_init_end__ = __non_init_start__ + SIZEOF(.non_init);
  __non_init_size__ = SIZEOF(.non_init);
  __non_init_load_end__ = __non_init_end__;

  . = ASSERT(__non_init_start__ == __non_init_end__ || (__non_init_end__ - __BOOT_RAM_segment_start__) <= __BOOT_RAM_segment_size__ , "error: .non_init is too large to fit in BOOT_RAM memory segment");

  __heap_load_start__ = ALIGN(__non_init_end__ , 4);
  .heap ALIGN(__non_init_end__ , 4) (NOLOAD) : AT(ALIGN(__non_init_end__ , 4))
  {
    __heap_start__ = .;
    *(.heap .heap.*)
    . = ALIGN(MAX(__heap_start__ + __HEAPSIZE__ , .), 4);
  }
  __heap_end__ = __heap_start__ + SIZEOF(.heap);
  __heap_size__ = SIZEOF(.heap);
  __heap_load_end__ = __heap_end__;

  . = ASSERT(__heap_start__ == __heap_end__ || (__heap_end__ - __BOOT_RAM_segment_start__) <= __BOOT_RAM_segment_size__ , "error: .heap is too large to fit in BOOT_RAM memory segment");

  . = ASSERT(__HEAPSIZE__  == ALIGN(__HEAPSIZE__, 4), "error: .heap section size does not match alignment");

  . = ASSERT(__heap_size__ <= __HEAPSIZE__ , "error: .heap section is larger than specified size");

  __stack_load_start__ = ALIGN(__heap_end__ , 8);
  .stack ALIGN(__heap_end__ , 8) (NOLOAD) : AT(ALIGN(__heap_end__ , 8))
  {
    __stack_start__ = .;
    *(.stack .stack.*)
    . = ALIGN(MAX(__stack_start__ + __STACKSIZE__ , .), 8);
  }
  __stack_end__ = __stack_start__ + SIZEOF(.stack);
  __stack_size__ = SIZEOF(.stack);
  __stack_load_end__ = __stack_end__;

  . = ASSERT(__stack_start__ == __stack_end__ || (__stack_end__ - __BOOT_RAM_segment_start__) <= __BOOT_RAM_segment_size__ , "error: .stack is too large to fit in BOOT_RAM memory segment");

  . = ASSERT(__STACKSIZE__  == ALIGN(__STACKSIZE__, 8), "error: .stack section size does not match alignment");

  . = ASSERT(__stack_size__ <= __STACKSIZE__ , "error: .stack section is larger than specified size");

  __stack_irq_load_start__ = ALIGN(__stack_end__ , 8);
  .stack_irq ALIGN(__stack_end__ , 8) (NOLOAD) : AT(ALIGN(__stack_end__ , 8))
  {
    __stack_irq_start__ = .;
    *(.stack_irq .stack_irq.*)
    . = ALIGN(MAX(__stack_irq_start__ + __STACKSIZE_IRQ__ , .), 8);
  }
  __stack_irq_end__ = __stack_irq_start__ + SIZEOF(.stack_irq);
  __stack_irq_size__ = SIZEOF(.stack_irq);
  __stack_irq_load_end__ = __stack_irq_end__;

  . = ASSERT(__stack_irq_start__ == __stack_irq_end__ || (__stack_irq_end__ - __BOOT_RAM_segment_start__) <= __BOOT_RAM_segment_size__ , "error: .stack_irq is too large to fit in BOOT_RAM memory segment");

  . = ASSERT(__STACKSIZE_IRQ__  == ALIGN(__STACKSIZE_IRQ__, 8), "error: .stack_irq section size does not match alignment");

  . = ASSERT(__stack_irq_size__ <= __STACKSIZE_IRQ__ , "error: .stack_irq section is larger than specified size");

  __stack_fiq_load_start__ = ALIGN(__stack_irq_end__ , 8);
  .stack_fiq ALIGN(__stack_irq_end__ , 8) (NOLOAD) : AT(ALIGN(__stack_irq_end__ , 8))
  {
    __stack_fiq_start__ = .;
    *(.stack_fiq .stack_fiq.*)
    . = ALIGN(MAX(__stack_fiq_start__ + __STACKSIZE_FIQ__ , .), 8);
  }
  __stack_fiq_end__ = __stack_fiq_start__ + SIZEOF(.stack_fiq);
  __stack_fiq_size__ = SIZEOF(.stack_fiq);
  __stack_fiq_load_end__ = __stack_fiq_end__;

  . = ASSERT(__stack_fiq_start__ == __stack_fiq_end__ || (__stack_fiq_end__ - __BOOT_RAM_segment_start__) <= __BOOT_RAM_segment_size__ , "error: .stack_fiq is too large to fit in BOOT_RAM memory segment");

  . = ASSERT(__STACKSIZE_FIQ__  == ALIGN(__STACKSIZE_FIQ__, 8), "error: .stack_fiq section size does not match alignment");

  . = ASSERT(__stack_fiq_size__ <= __STACKSIZE_FIQ__ , "error: .stack_fiq section is larger than specified size");

  __stack_svc_load_start__ = ALIGN(__stack_fiq_end__ , 8);
  .stack_svc ALIGN(__stack_fiq_end__ , 8) (NOLOAD) : AT(ALIGN(__stack_fiq_end__ , 8))
  {
    __stack_svc_start__ = .;
    *(.stack_svc .stack_svc.*)
    . = ALIGN(MAX(__stack_svc_start__ + __STACKSIZE_SVC__ , .), 8);
  }
  __stack_svc_end__ = __stack_svc_start__ + SIZEOF(.stack_svc);
  __stack_svc_size__ = SIZEOF(.stack_svc);
  __stack_svc_load_end__ = __stack_svc_end__;

  . = ASSERT(__stack_svc_start__ == __stack_svc_end__ || (__stack_svc_end__ - __BOOT_RAM_segment_start__) <= __BOOT_RAM_segment_size__ , "error: .stack_svc is too large to fit in BOOT_RAM memory segment");

  . = ASSERT(__STACKSIZE_SVC__  == ALIGN(__STACKSIZE_SVC__, 8), "error: .stack_svc section size does not match alignment");

  . = ASSERT(__stack_svc_size__ <= __STACKSIZE_SVC__ , "error: .stack_svc section is larger than specified size");

  __stack_abt_load_start__ = ALIGN(__stack_svc_end__ , 8);
  .stack_abt ALIGN(__stack_svc_end__ , 8) (NOLOAD) : AT(ALIGN(__stack_svc_end__ , 8))
  {
    __stack_abt_start__ = .;
    *(.stack_abt .stack_abt.*)
    . = ALIGN(MAX(__stack_abt_start__ + __STACKSIZE_ABT__ , .), 8);
  }
  __stack_abt_end__ = __stack_abt_start__ + SIZEOF(.stack_abt);
  __stack_abt_size__ = SIZEOF(.stack_abt);
  __stack_abt_load_end__ = __stack_abt_end__;

  . = ASSERT(__stack_abt_start__ == __stack_abt_end__ || (__stack_abt_end__ - __BOOT_RAM_segment_start__) <= __BOOT_RAM_segment_size__ , "error: .stack_abt is too large to fit in BOOT_RAM memory segment");

  . = ASSERT(__STACKSIZE_ABT__  == ALIGN(__STACKSIZE_ABT__, 8), "error: .stack_abt section size does not match alignment");

  . = ASSERT(__stack_abt_size__ <= __STACKSIZE_ABT__ , "error: .stack_abt section is larger than specified size");

  __stack_und_load_start__ = ALIGN(__stack_abt_end__ , 8);
  .stack_und ALIGN(__stack_abt_end__ , 8) (NOLOAD) : AT(ALIGN(__stack_abt_end__ , 8))
  {
    __stack_und_start__ = .;
    *(.stack_und .stack_und.*)
    . = ALIGN(MAX(__stack_und_start__ + __STACKSIZE_UND__ , .), 8);
  }
  __stack_und_end__ = __stack_und_start__ + SIZEOF(.stack_und);
  __stack_und_size__ = SIZEOF(.stack_und);
  __stack_und_load_end__ = __stack_und_end__;

  . = ASSERT(__stack_und_start__ == __stack_und_end__ || (__stack_und_end__ - __BOOT_RAM_segment_start__) <= __BOOT_RAM_segment_size__ , "error: .stack_und is too large to fit in BOOT_RAM memory segment");

  . = ASSERT(__STACKSIZE_UND__  == ALIGN(__STACKSIZE_UND__, 8), "error: .stack_und section size does not match alignment");

  . = ASSERT(__stack_und_size__ <= __STACKSIZE_UND__ , "error: .stack_und section is larger than specified size");

  __tbss_load_start__ = ALIGN(__stack_und_end__ , 4);
  .tbss ALIGN(__stack_und_end__ , 4) (NOLOAD) : AT(ALIGN(__stack_und_end__ , 4))
  {
    __tbss_start__ = .;
    *(.tbss .tbss.*)
  }
  __tbss_end__ = __tbss_start__ + SIZEOF(.tbss);
  __tbss_size__ = SIZEOF(.tbss);
  __tbss_load_end__ = __tbss_end__;

  . = ASSERT(__tbss_start__ == __tbss_end__ || (__tbss_end__ - __BOOT_RAM_segment_start__) <= __BOOT_RAM_segment_size__ , "error: .tbss is too large to fit in BOOT_RAM memory segment");

  __tdata_load_start__ = ALIGN(__tbss_end__ , 4);
  .tdata ALIGN(__tbss_end__ , 4) : AT(ALIGN(__tbss_end__ , 4))
  {
    __tdata_start__ = .;
    *(.tdata .tdata.*)
  }
  __tdata_end__ = __tdata_start__ + SIZEOF(.tdata);
  __tdata_size__ = SIZEOF(.tdata);
  __tdata_load_end__ = __tdata_end__;

  . = ASSERT(__tdata_start__ == __tdata_end__ || (__tdata_end__ - __BOOT_RAM_segment_start__) <= __BOOT_RAM_segment_size__ , "error: .tdata is too large to fit in BOOT_RAM memory segment");

  __mmu_load_start__ = ALIGN(__tdata_end__ , 16384);
  .mmu ALIGN(__tdata_end__ , 16384) (NOLOAD) : AT(ALIGN(__tdata_end__ , 16384))
  {
    __mmu_start__ = .;
    *(.mmu .mmu.*)
  }
  __mmu_end__ = __mmu_start__ + SIZEOF(.mmu);
  __mmu_size__ = SIZEOF(.mmu);
  __mmu_load_end__ = __mmu_end__;

  __BOOT_RAM_segment_used_end__ = ALIGN(__tdata_end__ , 16384) + SIZEOF(.mmu);
  __BOOT_RAM_segment_used_size__ = __BOOT_RAM_segment_used_end__ - __BOOT_RAM_segment_start__;

  . = ASSERT(__mmu_start__ == __mmu_end__ || (__mmu_end__ - __BOOT_RAM_segment_start__) <= __BOOT_RAM_segment_size__ , "error: .mmu is too large to fit in BOOT_RAM memory segment");

}

