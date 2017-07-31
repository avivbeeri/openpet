#include <stdint.h>
#include <stdbool.h>

#ifndef VPET_H
#define VPET_H

enum VPET_type { TYPE_VACCINE, TYPE_DATA, TYPE_VIRUS };

typedef uint8_t uint_t;

typedef struct VPET_species_t {
  uint_t id;
  char* name;
  uint_t min_weight;
  enum VPET_type type;
} VPET_species;

typedef struct VPET_data {
  uint_t age;
  uint_t weight;
  uint_t hunger;
  uint_t strength;
  // DP = ENERGY
  uint_t dp;
  uint_t battles_fought;
  uint_t battles_won;
  uint_t progress;
  uint_t mistakes;
  uint_t overfeed;
  bool overfed;
  uint_t poop;
  VPET_species* species;
} VPET;

extern VPET_species species[];
void VPET_feed(VPET *vpet);
void VPET_flush(VPET *vpet);

#endif /* VPET_H */
