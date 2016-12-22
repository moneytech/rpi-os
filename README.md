Raspberry Pi OS Project
=======================

A toy project playing around with a Raspberry Pi ARM1176 chipset.

The `boot/` folder is the files included on a standard Raspberry Pi SD card.
The source code will compile to ELF and binary output which can replace the RPI's
standard boot files to create your own RPI OS.

The OS can be ran within QEMU as well using the `qemu-system-arm` emulator.

## Requirements

* ARM development tools (`arm-none-eabi-gcc`)
* QEMU recent build (`qemu-system-arm`)
* Make

## TODO

* User interaction (command line)
* Memory allocation (`malloc`)
* Read status of system

## Resources

* [Raspberry Pi Bare Bones](http://wiki.osdev.org/Raspberry_Pi_Bare_Bones)
* [OS Theory](http://wiki.osdev.org/Category:OS_theory)