/*================================*/
/*=====LPC8XX GNU STARTUP========*/
/*==A CODERED COMPATIBLE STARTUP==*/
/*================================*/
#if defined (__cplusplus)
#ifdef __REDLIB__
#error Redlib does not support C++
#else
//*****************************************************************************
//
// The entry point for the C++ library startup
//
//*****************************************************************************
extern "C"
{
  extern void __libc_init_array(void);
}
#endif
#endif

#define WEAK __attribute__ ((weak))
#define ALIAS(f) __attribute__ ((weak, alias (#f)))

#include "LPC8xx.h"

//*****************************************************************************
#if defined (__cplusplus)
extern "C" {
#endif

extern unsigned int __data_load_addr;
extern unsigned int __data_start;
extern unsigned int __data_end;
extern unsigned int __bss_start;
extern unsigned int __bss_end;
extern unsigned int __StackTop;
#ifdef FIXED_STACKHEAP_SIZE
#define STACK_SIZE  (800)
#define HEAP_SIZE  (200)
unsigned char StackMem[STACK_SIZE] __attribute__ ((section(".stack")));
unsigned char HeapMem[HEAP_SIZE] __attribute__ ((section(".heap"), align(8)));
#endif
//*****************************************************************************
//
// Forward declaration of the default handlers. These are aliased.
// When the application defines a handler (with the same name), this will
// automatically take precedence over these weak definitions
//
//*****************************************************************************
     void Reset_Handler(void);
WEAK void NMI_Handler(void);
WEAK void HardFault_Handler(void);
WEAK void SVC_Handler(void);
WEAK void PendSV_Handler(void);
WEAK void SysTick_Handler(void);
WEAK void IntDefaultHandler(void);
//*****************************************************************************
//
// Forward declaration of the specific IRQ handlers. These are aliased
// to the IntDefaultHandler, which is a 'forever' loop. When the application
// defines a handler (with the same name), this will automatically take
// precedence over these weak definitions
//
//*****************************************************************************
void SPI0_IRQHandler       (void) ALIAS(IntDefaultHandler);
void SPI1_IRQHandler       (void) ALIAS(IntDefaultHandler);
void UART0_IRQHandler      (void) ALIAS(IntDefaultHandler);
void UART1_IRQHandler      (void) ALIAS(IntDefaultHandler);
void UART2_IRQHandler      (void) ALIAS(IntDefaultHandler);
void I2C_IRQHandler        (void) ALIAS(IntDefaultHandler);
void SCT_IRQHandler        (void) ALIAS(IntDefaultHandler);
void MRT_IRQHandler        (void) ALIAS(IntDefaultHandler);
void CMP_IRQHandler        (void) ALIAS(IntDefaultHandler);
void WDT_IRQHandler        (void) ALIAS(IntDefaultHandler);
void BOD_IRQHandler        (void) ALIAS(IntDefaultHandler);
void WKT_IRQHandler        (void) ALIAS(IntDefaultHandler);
void PININT0_IRQHandler    (void) ALIAS(IntDefaultHandler);
void PININT1_IRQHandler    (void) ALIAS(IntDefaultHandler);
void PININT2_IRQHandler    (void) ALIAS(IntDefaultHandler);
void PININT3_IRQHandler    (void) ALIAS(IntDefaultHandler);
void PININT4_IRQHandler    (void) ALIAS(IntDefaultHandler);
void PININT5_IRQHandler    (void) ALIAS(IntDefaultHandler);
void PININT6_IRQHandler    (void) ALIAS(IntDefaultHandler);
void PININT7_IRQHandler    (void) ALIAS(IntDefaultHandler);

//*****************************************************************************
//
// The entry point for the application.
// __main() is the entry point for Redlib based applications
// main() is the entry point for Newlib based applications
//
//*****************************************************************************
#if defined (__REDLIB__)
extern void __main(void);
#endif
extern int main(void);
//*****************************************************************************
#if defined (__cplusplus)
} // extern "C"
#endif
//*****************************************************************************
//
// The vector table.  Note that the proper constructs must be placed on this to
// ensure that it ends up at physical address 0x0000.0000.
//
//*****************************************************************************
__attribute__ ((section(".isr_vector_table")))
void (* const Vectors[])(void) = {
#ifdef FIXED_STACKHEAP_SIZE
  (void (*)(void))(StackMem + STACK_SIZE),          // The initial stack pointer
#else
  (void (*)(void))&__StackTop,
#endif
  // Cortex M0+ Handlers
  Reset_Handler,      // The reset handler
  NMI_Handler,        // The NMI handler
  HardFault_Handler,  // The hard fault handler
  0,                  // Reserved
  0,                  // Reserved
  0,                  // Reserved
  0,                  // Reserved
  0,                  // Reserved
  0,                  // Reserved
  0,                  // Reserved
  SVC_Handler,        // SVCall handler
  0,                  // Reserved
  0,                  // Reserved
  PendSV_Handler,     // The PendSV handler
  SysTick_Handler,    // The SysTick handler

  // LPC8xx Handlers
  SPI0_IRQHandler,    // SPI0 controller
  SPI1_IRQHandler,    // SPI1 controller
  0,                  // Reserved
  UART0_IRQHandler,   // UART0
  UART1_IRQHandler,   // UART1
  UART2_IRQHandler,   // UART2
  0,                  // Reserved
  0,                  // Reserved
  I2C_IRQHandler,     // I2C controller
  SCT_IRQHandler,     // Smart Counter Timer
  MRT_IRQHandler,     // Multi-Rate Timer
  CMP_IRQHandler,     // Comparator
  WDT_IRQHandler,     // PIO1 (0:11)
  BOD_IRQHandler,     // Brown Out Detect
  0,                  // Reserved
  WKT_IRQHandler,     // Wakeup timer
  0,                  // Reserved
  0,                  // Reserved
  0,                  // Reserved
  0,                  // Reserved
  0,                  // Reserved
  0,                  // Reserved
  0,                  // Reserved
  0,                  // Reserved
  PININT0_IRQHandler, // PIO INT0
  PININT1_IRQHandler, // PIO INT1
  PININT2_IRQHandler, // PIO INT2
  PININT3_IRQHandler, // PIO INT3
  PININT4_IRQHandler, // PIO INT4
  PININT5_IRQHandler, // PIO INT5
  PININT6_IRQHandler, // PIO INT6
  PININT7_IRQHandler, // PIO INT7
};

