#ifndef __PONG_H__
#define	__PONG_H__

#include <darnit/darnit.h>
#include <stdio.h>
#include <stdlib.h>

#include "player.h"

typedef struct {
	int			x;
	int			y;
} PLAYER;


typedef struct {
	int			x;
	int			y;
	int			vel;
	int			angle;
} BALL;


typedef struct {
	DARNIT_TILESHEET	*player_ts;
	DARNIT_TILESHEET	*ball_ts;
	PLAYER			player;
	BALL			ball;
} PONG;

PONG p;


#endif
