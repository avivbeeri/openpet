#include <stdint.h>
#include <stdbool.h>
#include "vpet.h"

VPET_species species[1] = {
  { .id = 0, .name = "", .min_weight = 5, .type = TYPE_DATA }
};

void VPET_feed(VPET *vpet) {
  vpet->hunger++;
  if (vpet->hunger > 4) {
    vpet->hunger = 4;
    if (vpet->overfed == false) {
      vpet->overfeed++;
      vpet->overfed = true;
    }
  }
}

void VPET_flush(VPET *vpet) {
  vpet->poop = 0;
}

