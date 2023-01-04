#include "generator.h"

using namespace std;

void Generator::thread(void) {
	ensitlm::data_t data = 0;
	ensitlm::addr_t addr = 0x10000000;
	for (ensitlm::data_t i = 0; i < 10; i++) {
    data = i;
		cout << name() << ": sends (" << std::hex << data 
         << ") to address (" << addr << ")" << endl;
    
    if (initiator.write(addr, data) != tlm::TLM_OK_RESPONSE) {
      SC_REPORT_ERROR(name(), "bad response status received");
	    abort();
    }
    
    addr += sizeof(ensitlm::data_t);
	}
}

Generator::Generator(sc_core::sc_module_name name) : sc_core::sc_module(name) {
	SC_THREAD(thread);
}
