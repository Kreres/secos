// ##########
// Pagination
// ##########

#include <exam/pagination.h>

#define virtual_addr(__pgd_id__, __ptb_id__) ((__pgd_id__ << PG_4M_SHIFT) + (__ptb_id__ << PG_4K_SHIFT))

extern info_t *info;

// display CR3 value
void show_cr3()
{
   cr3_reg_t cr3 = {.raw = get_cr3()};
   debug("CR3 = %p\n", cr3.raw);
}

void enable_paging()
{
   uint32_t cr0 = get_cr0();
   set_cr0(cr0|CR0_PG);
}

//affichage des pages 
void print_pages(pde32_t * pgd, uint32_t nbr_pgd, uint32_t nbr_pde)
{
	for(uint32_t k = 0; k < nbr_pgd; k++)
	{
		debug("PDE n°%d\n", k); 
		pte32_t * pte = (pte32_t *)page_addr(pgd[k].addr); 
		for(uint32_t l = 0; l < nbr_pde; l++)
		{
			debug("ID : 0x%6d, PHY @ : 0x%6x, VIRT @ : 0x%x, lvl : %x\n", l, page_addr(pte[l].addr), virtual_addr(k,l), pte[l].lvl);
		}		
	}
}

// kernel pages - pgd_id 0
void pagination_kernel()
{
	pde32_t * pgdK = (pde32_t*)0x20000; 
	pte32_t * ptbK = (pte32_t *)0x400000; 

	for(int j = 0; j < 1024; j++)
	{
	// 1024 entries in ptb
		for(int i = 0; i < 1024; i++)
		{
			pg_set_entry(&ptbK[i] + j*1024, PG_KRN|PG_RW, i+(j<<10));
		}
		memset((void*)&pgdK[j], 0, PAGE_SIZE);
		pg_set_entry(&pgdK[j], PG_KRN|PG_RW, page_nr(&ptbK[j*1024]));
	}
	//print_pages(pgdK, 3, 10); 
	debug("kernel pages done \n");
	set_cr3(pgdK); 
}

void pagination_user()
{
	// mettre une entrée en droit KRN - 
	pde32_t * pgd1 = (pde32_t*)PGD_1_ADDR; 
	pte32_t * ptb1 = (pte32_t *)PTB_1_ADDR; 
	
	pde32_t * pgd2 = (pde32_t*)PGD_2_ADDR; 
	pte32_t * ptb2 = (pte32_t *)PTB_2_ADDR;

	for(int j = 0; j < 1024; j++)
	{
	// 1024 entries in ptb
		for(int i = 0; i < 1024; i++)
		{
			pg_set_entry(&ptb1[i] + j*1024, PG_USR|PG_RW, i+(j<<10));
			pg_set_entry(&ptb2[i] + j*1024, PG_USR|PG_RW, i+(j<<10));
		}
		memset((void*)&pgd1[j], 0, PAGE_SIZE);
		memset((void*)&pgd2[j], 0, PAGE_SIZE);
		
		pg_set_entry(&pgd1[j], PG_USR|PG_RW, page_nr(&ptb1[j*1024]));
		pg_set_entry(&pgd2[j], PG_USR|PG_RW, page_nr(&ptb2[j*1024]));
	}
	pg_set_entry(&ptb1[0], PG_KRN|PG_RW, 0);
	pg_set_entry(&ptb2[0], PG_KRN|PG_RW, 0);
	
	//print_pages(pgd1, 2, 1024); 
	//print_pages(pgd2, 3, 10); 
	debug("user pages done \n");
}

void pagination_SUP(pde32_t * pgd, uint32_t phy_addr, uint32_t virt_addr, int attribute)
{
	uint32_t pgd_idx = pd32_idx(virt_addr); 
	uint32_t ptb_idx = pt32_idx(virt_addr);
	
	pte32_t * ptb_tmp = (pte32_t*)page_addr(pgd[pgd_idx].addr); 
	
	pg_set_entry(&ptb_tmp[ptb_idx], attribute, page_nr(phy_addr)); 
	//debug("ajout task :INDEX %d, ADDR VIRT 0x%x\n", pgd[0].raw, virt_addr); 
	
}


void pages_for_all()
{
	// kernel
	pagination_kernel();
	// user1()
	pagination_user(); 
	
}

// MEMOIRE PARTAGEE ?????