#include "memory.h"

using namespace std;

tlm::tlm_response_status Memory::write(const ensitlm::addr_t &a,
                                       const ensitlm::data_t &d) {
	cout << name() << ": writes (" << d << ") at address ("
       << a << ")" << endl;
	return tlm::TLM_OK_RESPONSE;
}

tlm::tlm_response_status Memory::read(const ensitlm::addr_t &a,
                                      /* */ ensitlm::data_t &d) {
	cout << name() << ": read (" << d << ") from address ("
       << a << ")" << endl;
  
  SC_REPORT_ERROR("TLM", "not implemented");
  abort();
}
