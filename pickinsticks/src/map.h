#ifndef __MAP_H__
#define	__MAP_H__

#define	MAP_W	(s.map.handle->layer->tilemap->w * s.map.handle->layer->tile_w)
#define	MAP_H	(s.map.handle->layer->tilemap->h * s.map.handle->layer->tile_h)


typedef struct {
	DARNIT_MAP		*handle;
} MAP;

int mapInit();
void mapDraw();


#endif
