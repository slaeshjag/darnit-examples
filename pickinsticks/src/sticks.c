#include "sticks.h"


int init() {
	darnitFSMount("gfx.ldi");
	
	if (playerInit() == -1);
	else if (mapInit() == -1);
	else if (stickInit() == -1);
	else if (textInit() == -1);
	else return 0;

	return -1;
}


int main(int argc, char **argv) {
	if (darnitInit("darnitSticks!", "sticks", NULL) == NULL) {
		fprintf(stderr, "libDarnit failed to init. Lets just give up...\n");
		return -1;
	}

	if (init() == -1)
		darnitQuit();

	for (;;) {
		s.keys = darnitButtonGet();
		darnitRenderBegin();
		
		playerCamera();
		mapDraw();
		darnitRenderBlendingEnable();
		stickLoop();
		playerDraw();
		darnitRenderOffset(0, 0);
		textLoop();
		darnitRenderBlendingDisable();

		darnitRenderEnd();
		darnitLoop();
	}

	return 0;
}
