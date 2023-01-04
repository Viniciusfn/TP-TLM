#include "ensitlm.h"

struct Generator : sc_core::sc_module {
	ensitlm::initiator_socket<Generator> initiator;
  sc_core::sc_signal<bool> display_intr;

	void thread(void);

	SC_CTOR(Generator);
};
