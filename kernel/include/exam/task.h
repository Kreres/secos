// ##########
// Tasks
// ##########

#ifndef TASK
#define TASK

#include <asm.h>
#include <debug.h>
#include <info.h>
#include <segmem.h>
#include <intr.h>
#include <types.h>
#include <cr.h>
#include <io.h>
#include <pic.h>
#include <exam/pagination.h>
#include <exam/tp.h>

struct struct_task
{
	uint32_t eflags; 
	
	uint32_t id_task;
	
	uint32_t pgd_task;

	uint32_t eip_task; 
	
	// registers
	uint32_t ctx_task;
	uint32_t eax_task; 
	uint32_t ebx_task; 
	uint32_t ecx_task; 
	uint32_t edx_task; 
	uint32_t esi_task; 
	uint32_t edi_task;
	uint32_t ebp_task; 
	
	uint32_t kernel_stack_task;
	uint32_t user_stack_task; 
	
	struct struct_task * next_task; 
};

void irq0_int(); 

void task1(); 

void task2(uint32_t * compt); 

void create_2_tasks(); 

void start_tasks(); 

#endif