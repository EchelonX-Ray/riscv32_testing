#ifndef _insertion_general_oper_h
#define _insertion_general_oper_h

#define CPU_WAIT() \
	{	__asm__ __volatile__ ("wfi");	}

#define CPU_RELAX() \
	{	__asm__ __volatile__ ("nop"); \
		__asm__ __volatile__ ("nop"); \
		__asm__ __volatile__ ("nop"); \
		__asm__ __volatile__ ("nop"); \
		__asm__ __volatile__ ("nop");	}

#define ENABLE_SOFTWARE_INTERRUPT() \
	{	__asm__ __volatile__ ("csrrs zero, mie, %0" : : "r" (0x00000008u));	}

#define DISABLE_SOFTWARE_INTERRUPT() \
	{	__asm__ __volatile__ ("csrrc zero, mie, %0" : : "r" (0x00000008u));	}

#define ENABLE_TIMER_INTERRUPT() \
	{	__asm__ __volatile__ ("csrrs zero, mie, %0" : : "r" (0x00000080u));	}

#define DISABLE_TIMER_INTERRUPT() \
	{	__asm__ __volatile__ ("csrrc zero, mie, %0" : : "r" (0x00000080u));	}

#define ENABLE_EXTERNAL_INTERRUPT() \
	{	__asm__ __volatile__ ("csrrs zero, mie, %0" : : "r" (0x00000800u));	}

#define DISABLE_EXTERNAL_INTERRUPT() \
	{	__asm__ __volatile__ ("csrrc zero, mie, %0" : : "r" (0x00000800u));	}

#endif
