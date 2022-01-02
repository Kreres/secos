// ##########
// creation of the IDT
// ##########

#ifndef INTERRUPTION
#define INTERRUPTION

#include <gpr.h>
#include <asm.h>
#include <debug.h>
#include <info.h>
#include <segmem.h>
#include <intr.h>
#include <types.h>
#include <cr.h>
#include <io.h>
#include <pic.h>
#include <exam/task.h>	

// create a idt descriptor given a index and a CPU privilege level
void createIDThandlerSyscall();

void syscall_isr();

void createIDThandlerIRQ();


#endif