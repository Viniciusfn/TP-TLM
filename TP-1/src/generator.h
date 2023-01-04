#include "ensitlm.h"
#include "conf.h"

using namespace sc_core;

struct Generator : sc_core::sc_module {
	ensitlm::initiator_socket<Generator> initiator;
  sc_signal <bool , SC_MANY_WRITERS> irq_signal;
	sc_event interrupt;

	void thread(void);
	void itr_detector(void);

	SC_CTOR(Generator);
};
