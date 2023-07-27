#include "etiss/IntegratedLibrary/MemMappedPeriph.h"

#ifndef ETISS_PLUGIN_VANILLAACCELERATOR_H
#define ETISS_PLUGIN_VANILLAACCELERATOR_H

namespace etiss
{

namespace plugin
{


class VanillaAccelerator: public etiss::plugin::MemMappedPeriph
{
    public:

        VanillaAccelerator(uint64_t addr=0x70000000, uint32_t irq_line=20) : regIf{}, base_addr{addr}, irq_line{irq_line} {}

        void write32(uint64_t addr, uint32_t val);

        uint32 read32(uint64_t addr);

        MappedMemory getMappedMem() const {
            MappedMemory mm;
            mm.base = base_addr;
            mm.size = sizeof(RegIF);
            return mm;
        }

    private:

        typedef struct regs 
        {
            uint32_t ifmap;   
            uint32_t weights; 
            uint32_t result;  
            uint32_t oc;      
            uint32_t iw;      
            uint32_t ih;      
            uint32_t ic;      
            uint32_t kh;      
            uint32_t kw;  
            uint32_t control;    // writing 1 starts the execution, writing 0 clears the internal status, but keeps rest of the registers unchanged. 
            uint32_t status;     // read-only! status of 0 indicates 'idle', 1 indicates complition, >2 indicated error, status needs to be cleared, by writing 0 to control register, before it can be newly started, rest of the registers remain unchanged
            
        } regs_t;

        union RegIF
        {
            regs_t regs;
            uint32_t arr[sizeof(regs_t)/sizeof(uint32_t)];
        };
        RegIF regIf;
        
    protected:
        uint64_t base_addr;
        uint32_t irq_line;
        std::string _getPluginName() const;
        void addedToCPUCore(etiss::CPUCore *core); 
        etiss::CPUCore *cpu_core__;

};

} // namespace plugin

} // namespace etiss

#endif