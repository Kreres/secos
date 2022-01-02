// ##########
// Tasks
// ##########

#include <exam/task.h>

extern info_t *info;

// list of tasks
struct struct_task tasks[1]; 
// running task
struct struct_task * task_in_progress; 

int is_a_task_running = 0; 

tss_t * tss = (tss_t*)TSS_ADDR;


//__attribute__((naked)) 
// irq handler - see interruption.h - change task
void irq0_int()
{
	outb(PIC_EOI,PIC1);
	
	if (is_a_task_running == 0) // tart the first task
	{  
		debug("start user1\n");
		is_a_task_running = 1; 
		task_in_progress = &tasks[0]; 
	}else // task switch
	{	
		//debug("task switch\n");
		debug("\n####################################\n");
		debug("EIP 1 %x, EIP 2 %x\n", tasks[0].eip_task, tasks[1].eip_task); 
		// find if we are in user1 or user2
		uint32_t current_eip; 
		__asm__ volatile("mov 4(%%ebp), %0" : "=r"(current_eip)); 
		debug("current eip : %x\n", current_eip-7); 
	
		set_ds(gdt_seg_sel(4, SEG_SEL_USR)); 
		set_es(gdt_seg_sel(4, SEG_SEL_USR)); 
		set_fs(gdt_seg_sel(4, SEG_SEL_USR)); 
		set_gs(gdt_seg_sel(4, SEG_SEL_USR)); 
		// print /x * ((struct struct_task *) task_in_progress)
/*
		//save register of previous task
		__asm__ volatile("mov (%%eax), %0" : "=r"(task_in_progress->eax_task));
		__asm__ volatile("mov (%%ebx), %0" : "=r"(task_in_progress->ebx_task));
		__asm__ volatile("mov (%%ecx), %0" : "=r"(task_in_progress->ecx_task));
		__asm__ volatile("mov (%%edx), %0" : "=r"(task_in_progress->edx_task));
		__asm__ volatile("mov (%%esi), %0" : "=r"(task_in_progress->esi_task));
		__asm__ volatile("mov (%%edi), %0" : "=r"(task_in_progress->edi_task));
		__asm__ volatile("mov (%%esp), %0" : "=r"(task_in_progress->kernel_stack_task));
		__asm__ volatile("mov (%%ebp), %0" : "=r"(task_in_progress->ebp_task));
*/
		//save_flags(task_in_progress->eflags); 
		//task_in_progress->eflags = task_in_progress->eflags|EFLAGS_IF;
		
		// set new pgd value
		debug("CR3 : %x\n", task_in_progress->pgd_task);  
		tss->s0.esp = task_in_progress->kernel_stack_task; 
		set_cr3(task_in_progress->pgd_task); 
		set_ebp(task_in_progress->user_stack_task); 
	
		// push plein de trucs
		__asm__ volatile("push %0" :: "r"(gdt_seg_sel(4, SEG_SEL_USR))); //ss
		__asm__ volatile("push %0" :: "r"(task_in_progress->user_stack_task)); //esp
		__asm__ volatile("pushf"); // eflags
		__asm__ volatile("push %0" :: "r"(gdt_seg_sel(3, SEG_SEL_USR))); // cs
		__asm__ volatile("push %0" :: "r"(task_in_progress->eip_task)); // eip
 
 
/*
		//__asm__ volatile("mov %0, %%esp" :: "r"(task_in_progress->kernel_stack_task));
		__asm__ volatile("mov %0, %%eax" :: "r"(task_in_progress->eax_task));
		__asm__ volatile("mov %0, %%ebx" :: "r"(task_in_progress->ebx_task));
		__asm__ volatile("mov %0, %%ecx" :: "r"(task_in_progress->ecx_task));
		__asm__ volatile("mov %0, %%edx" :: "r"(task_in_progress->edx_task));
		__asm__ volatile("mov %0, %%esi" :: "r"(task_in_progress->esi_task));
		__asm__ volatile("mov %0, %%edi" :: "r"(task_in_progress->edi_task));		
	

		__asm__ volatile("mov (%%eax), %0" : "=r"(task_in_progress->eax_task));
		__asm__ volatile("mov (%%ebx), %0" : "=r"(task_in_progress->ebx_task));
		__asm__ volatile("mov (%%ecx), %0" : "=r"(task_in_progress->ecx_task));
		__asm__ volatile("mov (%%edx), %0" : "=r"(task_in_progress->edx_task));
		__asm__ volatile("mov (%%esi), %0" : "=r"(task_in_progress->esi_task));
		__asm__ volatile("mov (%%edi), %0" : "=r"(task_in_progress->edi_task));
		__asm__ volatile("mov (%%esp), %0" : "=r"(task_in_progress->kernel_stack_task));
		__asm__ volatile("mov (%%ebp), %0" : "=r"(task_in_progress->ebp_task));
*/		
		// change task 
		task_in_progress = task_in_progress->next_task;
		
		//while(1); 
		__asm__ volatile("iret"); 

	}
}


// increment the value of counter
void task1()
{
	debug("task1"); 
}

// display the value of counter - equivalent of sys_counter()
void task2(uint32_t * compt)
{
	asm volatile("mov %0, %%eax" :: "r"(compt)); 
	asm volatile("int $0x80":: "a"(compt)); 
}


// init a struct_task 
struct struct_task * init_task(struct struct_task * t, uint32_t i, uint32_t pgd_addr, uint32_t kernel_stack, uint32_t user_stack, uint32_t eip)
{ 
	t->id_task = i;
	t->pgd_task = pgd_addr; 
	t->ctx_task = (uint32_t) 0XEEEE; 
	t->kernel_stack_task = kernel_stack; 
	t->user_stack_task = user_stack; 
	t->eip_task = eip; 
	return t; 
}

// statically create 2 task
void create_2_tasks()
{
	//pte32_t * ptb1 = (pte32_t *)PTB_1_ADDR; 
	//pte32_t * ptb2 = (pte32_t *)PTB_1_ADDR; 
	// create 2 tasks
	init_task(&tasks[0], (uint32_t) 1, PGD_1_ADDR, 0x1720FFF, 0x1700FFF, (uint32_t)&user1); 
	init_task(&tasks[1] , (uint32_t) 2, PGD_2_ADDR, 0x1201FFF, 0x1200FFF, (uint32_t)&user2); 
	debug("task 1 et task 2 créés\n"); 
	tasks[0].next_task = &tasks[1]; 
	tasks[1].next_task = &tasks[0]; 
}

void start_tasks()
{
	create_2_tasks();
	// IDT pages
	pagination_SUP((pde32_t *)&tasks[0].pgd_task, 0x302010, 0x302010 ,  PG_USR|PG_RW); 
	pagination_SUP((pde32_t *)&tasks[1].pgd_task, 0x302010, 0x302010 ,  PG_USR|PG_RW);
	
	debug("task 1 : %x, task2 %x\n", &tasks[0], &tasks[1]); 
	
	// debug line
	//pde32_t * pgd1 = (pde32_t*)PGD_1_ADDR;
	//print_pages(pgd1, 2, 1024); 
}



