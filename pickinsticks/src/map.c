#include "sticks.h"

int mapInit() {
	if ((s.map.handle = d_map_load("map.ldmz")) == NULL) {
		fprintf(stderr, "Unable to load map.ldmz\n");
		return -1;
	}

	return 0;
}


void mapDraw() {
	d_tilemap_draw(s.map.handle->layer->tilemap);

	return;
}
