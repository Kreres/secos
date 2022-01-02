// ##########
// creation of the GDT with the segments
// ##########

#ifndef SEGMENTATION
#define SEGMENTATION

#include <debug.h>
#include <info.h>
#include <segmem.h>
#include <string.h>



// number of segments in GDT (null, C0, D0, C3, D3, TSS)
#define NBR_SEG 6

// size of a segment 
#define SEG_SIZE sizeof(seg_desc_t)

#define TSS_ADDR 0x900000


// display the current GDT
void print_gdt(); 

// creation of a GDT segment 
void create_gdt_segment(int i, uint32_t limit, uint32_t base, uint32_t seg_type, uint32_t desc_type, uint32_t privilege, uint32_t seg_flag, uint32_t granularity, uint32_t longmode); 

// create a complete GDT with 5 segments
void createGDT(); 

#endif