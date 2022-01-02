// main of exam 

#include <asm.h>
#include <debug.h>
#include <info.h>
#include <exam/segmentation.h>
#include <exam/interruption.h>
#include <exam/pagination.h>
#include <exam/task.h>

#include <exam/tp.h>

extern info_t *info;

// init GDT, IDT, ...
void init_os()
{
   // SEGMENTATION 5 segments and TSS
   createGDT(); // include TSS
   
   // INTERRUPTION irq and syscall descriptors
   createIDThandlerSyscall();
   createIDThandlerIRQ(); 

   // PAGINATION
   pages_for_all();  
   
   // start multitasking
   start_tasks(); 
   
   // start pagination
   enable_paging(); 
} 


// write in the counter
void  __attribute__((section(".user1"))) user1()
{
	//debug("coucou"); 
	//__asm__ volatile("int $0x80"); 
	while(1); 
}

// display the value of the counter - interruption 0x80
void  __attribute__((section(".user2"))) user2()
{
	//debug("Lance user2"); 
	while(1); 
}



void tp()
{
	init_os(); 	
	
	uint32_t int_compteur = 0;
	debug("var %x\n", int_compteur); 
	force_interrupts_on(); 
	while(1); 	 
	
}