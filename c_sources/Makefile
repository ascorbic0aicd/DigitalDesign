default: all

XLEN ?= 64
RISCV_PREFIX ?= riscv$(XLEN)-unknown-elf-
RISCV_GCC ?= $(RISCV_PREFIX)gcc
GCC_WARNS := -Wall -Wextra -Wconversion -pedantic -Wcast-qual -Wcast-align -Wwrite-strings
RISCV_GCC_OPTS ?= -lgcc -static -mcmodel=medany -fvisibility=hidden -Tsections.ld -nolibc -nostartfiles ${GCC_WARNS}
RISCV_OBJDUMP ?= $(RISCV_PREFIX)objdump --disassemble-all --disassemble-zeroes --section=.text --section=.text.startup --section=.text.init --section=.data
RISCV_OBJCOPY ?= $(RISCV_PREFIX)objcopy -O verilog
RISCV_HEXGEN ?= 'BEGIN{output=0;}{ gsub("\r","",$$(NF)); if ($$1 ~/@/){if ($$1 ~/@00000000/) {output=code;} else {output=1- code;}; gsub("@","0x",$$1); addr=strtonum($$1); if (output==1) {printf "@%08x\n",(addr%262144)/4;}} else {if (output==1) { for(i=1;i<NF;i+=4) print $$(i+3)$$(i+2)$$(i+1)$$i;}}}'
RISCV_COEGEN ?= 'BEGIN{printf "memory_initialization_radix=16;\n memory_initialization_vector=\n"; addr=0;} { gsub("\r","",$$(NF)); if ($$1 ~/@/) {gsub("@","0x",$$1);temp=strtonum($$1); for(;addr<temp;addr++){print "00000000,";}} else {printf "%s,\n", $$1; addr=addr+1;}} END{print "0;\n";}'


SRCS := $(wildcard *.c)
OBJS := $(SRCS:.c=.o)
EXEC := main

.c.o:
	$(RISCV_GCC) -march=rv32i -mabi=ilp32 -c $(RISCV_GCC_OPTS) $< -o $@
	
${EXEC}.elf : $(OBJS)
	${RISCV_GCC} -march=rv32i -mabi=ilp32 ${RISCV_GCC_OPTS} -e entry $(OBJS) -o $@
	${RISCV_OBJDUMP} ${EXEC}.elf > ${EXEC}.dump

${EXEC}.tmp: ${EXEC}.elf
	$(RISCV_OBJCOPY) $< $@

${EXEC}.hex: ${EXEC}.tmp
	awk -v code=1 $(RISCV_HEXGEN) $< > $@
	awk -v code=0 $(RISCV_HEXGEN) $< > ${EXEC}_d.hex
${EXEC}.coe: ${EXEC}.hex
	awk ${RISCV_COEGEN} $< > $@
	awk ${RISCV_COEGEN} ${EXEC}_d.hex > ${EXEC}_d.coe
	cp *.hex bin
	cp *.coe bin
	
.PHONY: all clean

all: ${EXEC}.coe

clean:
	rm -f *.o
	rm -f *.dump
	rm -f *.tmp
	rm -f *.elf
	rm -f *.hex
	rm -f *.coe