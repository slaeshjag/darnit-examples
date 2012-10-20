#ifndef __PLAYER_H__
#define	__PLAYER_H__


#define	PLAYER_SPEED	0.15f

typedef struct {
	int			x;
	int			y;
	int			dir;
	DARNIT_SPRITE		*sprite;
} PLAYER;

int playerInit();
void playerDraw();
void playerCamera();


#endif
