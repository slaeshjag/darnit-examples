#ifndef __TEXT_H__
#define	__TEXT_H__

typedef struct {
	DARNIT_FONT			*font;
	DARNIT_TEXT_SURFACE		*surface;
} TEXT;

int textInit();
void textLoop();


#endif
