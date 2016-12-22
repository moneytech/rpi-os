# binaries needed to build
CC=arm-none-eabi-gcc
OBJCOP=arm-none-eabi-objcopy

# folders
SRC=src
BIN=bin
BOOT=boot

CPU=-mcpu=arm1176jzf-s

# BOOT instruction source and output
BOOTIN=$(SRC)/boot.S
BOOTOUT=$(BIN)/boot.o
BOOTFLAGS=$(CPU) -fpic -ffreestanding -c $(BOOTIN) -o $(BOOTOUT)

# KERNEL instructions source and output
KERNIN=$(SRC)/kernel.c
KERNOUT=$(BIN)/kernel.o
KFLAG2=-O2 -Wall -Wextra
KFLAG1=$(CPU) -fpic -ffreestanding -std=gnu99 -c $(KERNIN) -o $(KERNOUT) $(KFLAG2)

# Kernel linking instructions
LINKER=$(SRC)/linker.ld
QLINKR=$(SRC)/qemulink.ld
LINKOUT=pi_os.elf
BINOUT=pi_os.bin
LINKFLAGS=-T $(LINKER) -o $(BIN)/$(LINKOUT) -ffreestanding -O2 -nostdlib $(BOOTOUT) $(KERNOUT) 
OCFLAGS=$(BIN)/$(LINKOUT) -O binary $(BIN)/$(BINOUT) 

# finally the instructions
compile:
	$(CC) $(BOOTFLAGS) 
	$(CC) $(KFLAG1)
	$(CC) $(LINKFLAGS)
	$(OBJCOP) $(OCFLAGS)
	cp $(BIN)/$(LINKOUT) $(BOOT) 
	cp $(BIN)/$(BINOUT) $(BOOT)

# used to compile for Pi v1
compile_v1:
	$(CC) $(BOOTFLAGS) 
	$(CC) -lDRPI1 $(KFLAG1)
	$(CC) $(LINKFLAGS)
	$(OBJCOP) $(OCFLAGS)
	cp $(LINKOUT) $(BOOT) 
	cp $(BINOUT) $(BOOT)

# clean compiled files out of directories
clean:
	rm -rf $(BIN)/* $(BOOT)/$(LINKOUT) $(BOOT)/$(BINOUT)

# Execute the run shell script
run:
	sh run.sh
