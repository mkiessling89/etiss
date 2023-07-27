#include <stdio.h>
#include "vanilla_acc.h"

#ifndef __riscv__
#define __riscv__
#endif
#include "int.h"
#include "utils.h"

typedef struct regs 
{
	uint32_t ifmap;   
	uint32_t weights;
	uint32_t result;  
	int32_t oc;      
	int32_t iw;      
	int32_t ih;      
	int32_t ic;      
	int32_t kh;      
	int32_t kw; 
    uint32_t control;    // writing 1 starts the execution, writing 0 clears the internal status, but keeps rest of the registers unchanged. 
    volatile uint32_t status;     // read-only! status of 0 indicates 'idle', 1 indicates complition, >2 indicated error, status needs to be cleared, by writing 0 to control register, before it can be newly started, rest of the registers remain unchanged
} regs_t;

static volatile regs_t *p_regs  = (volatile regs_t *)0x70001000;  // set the base address of the peripheral, that would come form some hw ip header. 

uint32_t vanilla_accelerator_conv2dnchw(float* ifmap, float* weights, float* result, int oc, int iw, int ih, int ic,
                        int kh, int kw) 
{
	
	p_regs->control = 0;  // clear internal status of accelerator
	
	p_regs->ifmap   = (uint32_t) ifmap; 
	p_regs->weights = (uint32_t) weights;
	p_regs->result  = (uint32_t) result;  
	
	p_regs->oc = oc;      
	p_regs->iw = iw;      
	p_regs->ih = ih;      
	p_regs->ic = ic;      
	p_regs->kh = kh;      
	p_regs->kw = kw; 
	p_regs->control = 2;  // last command, to start the operation, with 2 instead of 1 the IRQ is raised after completness.

	/* Right now the execution internally is done in an amazing single cycle ;-), so the polling for the status is 'just' an illusion! */
	while( !p_regs->status )
		;

    return p_regs->status;
}

extern void printf_fromisr(const char *format, ...);

void ISR_ACC(void)
{
	unsigned int mie = 0x0;
    csrr(mie, mie);

	unsigned int mip = 0x0;
    csrr(mip, mip);
	printf("inside isr: mie: 0x%08x, mip: 0x%08x\n", mie, mip);

	printf_fromisr("BAEMMMM!!!\n");
	printf_fromisr("     status: 0x%08x\n", p_regs->status );
	
}