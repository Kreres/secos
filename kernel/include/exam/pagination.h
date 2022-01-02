// ##########
// Pagination
// ##########

#ifndef PAGINATION
#define PAGINATION

#include <debug.h>
#include <cr.h>
#include <pagemem.h>
#include <info.h>

// define commonly used address
#define PGD_SIZE 0x01000

#define PAGE_KERNEL 0x20000

#define PAGE_USER (PGD_SIZE + PAGE_KERNEL)

#define PGD_1_ADDR 0x60000
#define PGD_2_ADDR 0x70000

#define PTB_1_ADDR 0x1300000
#define PTB_2_ADDR 0x800000

void enable_paging(); 

void print_pages(pde32_t * pgd, uint32_t nbr_pgd, uint32_t nbr_pde); 

void pagination_kernel(); 
void pagination_user();

void pagination_SUP(pde32_t * pgd, uint32_t phy_addr, uint32_t virt_addr, int attribute); 

void pages_for_all();

#endif