// Configuration file containing main parameters

#ifndef CONF_H
#define CONF_H

#include "LCDC_registermap.h"

#define VRAM_ADDR 0x10000000
#define MEM_SIZE 87040
#define LCDC_ADDR 0x20000000
#define ROM_ADDR  0x30000000
#define ROM_SIZE (320 * 240 / 2)

//#define PHASE_1
#define PHASE_2

#endif