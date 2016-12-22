#!/bin/sh
BOOT=boot
ELF=$BOOT/pi_os.elf
if [ -f $ELF ]; then
    qemu-system-arm -kernel $ELF -m 256 -M raspi2 -serial stdio
else
    echo "No ELF found; try 'make compile' first"
    exit 1
fi
exit 0
