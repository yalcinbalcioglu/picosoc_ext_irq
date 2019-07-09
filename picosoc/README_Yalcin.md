
removed everything related to icebreaker


make hx8ksim
gtkwave testbench.vcd

memory organization

0 to 0x400 sram with init

0x170 onward to 0x3F0 is irq regs for first 32 words and irq stack for 128 words down from 0x3F0

0x800 downward to 0x400 sram stack

0x800 onward flash

make clean

python makefile2dot.py <Makefile >out.dot

irqs worked with cpuregs defined as variables and not as a module


