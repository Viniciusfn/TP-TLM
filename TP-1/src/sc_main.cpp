#include "ensitlm.h"
#include "generator.h"
#include "bus.h"

int sc_main(int argc, char **argv) {
	(void)argc;
	(void)argv;

	Generator g("CPU");
  Bus bus("Bus");

	/* connect components to the bus */
	g.initiator.bind(bus.target);

	/* and start simulation */
	sc_core::sc_start();
	return 0;
}
