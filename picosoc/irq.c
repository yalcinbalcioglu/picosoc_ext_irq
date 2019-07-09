// This is free and unencumbered software released into the public domain.
//
// Anyone is free to copy, modify, publish, use, compile, sell, or
// distribute this software, either in source code form or as a compiled
// binary, for any purpose, commercial or non-commercial, and by any
// means.

#include "firmware.h"


uint32_t *irq(uint32_t *regs, uint32_t irqs)
{
	static unsigned int ext_irq_4_count = 0;
	static unsigned int ext_irq_5_count = 0;
	static unsigned int timer_irq_count = 0;

	// checking compressed isa q0 reg handling
	if ((irqs & 6) != 0) {
		uint32_t pc = (regs[0] & 1) ? regs[0] - 3 : regs[0] - 4;
		uint32_t instr = *(uint16_t*)pc;

		if ((instr & 3) == 3)
			instr = instr | (*(uint16_t*)(pc + 2)) << 16;

		if (((instr & 3) != 3) != (regs[0] & 1)) {
			print("Mismatch between q0 LSB and decoded instruction word! q0=0x");
			print_hex(regs[0], 8);
			print(", instr=0x");
			if ((instr & 3) == 3)
				print_hex(instr, 8);
			else
				print_hex(instr, 4);
			print("\n");
			__asm__ volatile ("ebreak");
		}
	}

	if ((irqs & (1<<4)) != 0) {
		ext_irq_4_count++;
		 print("[EXT-IRQ-4]");		
	}

	if ((irqs & (1<<5)) != 0) {
		ext_irq_5_count++;
		 print("[EXT-IRQ-5]");		
	}

	if ((irqs & 1) != 0) {
		timer_irq_count++;
		 print("[TIMER-IRQ]");		
	}

	if ((irqs & 6) != 0)
	{
		uint32_t pc = (regs[0] & 1) ? regs[0] - 3 : regs[0] - 4;
		uint32_t instr = *(uint16_t*)pc;

		if ((instr & 3) == 3)
			instr = instr | (*(uint16_t*)(pc + 2)) << 16;

		print("\n");
		print("------------------------------------------------------------\n");

		if ((irqs & 2) != 0) {
			if (instr == 0x00100073 || instr == 0x9002) {
				print("EBREAK instruction at 0x");
				print_hex(pc, 8);
				print("\n");
			} else {
				print("Illegal Instruction at 0x");
				print_hex(pc, 8);
				print(": 0x");
				print_hex(instr, ((instr & 3) == 3) ? 8 : 4);
				print("\n");
			}
		}

		if ((irqs & 4) != 0) {
			print("Bus error in Instruction at 0x");
			print_hex(pc, 8);
			print(": 0x");
			print_hex(instr, ((instr & 3) == 3) ? 8 : 4);
			print("\n");
		}

		for (int i = 0; i < 8; i++)
		for (int k = 0; k < 4; k++)
		{
			int r = i + k*8;

			if (r == 0) {
				print("pc  ");
			} else
			if (r < 10) {
				putchar('x');
				putchar('0' + r);
				putchar(' ');
				putchar(' ');
			} else
			if (r < 20) {
				putchar('x');
				putchar('1');
				putchar('0' + r - 10);
				putchar(' ');
			} else
			if (r < 30) {
				putchar('x');
				putchar('2');
				putchar('0' + r - 20);
				putchar(' ');
			} else {
				putchar('x');
				putchar('3');
				putchar('0' + r - 30);
				putchar(' ');
			}

			print_hex(regs[r], 8);
			print(k == 3 ? "\n" : "    ");
		}

		print("------------------------------------------------------------\n");

		print("Number of fast external IRQs counted: ");
		print_dec(ext_irq_4_count);
		print("\n");

		print("Number of slow external IRQs counted: ");
		print_dec(ext_irq_5_count);
		print("\n");

		print("Number of timer IRQs counted: ");
		print_dec(timer_irq_count);
		print("\n");

		__asm__ volatile ("ebreak");
	}


	return regs;
}

