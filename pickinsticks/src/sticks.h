#ifndef __STICKS_H__
#define	__STICKS_H__

#include <darnit/darnit.h>

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "player.h"
#include "map.h"
#include "stick.h"
#include "text.h"

typedef struct {
	PLAYER			player;
	MAP			map;
	DARNIT_KEYS		keys;
	STICK			stick;
	TEXT			text;
} STICKS;


STICKS s;


#endif
