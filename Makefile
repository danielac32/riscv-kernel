

#PREFIX:=riscv64-unknown-elf-
#CFLAGS:=-I/usr/include
#export PATH=/opt/riscv/bin:$PATH
TOOLSET     ?= riscv64-unknown-elf-
CC           = $(TOOLSET)gcc
LD           = $(TOOLSET)ld
AR           = $(TOOLSET)gcc-ar
OBJCOPY      = $(TOOLSET)objcopy
OPTFLAGS    ?= -Og
 

RM = rm -f
fixpath = $(strip $1)

 
CFLAGS      ?= -O3 -march=rv32ima -mabi=ilp32 -ffreestanding -nostdlib -nostartfiles -lgcc -fno-builtin 
INCLUDES     =   -I include 
LDSCRIPT     =  linker.ld

OBJDIR        = obj
START         = $(wildcard system/boot.S) $(wildcard system/intr.S) $(wildcard system/ctxsw.S)
STARTOBJ      = $(addprefix $(OBJDIR)/, $(addsuffix .o, $(notdir $(basename $(START)))))
SOURCES       = $(wildcard system/*.c) 
OBJECTS       = $(addprefix $(OBJDIR)/, $(addsuffix .o, $(notdir $(basename $(SOURCES)))))
SRCLIB        = $(wildcard lib/*.c)
LIBOBJ        = $(addprefix $(OBJDIR)/, $(addsuffix .o, $(notdir $(basename $(SRCLIB)))))
FLOATLIB      = $(wildcard floatmath/*.S)
FLOATOBJ      = $(addprefix $(OBJDIR)/, $(addsuffix .o, $(notdir $(basename $(FLOATLIB)))))
FSLIB         = $(wildcard littlefs/*.c)
FSOBJ         = $(addprefix $(OBJDIR)/, $(addsuffix .o, $(notdir $(basename $(FSLIB)))))
SRCDEVICE     = $(wildcard device/nam/*.c) $(wildcard device/tty/*.c) 
DEVICEOBJ     = $(addprefix $(OBJDIR)/, $(addsuffix .o, $(notdir $(basename $(SRCDEVICE)))))
SRCSHELL      = #$(wildcard shell/*.c) 
SHELLOBJ      = #$(addprefix $(OBJDIR)/, $(addsuffix .o, $(notdir $(basename $(SRCSHELL)))))


DOUT         = kernel


SRCPATH = $(sort $(dir $(START) $(SOURCES) $(SRCLIB) $(FLOATLIB) $(FSLIB) $(SRCSHELL) $(SRCDEVICE)))
vpath %.c $(SRCPATH)
vpath %.S $(SRCPATH)


 
$(OBJDIR):
	@mkdir $@


kernel: $(DOUT).bin
		riscv32-unknown-elf-objdump -d kernel.elf > kernel.list


$(DOUT).bin : $(DOUT).elf
	@echo building $@
	@$(OBJCOPY) -O binary $< $@



$(DOUT).elf : $(OBJDIR) $(STARTOBJ) $(LIBOBJ) $(FLOATOBJ) $(FSOBJ) $(DEVICEOBJ) $(SHELLOBJ) $(OBJECTS) 
	@echo building $@
	@$(CC) $(CFLAGS) $(LDFLAGS) -T $(LDSCRIPT) $(STARTOBJ) $(LIBOBJ) $(FLOATOBJ) $(FSOBJ) $(SHELLOBJ) $(DEVICEOBJ) $(OBJECTS) -o $@

	
clean: $(OBJDIR)
	$(MAKE) --version
	@$(RM) $(DOUT).*
	@$(RM) $(call fixpath, $(OBJDIR)/*.*)


$(OBJDIR)/%.o: %.S
	@echo assembling $<
	@$(CC) $(CFLAGS)  $(INCLUDES) -c $< -o $@

$(OBJDIR)/%.o: %.c
	@echo compiling $<
	@$(CC) $(CFLAGS)  $(INCLUDES) -c $< -o $@

run:
	clear
	gcc -o mini-rv32ima ../mini-rv32ima/mini-rv32ima.c 
	./mini-rv32ima -f kernel.bin

riscv:
	gcc -o mini-rv32ima ../mini-rv32ima/mini-rv32ima.c 