//*****************************************************************************
// Reset entry point for your code.
// Sets up a simple runtime environment and initializes the C/C++
// library.
//*****************************************************************************
__attribute__ ((section(".after_vectors")))
void Reset_Handler(void)
{
  /*
   * Initialize Internal SRAM
   */
  unsigned int *src, *dst;

  /* Copy data section from flash to RAM */
  src = &__data_load_addr;
  dst = &__data_start;
  while (dst < &__data_end)
    *dst++ = *src++;

  /* Zero fill the bss section */
  dst = &__bss_start;
  while (dst < &__bss_end)
    *dst++ = 0;

  SystemInit();

#if defined (__cplusplus)
  //
  // Call C++ library initialisation
  //
  __libc_init_array();
#endif

#if defined (__REDLIB__)
  // Call the Redlib library, which in turn calls main()
  __main() ;
#else
  main();
#endif
  //
  // main() shouldn't return, but if it does, we'll just enter an infinite loop
  //
  while (1)
  {
  }
}

//*****************************************************************************
//
// This is the code that gets called when the processor receives a NMI.  This
// simply enters an infinite loop, preserving the system state for examination
// by a debugger.
//
//*****************************************************************************
__attribute__ ((section(".after_vectors")))
void NMI_Handler(void)
{
    while(1)
    {
    }
}

__attribute__ ((section(".after_vectors")))
void HardFault_Handler(void)
{
    while(1)
    {
    }
}

__attribute__ ((section(".after_vectors")))
void SVCall_Handler(void)
{
    while(1)
    {
    }
}

__attribute__ ((section(".after_vectors")))
void PendSV_Handler(void)
{
    while(1)
    {
    }
}
__attribute__ ((section(".after_vectors")))
void SysTick_Handler(void)
{
    while(1)
    {
    }
}

//*****************************************************************************
//
// Processor ends up here if an unexpected interrupt occurs or a handler
// is not present in the application code.
//
//*****************************************************************************
__attribute__ ((section(".after_vectors")))
void IntDefaultHandler(void)
{
    while(1)
    {
    }
}
