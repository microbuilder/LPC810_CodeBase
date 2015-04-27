# LPC810 CodeBase

Open source code base for the ARM Cortex M0+ LPC810 family from NXP.

This code base is intended to work with the LPC810 in a DIP8 package.  While these drivers are based on the original LPC800 example code from NXP, the LPC810 has limited resources (4KB flash and 1KB SRAM), so smaller, lighter-weight drivers had to be written to get the most out of these resources we have.

The current code implements the following peripheral drivers:

- A basic SPI driver
- Some simple GPIO helper functions (although GPIO should normally be accessed directly via the appropriate registers)
- A simple driver for UART0 and printf-redirection that allows 'printf' output to be transmitted to UART0
- A basic multi-rate timer driver that allows us to set delays

The code base also implements a mini printf that takes up much less space than the default printf used in most libc variants.  If necessary, it's easy to change the printf redirection to a location other than UART0 via the printf-redirection.c file.

# Using the Makefile

If you wish to use 'make' from the command-line, you can run the following commands:

- `make clean`
- `make all`

## Common 'Make' Errors

### make: arm-none-eabi-gcc: No such file or directory

If you get this error, you need to add the '/bin' folder of your toolchain to your 'system path' variable.

On OS X or Linux this can be done with the following command ([GCC ARM Embedded 2014 Q2](https://launchpad.net/gcc-arm-embedded/4.8/4.8-2014-q2-update) used below, stored in the user's `/Downloads` folder):

```
export PATH=$HOME/Downloads/gcc-arm-none-eabi-4_8-2014q2/bin:$PATH
```

You can test if GCC is available on the system path by running the following command:

```
arm-none-eabi-gcc --version
```