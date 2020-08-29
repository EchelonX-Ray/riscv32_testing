#include "./../inc/types.h"
#include "./../inc/string.h"
#include "./inc/general_oper.h"
#include "./inc/memmap.h"
#include "./inc/gpio_oper.h"
#include "./drivers/uart.h"
#include "./interrupts/base.h"
#include "./memalloc.h"
#include "./globals.h"

#define rck 2
#define srck 3
#define s_in 4

void kwrite(char* str) {
	uart_write(str, UART0_BASE);
	return;
}

void printv(char* str) {
	kwrite(str);
	return;
}

void printh(char* str, uint32_t num, char* str2) {
	kwrite(str);
	char buff[50];
	itoa(num, buff, 50, -16, 8);
	kwrite(buff);
	kwrite(str2);
	return;
}

void printd(char* str, uint32_t num) {
	kwrite(str);
	char buff[50];
	itoa(num, buff, 50, -10, 0);
	kwrite(buff);
	return;
}

void prints(char* str, char* str2) {
	kwrite(str);
	kwrite(str2);
	return;
}

void kmemtest() {
	kallocinit((void*)&KHEAP_START, (void*)0x80003FFF);
	
	char* cstr1 = "Hello, World!\n";
	size_t cstr1_len = strlen(cstr1) + 1;
	char* cstr2 = "Michael\n";
	size_t cstr2_len = strlen(cstr2) + 1;
	char* cstr3 = "Kloos\n";
	size_t cstr3_len = strlen(cstr3) + 1;
	char* cstr4 = "Bye, World!\n";
	size_t cstr4_len = strlen(cstr4) + 1;
	printd("cstr1_len: ", cstr1_len);
	prints("\tcstr1: ", cstr1);
	printd("cstr2_len: ", cstr2_len);
	prints("\tcstr2: ", cstr2);
	printd("cstr3_len: ", cstr3_len);
	prints("\tcstr3: ", cstr3);
	printd("cstr4_len: ", cstr4_len);
	prints("\tcstr4: ", cstr4);
	printv("\n");
	
	char* dstr1 = kmalloc(cstr1_len);
	strcpy(dstr1, cstr1);
	prints("dstr1: ", dstr1);
	printh("kmalloc dstr1 @   0x", (long unsigned int)dstr1, "\n");
	char* dstr2 = kmalloc(cstr2_len);
	strcpy(dstr2, cstr2);
	prints("dstr2: ", dstr2);
	printh("kmalloc dstr2 @   0x", (long unsigned int)dstr2, "\n");
	char* dstr3 = kmalloc(cstr3_len);
	strcpy(dstr3, cstr3);
	prints("dstr3: ", dstr3);
	printh("kmalloc dstr3 @   0x", (long unsigned int)dstr3, "\n");
	char* dstr4 = kmalloc(cstr4_len);
	strcpy(dstr4, cstr4);
	prints("dstr4: ", dstr4);
	printh("kmalloc dstr4 @   0x", (long unsigned int)dstr4, "\n");
	
	printv("\n");
	kfree(dstr2);
	
	prints("dstr1: ", dstr1);
	printh("kmalloc dstr1 @   0x", (long unsigned int)dstr1, "\n");
	prints("dstr2: ", dstr2);
	printh("kmalloc dstr2 @   0x", (long unsigned int)dstr2, "\n");
	prints("dstr3: ", dstr3);
	printh("kmalloc dstr3 @   0x", (long unsigned int)dstr3, "\n");
	prints("dstr4: ", dstr4);
	printh("kmalloc dstr4 @   0x", (long unsigned int)dstr4, "\n");
	
	printv("\n");
	dstr2 = kmalloc(cstr2_len);
	
	prints("dstr1: ", dstr1);
	printh("kmalloc dstr1 @   0x", (long unsigned int)dstr1, "\n");
	prints("dstr2: ", dstr2);
	printh("kmalloc dstr2 @   0x", (long unsigned int)dstr2, "\n");
	prints("dstr3: ", dstr3);
	printh("kmalloc dstr3 @   0x", (long unsigned int)dstr3, "\n");
	prints("dstr4: ", dstr4);
	printh("kmalloc dstr4 @   0x", (long unsigned int)dstr4, "\n");
	
	printv("\n");
	strcpy(dstr2, "Hi\n");
	
	prints("dstr1: ", dstr1);
	printh("kmalloc dstr1 @   0x", (long unsigned int)dstr1, "\n");
	prints("dstr2: ", dstr2);
	printh("kmalloc dstr2 @   0x", (long unsigned int)dstr2, "\n");
	prints("dstr3: ", dstr3);
	printh("kmalloc dstr3 @   0x", (long unsigned int)dstr3, "\n");
	prints("dstr4: ", dstr4);
	printh("kmalloc dstr4 @   0x", (long unsigned int)dstr4, "\n");
	
	printv("\n");
	
	prints("dstr1: ", dstr1);
	printh("kmalloc dstr1 @   0x", (long unsigned int)dstr1, "\n");
	prints("dstr2: ", dstr2 + 4);
	printh("kmalloc dstr2 @   0x", (long unsigned int)dstr2, "\n");
	prints("dstr3: ", dstr3);
	printh("kmalloc dstr3 @   0x", (long unsigned int)dstr3, "\n");
	prints("dstr4: ", dstr4);
	printh("kmalloc dstr4 @   0x", (long unsigned int)dstr4, "\n");
	
	return;
}

