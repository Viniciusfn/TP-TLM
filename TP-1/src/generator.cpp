#include "generator.h"

using namespace std;

void Generator::thread(void) {
  #ifdef PHASE_1
    ensitlm::data_t data = 0;
    ensitlm::addr_t addr = 0x10000000;
    
    cout << endl << "============== WRITING CHECK ==============" << endl;

    // Writing 10 times
    for (ensitlm::data_t i = 0; i < 64; i++) {
      data = i;
      cout << name() << ": sends (" << std::hex << data 
          << ") to address (" << addr << ")" << endl;
      
      // Write and check reponse
      if (initiator.write(addr, data) != tlm::TLM_OK_RESPONSE) {
        SC_REPORT_ERROR(name(), "bad response status received");
        abort();
      }
      
      addr += sizeof(ensitlm::data_t); //step to next address
    }

    cout << endl << "============== READING CHECK ==============" << endl;

    // Reading 10 times and checking
    addr = 0x10000000;
    for (ensitlm::data_t i = 0; i < 64; i++) {
      // Reads and check response
      if (initiator.read(addr, data) != tlm::TLM_OK_RESPONSE) {
        SC_REPORT_ERROR(name(), "bad response status received!");
        abort();
      }
      
      // Check the data value
      if (data != i) {
        SC_REPORT_ERROR(name(), "wrong value on read data!");
        abort();
      }

      cout << name() << ": reads (" << std::hex << data 
          << ") from address (" << addr << ")" << endl;

      addr += sizeof(ensitlm::data_t); //step to next addr
  	}
  #endif

  #ifdef PHASE_2
    ensitlm::data_t data = 0xFFFFFFFF;
    ensitlm::addr_t addr = VRAM_ADDR;

    // Filling VRAM
    cout << "Filing VRAM with white image..." << endl;
    for (long unsigned int i = 0; i < ( MEM_SIZE / sizeof(ensitlm::data_t) ) ; i++) {
      // Write and check reponse
      if (initiator.write(addr, data) != tlm::TLM_OK_RESPONSE) {
        SC_REPORT_ERROR(name(), "bad response status received");
        abort();
      }

      addr += sizeof(ensitlm::data_t); //step to next addr
    }

    // Telling LCD controller the VRAM address
    if (initiator.write(LCDC_ADDR+LCDC_ADDR_REG, VRAM_ADDR) != tlm::TLM_OK_RESPONSE) {
      SC_REPORT_ERROR(name(), "bad response status received");
      abort();
    }

    // Start LCD controller
    if (initiator.write(LCDC_ADDR+LCDC_START_REG, 0x00000001) != tlm::TLM_OK_RESPONSE) {
      SC_REPORT_ERROR(name(), "bad response status received");
      abort();
    }

  #endif
}

Generator::Generator(sc_core::sc_module_name name) : sc_core::sc_module(name) {
	SC_THREAD(thread);
}