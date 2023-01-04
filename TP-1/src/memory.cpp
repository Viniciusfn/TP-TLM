#include "memory.h"

using namespace std;

tlm::tlm_response_status Memory::write(const ensitlm::addr_t &a,
                                       const ensitlm::data_t &d) {  
  // Checks address validity
  if (a >= size) {
    SC_REPORT_ERROR(name(), "invalid address!");
	  abort();
  }
  
  // Storing the data
  storage[a / sizeof(ensitlm::data_t)] = d;

  cout << name() << ": writes (" << storage[a / sizeof(ensitlm::data_t)] 
       << ") at address (" << a << ")" << endl;
	return tlm::TLM_OK_RESPONSE;
}

tlm::tlm_response_status Memory::read(const ensitlm::addr_t &a,
                                      /* */ ensitlm::data_t &d) {
	// Reading the data from given address
  d = storage[a / sizeof(ensitlm::data_t)];
  
  cout << name() << ": returns (" << d << ") from address ("
       << a << ")" << endl;
  return tlm::TLM_OK_RESPONSE;
}

Memory::Memory(sc_core::sc_module_name name, size_t size) : sc_core::sc_module(name), size(size) {
  // Storage allocation ('size' in bytes)
  storage = new ensitlm::data_t [size / sizeof(ensitlm::data_t)];
}

Memory::~Memory() {
  delete [] storage;
}