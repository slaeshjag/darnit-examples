#include "sticks.h"

int mapInit() {
	if ((s.map.handle = darnitMapLoad("map.ldmz")) == NULL) {
		fprintf(stderr, "Unable to load map.ldmz\n");
		return -1;
	}

	return 0;
}


void mapDraw() {
	darnitRenderTilemap(s.map.handle->layer->tilemap);

	return;
}