signed int kmain(unsigned int argc, char* argv[], char* envp[]) {
	volatile uint32_t* urat_reg;
	volatile uint32_t* ctrl_reg;
	volatile uint32_t* prci_reg;
	
	// Setup the Clock to 256MHz by setting up the PLL Frequency Multipiers and Dividers
	prci_reg = (uint32_t*)(PRCI_BASE + PRCI_HFROSCCFG);
	*prci_reg |= (1 << 30);
	prci_reg = (uint32_t*)(PRCI_BASE + PRCI_PLLCFG);
	*prci_reg |= (1 << 16) | (1 << 17) | (1 << 18);
	// The input is 16MHz from the external crystal oscillator HFXOSC on the Redboard Red-V
	// pllr = 1 Step #1: Divide by [pllr + 1] = 2 [bitshift by 0 to locate in PLLCFG register]
	// pllf = 31 Step #2: Multiply by [2 * (pllf + 1)] = 64 [bitshift by 4 to locate in PLLCFG register]
	// pllq = 1 Step #3: Divide by [2 ^ pllq] = 2 [bitshift by 10 to locate in PLLCFG register]
	*prci_reg = (*prci_reg & 0x7FFFF000) | (1 << 0) | (31 << 4) | (1 << 10);
	*prci_reg &= ~(1 << 18);
	prci_reg = (uint32_t*)(PRCI_BASE + PRCI_HFROSCCFG);
	*prci_reg &= ~(1 << 30);

	// Set Div to 16MHz / (138 + 1) = ~115200 Baud
	urat_reg = (uint32_t*)(UART0_BASE + UART_DIV);
	*urat_reg = 2221;
	urat_reg = (uint32_t*)(UART1_BASE + UART_DIV);
	*urat_reg = 2221;

	// Disable UART Watermark Interrupts
	ctrl_reg = (uint32_t*)(UART0_BASE + UART_IE);
	*ctrl_reg = 0x0;
	ctrl_reg = (uint32_t*)(UART1_BASE + UART_IE);
	*ctrl_reg = 0x0;

	// Set GPIO Pins 2, 3, 4, and 5 to GPIO mode, Output mode, and Turn Them Off
	IOF_SEL(5, 0);
	IOF_SEL(4, 0);
	IOF_SEL(3, 0);
	IOF_SEL(2, 0);
	OUTPUT(5);
	OUTPUT(4);
	OUTPUT(3);
	OUTPUT(2);
	LOW(5);
	LOW(4);
	LOW(3);
	LOW(2);

	// Set HW I/O UART0 functionality through the GPIO Pins (IOF)
	IOF_SEL(16, 1);
	IOF_SEL(17, 1);

	// Enable TX on UART0
	ctrl_reg = (uint32_t*)(UART0_BASE + UART_TXCTRL);
	*ctrl_reg = 0x1;

	// CLINT
	// mtimecmp
	ctrl_reg = (uint32_t*)(CLINT_BASE + CLINT_MTIMECMP_LO); // Lower-half of 64-bit value
	*ctrl_reg = 32768 / 2;
	//*ctrl_reg = 1000;
	ctrl_reg = (uint32_t*)(CLINT_BASE + CLINT_MTIMECMP_HI); // Upper-half of 64-bit value
	*ctrl_reg = 0;
	// mtime
	ctrl_reg = (uint32_t*)(CLINT_BASE + CLINT_MTIME_LO); // Lower-half of 64-bit value
	*ctrl_reg = 0;
	ctrl_reg = (uint32_t*)(CLINT_BASE + CLINT_MTIME_HI); // Upper-half of 64-bit value
	*ctrl_reg = 0;

	// lfrosc
	// This has no effect on the Red-V because the lfrosc is driven by an external crystal oscillator @ 32.768khz
	//ctrl_reg = (uint32_t*)(AON_BASE + AON_LFROSCCFG);

	// rtc counter_start
	//ctrl_reg = (uint32_t*)(AON_BASE + AON_RTCCOUNTLO); // Low-Half
	//*ctrl_reg = 0x00000000;
	//ctrl_reg = (uint32_t*)(AON_BASE + AON_RTCCOUNTHI); // High-Half
	//*ctrl_reg = 0x00000000;

	// rtccmp0
	ctrl_reg = (uint32_t*)(AON_BASE + AON_RTCCMP0);
	*ctrl_reg = 0xF0000000;

	__asm__ __volatile__ ("csrrw zero, mtvec, %0" : : "r" (&interrupt_handler));
	__asm__ __volatile__ ("csrrw zero, mie, %0" : : "r" (0x00000000));
	__asm__ __volatile__ ("csrrs zero, mstatus, %0" : : "r" (0x00000008));
	
	// RTCCFG Enable
	ctrl_reg = (uint32_t*)(AON_BASE + AON_RTCCFG);
	*ctrl_reg = 0x0000100F;
	
	HIGH(5);
	
	kmemtest();
	
	cpu_context_ptr = 0;
	
	/*
	ENABLE_TIMER_INTERRUPT();
	
	LOW(s_in);
	LOW(srck);
	LOW(rck);
	pause();
	
	HIGH(s_in);
	pause();
	HIGH(srck);
	pause();
	LOW(srck);
	pause();
	
	LOW(s_in);
	pause();
	HIGH(srck);
	pause();
	LOW(srck);
	pause();
	
	HIGH(s_in);
	pause();
	HIGH(srck);
	pause();
	LOW(srck);
	pause();
	
	LOW(s_in);
	pause();

	HIGH(rck);
	pause();
	LOW(rck);
	pause();

	DISABLE_TIMER_INTERRUPT();
	*/

	// RTCCFG Disable
	ctrl_reg = (uint32_t*)(AON_BASE + AON_RTCCFG);
	*ctrl_reg = 0x00000000;

	kwrite("TraceY\n");
	CPU_WAIT();
	kwrite("TraceZ\n");

	return 0;
}
