#include "memory.h"

using namespace std;

tlm::tlm_response_status Memory::write(const ensitlm::addr_t &a,
                                       const ensitlm::data_t &d) {  
  // Checks address validity
  if (a >= size) {
    SC_REPORT_ERROR(name(), "invalid access!");
    abort();
  }
  
  // Storing the data
  storage[a / sizeof(ensitlm::data_t)] = d;

  cout << name() << ": WRITE OP : address=" << a << " : data=" << d << endl;
	return tlm::TLM_OK_RESPONSE;
}

tlm::tlm_response_status Memory::read(const ensitlm::addr_t &a,
                                      /* */ ensitlm::data_t &d) {
	// Reading the data from given address
  d = storage[a / sizeof(ensitlm::data_t)];
  
  cout << name() << ": READ OP : address=" << a << " : data=" << d << endl;
  return tlm::TLM_OK_RESPONSE;
}

Memory::Memory(sc_core::sc_module_name name, size_t size) : sc_core::sc_module(name), size(size) {
  // Storage allocation ('size' in bytes)
  storage = new ensitlm::data_t [size / sizeof(ensitlm::data_t)];
}

Memory::~Memory() {
  delete [] storage;
}