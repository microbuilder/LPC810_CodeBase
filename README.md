LPC810_CodeBase
===============

Open source code base for the ARM Cortex M0+ LPC810 family from NXP.

This code base is intended to work with the LPC810 in a DIP8 package.  While these drivers are based on the original LPC800 example code from NXP, the LPC810 has limited resources (4KB flash and 1KB SRAM), so smaller, lighter-weight drivers had to be written to get the most out of these resources we have.

The current code implements the following peripheral drivers:

- A basic SPI driver
- Some simple GPIO helper functions (although GPIO should normally be accessed directly via the appropriate registers)
- A simple driver for UART0 and printf-redirection that allows 'printf' output to be transmitted to UART0
- A basic multi-rate timer driver that allows us to set delays

The code base also implements a mini printf that takes up much less space than the default printf used in most libc variants.  If necessary, it's easy to change the printf redirection to a location other than UART0 via the printf-redirection.c file.