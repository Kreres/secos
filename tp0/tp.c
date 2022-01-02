/* GPLv2 (c) Airbus */
#include <debug.h>
#include <info.h>

extern info_t   *info;
extern uint32_t __kernel_start__;
extern uint32_t __kernel_end__;

void tp() {
   debug("kernel mem [0x%x - 0x%x]\n", &__kernel_start__, &__kernel_end__);
   debug("MBI flags 0x%x\n", info->mbi->flags);

   // modification 
   debug("mem map addr : 0x%x \n", info->mbi->mmap_addr);
   debug("mem map lenth : 0x%x \n", info->mbi->mmap_length); 

   multiboot_memory_map_t * start ;
   start = (multiboot_memory_map_t *) info->mbi->mmap_addr ;

   multiboot_memory_map_t * length ;
   length = (multiboot_memory_map_t *) info->mbi->mmap_length; 

   while (length > start)
   {
      debug("size %d addr %x length %d type %d \n", start->size, start->addr, start->len, start->type); 
      start++; 
   }
}
