#ifndef __STICK_H__
#define	__STICK_H__

typedef struct {
	int			x;
	int			y;
	int			count;
	DARNIT_TILE		*tile;
	DARNIT_TILESHEET	*ts;
} STICK;


void stickMove();
void stickLoop();
int stickInit();


#endif
