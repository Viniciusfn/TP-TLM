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
    ensitlm::data_t rom_data;
    ensitlm::data_t vram_data1;
    ensitlm::data_t vram_data2;
    ensitlm::addr_t vram_addr = VRAM_ADDR;
    ensitlm::addr_t rom_addr = ROM_ADDR;

    // Reading ROM and filling VRAM
    cout << "Filing VRAM with image from ROM..." << endl;
    for (long unsigned int i = 0; i < ( ROM_SIZE / sizeof(ensitlm::data_t) ) ; i++) {
      // Read from ROM
      if (initiator.read(rom_addr, rom_data) != tlm::TLM_OK_RESPONSE) {
        SC_REPORT_ERROR(name(), "bad response status received!");
        abort();
      }      

      // Fix data format
      vram_data1 = 0;
      vram_data2 = 0;
      int half_data = sizeof(ensitlm::data_t) * 4; //in bits
      for (int i = 0; i < half_data; i += 4) {
        vram_data1 += (rom_data & (0xF << i)) << (i+4);
        vram_data2 += (rom_data & (0xF << (i+half_data))) >> half_data << (i+4);
      }

      // Write in VRAM
      if (initiator.write(vram_addr, vram_data1) != tlm::TLM_OK_RESPONSE) {
        SC_REPORT_ERROR(name(), "bad response status received");
        abort();
      }
      vram_addr += sizeof(ensitlm::data_t); // Step to next address
      if (initiator.write(vram_addr, vram_data2) != tlm::TLM_OK_RESPONSE) {
        SC_REPORT_ERROR(name(), "bad response status received");
        abort();
      }

      // Step to next address
      vram_addr += sizeof(ensitlm::data_t); 
      rom_addr  += sizeof(ensitlm::data_t);
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

    // Wait for interruptions
    while(true) {
      wait(interrupt);

      cout << name() << ": Interruption received!" << endl;
    }

  #endif
}

void Generator::itr_detector(void) {
  if(irq_signal) {
    cout << "Notifying interruption..." << endl;
    interrupt.notify();
  }
}

Generator::Generator(sc_core::sc_module_name name) : sc_core::sc_module(name) {
	SC_THREAD(thread);
  sensitive << interrupt;
  SC_METHOD(itr_detector);
  sensitive << irq_signal;
}