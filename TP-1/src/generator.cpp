#include "generator.h"

using namespace std;

void Generator::thread(void) {
	ensitlm::data_t data = 0;
	ensitlm::addr_t addr = 0x10000000;
	
  cout << endl << "============== WRITING CHECK ==============" << endl;

  // Writing 10 times
  for (ensitlm::data_t i = 0; i < 64; i++) {
    data = i;
		cout << name() << ": sends (" << std::hex << data 
         << ") to address (" << addr << ")" << endl;
    
    if (initiator.write(addr, data) != tlm::TLM_OK_RESPONSE) {
      SC_REPORT_ERROR(name(), "bad response status received");
	    abort();
    }
    
    addr += sizeof(ensitlm::data_t); //steps to next addr
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
    
    // Checks the data value
    if (data != i) {
      SC_REPORT_ERROR(name(), "wrong value on read data!");
	    abort();
    }

    cout << name() << ": reads (" << std::hex << data 
         << ") from address (" << addr << ")" << endl;

    addr += sizeof(ensitlm::data_t); //steps to next addr
	}
}

Generator::Generator(sc_core::sc_module_name name) : sc_core::sc_module(name) {
	SC_THREAD(thread);
}