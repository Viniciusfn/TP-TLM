#include "ensitlm.h"
#include "generator.h"
#include "bus.h"
#include "memory.h"
#include "LCDC.h"


int sc_main(int argc, char **argv) {
	(void)argc;
	(void)argv;

	Generator g("Generator 1");
  Bus bus("Bus");
  Memory m("Memory", 87040);

	LCDC lcdc("LCDC", sc_core::sc_time(1.0 / 25, sc_core::SC_SEC));

	/* mapping targets */
	bus.map(m.target, 0x10000000, 87040);
	bus.map(lcdc.target_socket, 0x20000000, 12);

	/* connect components to the bus */
	g.initiator.bind(bus.target);
	lcdc.initiator_socket.bind(bus.target);
	bus.initiator.bind(m.target);
	bus.initiator.bind(lcdc.target_socket);

	/* display interruption */
	lcdc.display_intr.bind(g.display_intr);

	/* and start simulation */
	sc_core::sc_start();
	return 0;
}
