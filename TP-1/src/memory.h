#include "ensitlm.h"
#include "conf.h"

struct Memory : sc_core::sc_module {
	ensitlm::target_socket<Memory> target;
  ensitlm::data_t* storage;
  size_t size;

	tlm::tlm_response_status write(const ensitlm::addr_t &a,
	                               const ensitlm::data_t &d);

	tlm::tlm_response_status read(const ensitlm::addr_t &a,
	                              /* */ ensitlm::data_t &d);

	Memory(sc_core::sc_module_name, size_t);
  ~Memory();
};
