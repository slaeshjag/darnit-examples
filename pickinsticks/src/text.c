#include "sticks.h"


int textInit() {
	if ((s.text.font = darnitFontLoad("IDM_minimal.ttf", 24, 512, 512)) == NULL) {
		return -1;
	}

	s.text.surface = darnitTextSurfaceAlloc(s.text.font, 128, 800, 0, 0);
	
	return 0;
}


void textLoop() {
	char buff[64];

	sprintf(buff, "Sticks: %i", s.stick.count);
	darnitTextSurfaceReset(s.text.surface);
	darnitTextSurfaceStringAppend(s.text.surface, buff);
	darnitTextSurfaceDraw(s.text.surface);

	return;
}
