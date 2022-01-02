/* GPLv2 (c) Airbus */
#include <debug.h>
#include <info.h>
#include <segmem.h>

#define NBR_SEG 5
#define SEG_SIZE sizeof(seg_desc_t)

extern info_t *info;
seg_desc_t gdt[NBR_SEG]; 
gdt_reg_t newgdtr ; 
tss_t tss_entry; 

extern info_t *info;

void print_gdt()
{
   // print GDT 
   struct global_descriptor_table_register startGDT ; 
   get_gdtr(startGDT); 
   int start = (int)startGDT.desc ; 
   seg_desc_t * line = startGDT.desc;

   debug("start of GDT at %x \n", start);
   debug("====> limit GDT : %x \n", startGDT.limit); 
   
   // print GDT
   int i;
   for(i = 0; i < startGDT.limit + 1 ; i+= sizeof(seg_desc_t))
   {
     debug("Segment n° %d\n", i); 
     debug("segment limit : %x\n", line->limit_1 | (line->limit_2 << 16)); 
     debug("base: %x%x%x\n", line->base_1 | (line->base_2 << 16) | (line->base_3 << 24)); 
     debug("type: %x, flag: %x, priv: %x\n", line->type, line->s, line->dpl);
     debug("flag: %x, granularity: %x\n", line->p, line->g);
     debug("\n");  
     line++;  
   }
}

void create_gdt_segment(int i, uint64_t limit, uint64_t base, uint64_t seg_type, uint64_t desc_type, uint64_t privilege, uint64_t seg_flag, uint64_t granularity, uint64_t longmode)
{
  gdt[i].raw = 0; 
  gdt[i].limit_1 = limit & 0xFFFF; 
  gdt[i].limit_2 = (limit >> 16) & 0x0F ; 
  gdt[i].base_1 = base ; 
  gdt[i].base_2 = base >> 16; 
  gdt[i].base_3 = base >> 24;
  gdt[i].type = seg_type; 
  gdt[i].s = desc_type; 
  gdt[i].dpl = privilege; 
  gdt[i].p = seg_flag; 
  gdt[i].avl = 0; 
  gdt[i].l = longmode; 
  gdt[i].d = granularity; 
  gdt[i].g = granularity; 
}

void createGDT()
{
   gdt_reg_t default_gdtr ;
   get_gdtr(default_gdtr) ;

   default_gdtr.limit = NBR_SEG * SEG_SIZE - 1 ;
   default_gdtr.desc = gdt ;
   set_gdtr(default_gdtr) ;

   // initialize newgdtr
   debug("create GDT\n");  

   // Add 3 segments (1 null, 1 code, 1 data) 
   // ring 0
   create_gdt_segment(0, 0xfff0, 0, 0, 0, 0, 0, 0, 0); 
   create_gdt_segment(1, 0xfffff, 0, SEG_DESC_CODE_XR, 1, 0, 1, 1, 0); 
   create_gdt_segment(2, 0xfffff, 0, SEG_DESC_DATA_RW, 1, 0, 1, 1, 0); 
   // ring 3  
   create_gdt_segment(3, 0xfffff, 0, SEG_DESC_CODE_XR, 1, 3, 1, 1, 0);
   create_gdt_segment(4, 0xfffff, 0, SEG_DESC_DATA_RW, 1, 3, 1, 1, 0); 

   // initialize segment registers (cs : debut data, SS : debut code, DS : fin data, ES, FS, GS)
   debug("11111\n"); 
   set_ss(gdt_seg_sel(2, SEG_SEL_KRN));
   set_ds(gdt_seg_sel(2, SEG_SEL_KRN)); 
   set_cs(gdt_seg_sel(1, SEG_SEL_KRN)); 
   
   debug("new GDT :\n\n"); 
   print_gdt(); 
}

void createTSS(int i)
{
   uint32_t base = (uint32_t) &tss_entry;
   uint32_t limit = sizeof tss_entry;
   
   // TSS sector in GDT 
   create_gdt_segment(5, (uint32_t) &tss_entry, sizeof tss_entry, SEG_DESC_SYS_TSS_AVL_32, 1, 0, 1, 1, 0); 
   memset(&tss_entry, 0, sizeof tss_entry); 
   
   tss_entry.eip = 
}

void userland()
{
   asm volatile("mov %%eax, %%cr0":::); 
}

void tp()
{ 
   createGDT(); 

   // "far jump" : a jump to an instruction in a different segment than the current code segment but as the same privilege level
   // --> ça ne peut pas marcher

   
   // Go to ring 3
   // 1 - TSS
   
   
   asm volatile("iret":::); 
}

