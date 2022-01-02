// ##########
// creation of the IDT
// ##########

#include <exam/interruption.h>


extern info_t *info;


// create a idt descriptor for the int 0x80 with privilege 3
void createIDThandlerSyscall()
{
   idt_reg_t idtr;
   get_idtr(idtr);
   debug("IDT @ %x\n", idtr.addr);

   int_desc_t * idt_desc = &idtr.desc[0x80];
   offset_t offset =(unsigned int)syscall_isr;
   
   int_desc(&idtr.desc[0x80], gdt_krn_seg_sel(1), offset); 
   idt_desc -> dpl = 3; 
   idt_desc -> p = 1;
   idt_desc -> type = 0xF; 
   idt_desc -> selector = gdt_seg_sel(1,0); // chance de problemes
}

// handler of the 0x80 interuption
void __regparm__(1) syscall_handler(int_ctx_t *ctx)
{
   //debug("SYSCALL eax = %p\n", ctx->gpr.eax);
   debug("print counter: %x\n", *((uint32_t *)(ctx->gpr.eax.raw)));
}

__attribute__((naked)) void syscall_isr()
{
   asm volatile (
      "pusha        \n"
      "mov %esp, %eax       \n"
      "call syscall_handler \n"
      "popa ; iret"
      );
}


// create a idt descriptor for the irq0 int with privilege 0
void createIDThandlerIRQ()
{
   idt_reg_t idtr;
   get_idtr(idtr);

   int_desc_t * idt_desc = &idtr.desc[32];
   offset_t offset =(unsigned int)irq0_int;

   int_desc(&idtr.desc[32], gdt_krn_seg_sel(1), offset); 
   idt_desc -> dpl = 0; 
   idt_desc -> p = 1;
   idt_desc -> type = 0xF; 
   debug("IRQQQQQQ %p\n", idtr.addr);
   //idt_desc -> selector = gdt_seg_sel(1,0); // chance de problemes
}


