#include <darnit/darnit.h>
#include <stdlib.h>
#include <stdio.h>

#include "collision.h"

#define	ROTATE_COORD_X(angle, x, y)		(((d_util_sin((angle) + 900) * (x)) - (d_util_sin((angle)) * (y))) >> 16)
#define	ROTATE_COORD_Y(angle, x, y)		(((d_util_sin((angle)) * (x)) + (d_util_sin((angle + 900)) * (y))) >> 16)


typedef struct {
	int		w;
	int		h;
	int		a_y;
} MAIN;


typedef struct {
	int		lines;
	int		*coordinate;
	int		spawn;
} SHAPE;


typedef struct {
	SHAPE		*asteroid;
	DARNIT_LINE	*line;
	int		*coords;
	int		min;
	int		max;
	int		pos_x;
	int		pos_y;
	int		angle;
	int		velocity_x;
	int		velocity_y;
	int		angle_velocity;
} ASTEROID;


typedef struct {
	SHAPE		*ship;
	DARNIT_LINE	*line;
	int		*coords;
	int		min;
	int		max;
	int		x;
	int		y;
	int		angle;
	int		velocity;
} SHIP;


MAIN m;


ASTEROID *asteroid_new(int x, int y, SHAPE *shape) {
	ASTEROID *a;

	if (!(a = malloc(sizeof(ASTEROID))))
		return NULL;
	a->asteroid = shape;
	a->pos_x = x;
	a->pos_y = y;
	a->angle = rand() % 3600;
	a->velocity = rand() % ASTEROID_VELOCITY_MAX;
	a->coords = malloc((shape->lines + 1) * 2 * sizeof(int));
	a->angle_velocity = rand() % ASTEROID_ANGLE_VELOCITY_MAX;
	a->line = d_render_line_new(shape->lines);
	coordinate_rotate(a->line, a->coords, a->shape->lines, a->shape->coordinate, a->angle);


	return a;
}


ASTEROID *asteroid_delete(ASTEROID *a) {
	d_render_line_delete(a->line);
	free(a->coords);
	free(a);

	return NULL;
}


SHIP *ship_new(int x, int y, SHAPE *ship_shape) {
	SHIP *ship;

	if (!(ship = malloc(sizeof(SHIP))))
		return NULL;
	ship->shape = ship_shape;
	ship->x = x;
	ship->y = y;
	ship->angle = 900;
	ship->velocity_x = 0;
	ship->velocity_y = 0;
	ship->coords = malloc((ship_shape->lines + 1) * 2 * sizeof(int));
	ship->line = d_render_line_new(ship_shape->lines);
	coordinate_rotate(ship->line, ship->coords, ship->shape->coordinate, ship->angle);

	return ship;
}


SHIP *ship_delete(SHIP *ship) {
	d_render_line_free(ship->line);
	free(ship->coords);
	free(ship);

	return NULL;
}


void ship_loop(SHIP *ship) {
	if (d_keys_get().left)
		ship->angle += SHIP_ANGLE_SPEED;
	if (d_keys_get().right)
		ship->angle -= SHIP_ANGLE_SPEED;
	return;
}


SHAPE *shape_load(DARNIT_STRINGTABLE *st, const char *name) {
	SHAPE *shape;

	d_stringtable_section_load(st, name);
	if (atoi(d_stringtable_entry(st, "LINES")) <= 0) {
		d_stringtable_section_unload(st, name);
		return NULL;
	}

	if (!(shape = malloc(sizeof(SHAPE))))
		return NULL;
	
	shape->lines = atoi(d_stringtable_entry(st, "LINES"));
	shape->coordinate = malloc(sizeof(int) * 2 * shape->lines + 2);
	d_util_string_to_int_array(d_stringtable_entry(st, "COORDINATES"), " ", shape->coordinate, shape->lines * 2 + 2);
	d_stringtable_section_unload(st, name);

	return shape;
}


void coordinate_rotate(DARNIT_LINE *line, int *target_coord, int lines, int *coordinate, int angle, int *min, int *max) {
	int i, x, y, x2, y2, min, max;

	for (i = 0; i < lines; i++) {
		x = coordinate[i * 2];
		y = coordinate[i * 2 + 1];
		x2 = coordinate[i * 2 + 2];
		y2 = coordinate[i * 2 + 3];
		target_coord[i * 2] = ROTATE_COORD_X(angle, x, y);
		target_coord[i * 2 + 1] = ROTATE_COORD_Y(angle, x, y);
		target_coord[i * 2 + 2] = ROTATE_COORD_X(angle, x2, y2);
		target_coord[i * 2 + 3] = ROTATE_COORD_Y(angle, x2, y2);
		d_render_line_move(line, i, ROTATE_COORD_X(angle, x, y), ROTATE_COORD_Y(angle, x, y),
			ROTATE_COORD_X(angle, x2, y2), ROTATE_COORD_Y(angle, x2, y2));
	}

	min = max = 0;
	for (i = 0; i < lines + 1; i++) {
		max = (target_coord[i * 2] > max) ? target_coord[i * 2] : max;
		max = (target_coord[i * 2 + 1] > max) ? target_coord[i * 2 + 1] : max;
		min = (target_coord[i * 2] < min) ? target_coord[i * 2] : min;
		min = (target_coord[i * 2 + 1] < min) ? target_coord[i * 2 + 1] : min;
	}

	*min = min;
	*max = max;

	return;
}


void render_line_wrap(SHAPE *shape, DARNIT_LINES *line, int x, int y) {
	if (shape->min + x < 0) {
	
		d_render_line_draw(line, shape->lines);
}


int init() {
	d_init("Asteroids", "asteroids", NULL);
	d_fs_mount_self();

	m.w = 800;
	m.h = 480;
	m.a_y = 80;

	return 1;
}


int main(int argc, char **argv) {
	DARNIT_LINE *ship;
	SHIP *ship_o;
	DARNIT_STRINGTABLE *shipdata;
	SHAPE *ship_c;
	int i;

	if (!init())
		return -1;
	shipdata = d_stringtable_open("data/shipdata.stz");
	ship_c = shape_load(shipdata, "large");
	ship_o = ship_new(100, 100);
	
	ship = d_render_line_new(ship_c->lines, 1);

	for (i = 0;; i++) {
		ship_loop(ship_o);
		coordinate_rotate(ship, ship_c->coords, ship_c->lines, ship_c->coordinate, i * 10);
		d_render_begin();
		d_render_offset(-256, -128);
		d_render_line_draw(ship, ship_c->lines);
		d_render_end();
		d_loop();
	}

	d_quit();
	return 0;

