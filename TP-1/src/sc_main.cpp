#include "ensitlm.h"
#include "conf.h"
#include "generator.h"
#include "bus.h"
#include "memory.h"
#include "LCDC.h"
#include "ROM.h"


int sc_main(int argc, char **argv) {
	(void)argc;
	(void)argv;

	Generator g("Generator 1");
  Bus bus("Bus");
  Memory vram("Memory", MEM_SIZE);
	ROM rom("ROM");

	LCDC lcdc("LCDC", sc_core::sc_time(1.0 / 25, sc_core::SC_SEC));

	/* mapping targets */
	bus.map(vram.target, VRAM_ADDR, MEM_SIZE);
	bus.map(lcdc.target_socket, LCDC_ADDR, 12); // 3 registers (4B each)
	bus.map(rom.socket, ROM_ADDR, ROM_SIZE);

	/* connect components to the bus */
	g.initiator.bind(bus.target);
	lcdc.initiator_socket.bind(bus.target);
	bus.initiator.bind(vram.target);
	bus.initiator.bind(lcdc.target_socket);
	bus.initiator.bind(rom.socket);

	/* connect display interruption */
	lcdc.display_intr.bind(g.irq_signal);

	/* and start simulation */
	sc_core::sc_start();
	return 0;
}
