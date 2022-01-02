/* GPLv2 (c) Airbus */
#include <debug.h>
#include <info.h>
#include <segmem.h>
#include <intr.h>

extern info_t *info;

// display IDT address
void display_idt()
{
   idt_reg_t idtr ;
   get_idtr(idtr); 
   debug("adresse idtr : 0x%x\n", idtr.desc); 
}

// infinite loop with division by 0
void generate_interupt()
{
   while(1)
   {
       int division = 0; 
       debug("test \n");
       debug("%d \n", 10/division); 
   }
}

// intercept breakpoint
void bp_handler()
{
   debug("Elle est bonne !\n"); 

   // print eip
   uint32_t eip;
   asm volatile("mov 4(%%ebp), %0" : "=r" (eip) ::); 
   debug("adresse de l'eip : 0x%x\n", eip); 
   
   // bp_handler should get us back in the stack, after the interruption
   // ebp prend la valeur de esp (esp est en haut de l'empillement), 
   asm volatile("mov %%ebp, %%esp":::);
   asm volatile("pop %%ebp":::); 
   asm volatile("iret":::);
   
}

// trigger a breakpoint
void bp_trigger()
{ 
   debug("Comment est votre blanquette ?\n"); 
   asm volatile("int3");
   debug("Coucou, l'interuption est passée !");
}

// main
void tp()
{
   display_idt(); 
   //generate_interupt(); 
   //bp_handler(); 
   //bp_trigger(); 


   // modify int_desc_t and call bp_handler instead / voir intr_init dans intr.c
   idt_reg_t idtr; 
   get_idtr(idtr); 
   int_desc_t * idt = idtr.desc; 
 
   offset_t off =(unsigned int)&bp_handler; 
   int_desc(&idt[3], gdt_krn_seg_sel(1), off); 
   bp_trigger();
   // end int_desc_t modification


   

   
}
