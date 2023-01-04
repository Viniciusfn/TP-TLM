#include "ensitlm.h"
#include "generator.h"
#include "bus.h"
#include "memory.h"

int sc_main(int argc, char **argv) {
	(void)argc;
	(void)argv;

	Generator g1("Generator 1");
  Bus bus("Bus");
  Memory m("Memory", 200);

	/* mapping memory at addresses [0x10000000, 0x100000FF]*/
	bus.map(m.target, 0x10000000, 256);

	/* connect components to the bus */
	g1.initiator.bind(bus.target);
	bus.initiator.bind(m.target);

	/* and start simulation */
	sc_core::sc_start();
	return 0;
}
