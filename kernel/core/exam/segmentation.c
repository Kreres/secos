// ##########
// creation of the GDT with the segments
// ##########
#include <exam/segmentation.h>

extern info_t *info;
seg_desc_t gdt[NBR_SEG]; // 6
gdt_reg_t newgdtr ; 

tss_t * tss_entry = (tss_t*)TSS_ADDR; 


// print the current GDT
void print_gdt()
{
   // print GDT 
   struct global_descriptor_table_register startGDT ; 
   get_gdtr(startGDT); 
   int start = (int)startGDT.desc ; 
   seg_desc_t * line = startGDT.desc;

   debug("start of GDT at %x \n\n", start);
   
   // print GDT
   int i;
   for(i = 0; i < startGDT.limit + 1 ; i+= sizeof(seg_desc_t))
   {
     debug("SEGMENT N° %d\n", i/8); 
     debug("segment limit : %x\n", line->limit_1 | (line->limit_2 << 16)); 
	 uint32_t base = (line->base_3 << 24) | (line->base_2 << 16) | line->base_1;
	 debug("base %x\n", base); 
     //debug("base: %x%x%x\n", line->base_1 | (line->base_2 << 16) | (line->base_3 << 24)); 
     debug("seg_type: %x, desc_type: %x, privilege: %x\n", line->type, line->s, line->dpl);
     debug("seg_flag: %x, granularity: %x\n", line->p, line->g);
     debug("\n");  
     line++;  
   }
}

// creation of a GDT segment 
void create_gdt_segment(int i, uint32_t limit, uint32_t base, uint32_t seg_type, uint32_t desc_type, uint32_t privilege, uint32_t seg_flag, uint32_t granularity, uint32_t longmode)
{
  memset(gdt + i, 0, sizeof(seg_desc_t));
  gdt[i].raw = 0; 
  gdt[i].limit_1 = limit & 0xFFFF; 
  gdt[i].limit_2 = (limit >> 16) & 0x0F; 
  gdt[i].base_1 = base &0xFFFF; 
  gdt[i].base_2 = (base >> 16) &0xFFFF; 
  gdt[i].base_3 = (base >> 24) &0xFF;
  gdt[i].type = seg_type; 
  gdt[i].s = desc_type; 
  gdt[i].dpl = privilege; 
  gdt[i].p = seg_flag; 
  gdt[i].avl = 0; 
  gdt[i].l = longmode; 
  gdt[i].d = granularity; 
  gdt[i].g = granularity; 
}

// create a GDT with 6 entries - 3 R0 - 2 R3 - TSS
void createGDT()
{
   gdt_reg_t default_gdtr ;
   get_gdtr(default_gdtr) ;

   default_gdtr.limit = NBR_SEG * SEG_SIZE - 1 ;
   default_gdtr.desc = gdt ;

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

   memset(&tss_entry, 0, sizeof(seg_desc_t));
   // create TSS segment 
   debug("######################init TSS\n"); 
   tss_entry->s0.esp = get_ebp(); 
   tss_entry->s0.ss = gdt_seg_sel(2, SEG_SEL_KRN);
   

   //raw32_t tss_addr = {.raw = (offset_t) &tss_entry}; 
   create_gdt_segment(5, sizeof(*tss_entry), (uint32_t)tss_entry, SEG_DESC_SYS_TSS_AVL_32, 0, 0, 1, 0, 0);

   
   set_gdtr(default_gdtr) ;
   // initialize segment registers
   debug("set segment selector...\n\n"); 
   debug("test2"); 
   
   set_tr(gdt_seg_sel(5, SEG_SEL_KRN)); 
   // stack segment
   set_ss(gdt_seg_sel(2, SEG_SEL_KRN)); // ss : D0/2
   // data segments
   set_ds(gdt_seg_sel(2, SEG_SEL_KRN)); // ds : D0/2
   set_es(gdt_seg_sel(2, SEG_SEL_KRN)); // es : D0/2
   set_fs(gdt_seg_sel(2, SEG_SEL_KRN)); // fs : D0/2
   set_gs(gdt_seg_sel(2, SEG_SEL_KRN)); // gs : D0/2
   set_cs(gdt_seg_sel(1, SEG_SEL_KRN)); // cs : C0/1
   
   debug("new GDT :\n\n"); 
   print_gdt(); 
}


