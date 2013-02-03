#include "sticks.h"


int textInit() {
	if ((s.text.font = d_font_load("IDM_minimal.ttf", 24, 512, 512)) == NULL) {
		return -1;
	}

	s.text.surface = d_text_surface_new(s.text.font, 128, 800, 0, 0);
	
	return 0;
}


void textLoop() {
	char buff[64];

	sprintf(buff, "Sticks: %i", s.stick.count);
	d_text_surface_reset(s.text.surface);
	d_text_surface_string_append(s.text.surface, buff);
	d_text_surface_draw(s.text.surface);

	return;
}
