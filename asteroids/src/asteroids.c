#include <darnit/darnit.h>
#include <stdlib.h>
#include <stdio.h>

#include "collision.h"

#define	ROTATE_COORD_X(angle, x, y)		(((d_util_sin((angle) + 900) * (x)) - (d_util_sin((angle)) * (y))) >> 16)
#define	ROTATE_COORD_Y(angle, x, y)		(((d_util_sin((angle)) * (x)) + (d_util_sin((angle + 900)) * (y))) >> 16)

#define	ASTEROID_VELOCITY_MAX			100
#define	ASTEROID_ANGLE_VELOCITY_MAX		5
#define	SHIP_ANGLE_SPEED			3
#define	SHIP_VELOCITY_MAX			200
#define	SHIP_VELOCITY_ACCELERATE		5
#define	BULLET_LIFE				3000
#define	BULLET_LENGTH				10
#define	BULLET_SPEED				20000


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


typedef struct BULLET {
	int		x;
	int		y;
	int		time;
	int		vel_x;
	int		vel_y;
	int		angle;
	DARNIT_LINE	*line;
	struct BULLET	*next;
} BULLET;


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
	BULLET		*bullet;
	int		*coords;
	int		min;
	int		max;
	int		x;
	int		y;
	int		angle;
	int		velocity_x;
	int		velocity_y;
} SHIP;


MAIN m;
void coordinate_rotate(DARNIT_LINE *line, int *target_coord, int lines, int *coordinate, int angle, int *min, int *max);
void render_line_wrap(DARNIT_LINE *line, int x, int y, int max, int min);

void bullet_remove(BULLET **list, BULLET *remove) {
	BULLET **parent, *tmp;
	
	for (parent = list; *parent; parent = (void *) &((*parent)->next)) {
		if (*parent == remove) {
			tmp = *parent;
			*parent = (*parent)->next;
			d_render_line_free(tmp->line);
			free(remove);
			return;
		}
	}

	return;
}


int bullet_update(BULLET *bullet) {
	int x1, y1, x2, y2;
	
	bullet->x += bullet->vel_x;
	bullet->y += bullet->vel_y;
	bullet->time += d_last_frame_time();
	if (bullet->time >= BULLET_LIFE)
		return 0;

	if (bullet->x < 0)
		bullet->x += 800000;
	if (bullet->x >= 800000)
		bullet->x -= 800000;
	if (bullet->y < 0)
		bullet->y += 480000;
	if (bullet->y >= 480000)
		bullet->y -= 480000;
	
	x1 = (d_util_sin(bullet->angle + 900) * BULLET_LENGTH) >> 16;
	y1 = (d_util_sin(bullet->angle) * BULLET_LENGTH) >> 16;
	x2 = (d_util_sin(bullet->angle + 900) * BULLET_LENGTH) >> 16;
	y2 = (d_util_sin(bullet->angle) * BULLET_LENGTH) >> 16;
	x1 *= -1;
	y1 *= -1;
	d_render_line_move(bullet->line, 0, x1, y1, x2, y2);

	render_line_wrap(bullet->line, bullet->x / 1000, bullet->y / 1000, 15, -15);
	return 1;
}


void bullet_add(BULLET **list, int x, int y, int angle) {
	BULLET *new = malloc(sizeof(BULLET));

	new->x = x;
	new->y = y;
	new->time = 0;
	new->vel_x = (d_util_sin(900 + angle) * BULLET_SPEED) >> 16;
	new->vel_y = (d_util_sin(angle) * BULLET_SPEED) >> 16;
	new->angle = angle;
	new->line = d_render_line_new(1, 1);
	new->next = *list;
	bullet_update(new);
	*list = new;
	
	return;
}



ASTEROID *asteroid_new(int x, int y, SHAPE *shape) {
	ASTEROID *a;

	if (!(a = malloc(sizeof(ASTEROID))))
		return NULL;
	a->asteroid = shape;
	a->pos_x = x;
	a->pos_y = y;
	a->angle = rand() % 3600;
	a->velocity_x = rand() % ASTEROID_VELOCITY_MAX;
	a->velocity_y = rand() % ASTEROID_VELOCITY_MAX;
	a->coords = malloc((shape->lines + 1) * 2 * sizeof(int));
	a->angle_velocity = rand() % ASTEROID_ANGLE_VELOCITY_MAX;
	a->line = d_render_line_new(shape->lines, 1);
	coordinate_rotate(a->line, a->coords, a->asteroid->lines, a->asteroid->coordinate, a->angle, &a->min, &a->max);


	return a;
}


ASTEROID *asteroid_delete(ASTEROID *a) {
	d_render_line_free(a->line);
	free(a->coords);
	free(a);

	return NULL;
}


SHIP *ship_new(int x, int y, SHAPE *ship_shape) {
	SHIP *ship;

	if (!(ship = malloc(sizeof(SHIP))))
		return NULL;
	ship->ship = ship_shape;
	ship->x = x;
	ship->y = y;
	ship->angle = 900;
	ship->velocity_x = 0;
	ship->velocity_y = 0;
	ship->coords = malloc((ship_shape->lines + 1) * 2 * sizeof(int));
	ship->line = d_render_line_new(ship_shape->lines, 1);
	ship->bullet = NULL;
	coordinate_rotate(ship->line, ship->coords, ship->ship->lines, ship->ship->coordinate, ship->angle, &ship->min, &ship->max);

	return ship;
}


void ship_spawn_bullet(SHIP *ship) {
	int spawn_x, spawn_y;

	spawn_x = ((d_util_sin(ship->angle) * 24) >> 16) * 1000;
	spawn_y = ((d_util_sin(ship->angle - 900) * 16) >> 16) * 1000;
	spawn_x += ship->x;
	spawn_y += ship->y;
	bullet_add(&ship->bullet, spawn_x, spawn_y, ship->angle - 900);
	return;
}


SHIP *ship_delete(SHIP *ship) {
	d_render_line_free(ship->line);
	free(ship->coords);
	free(ship);

	return NULL;
}


void ship_loop(SHIP *ship) {
	BULLET **bullet;
	DARNIT_KEYS keys;

	if (d_keys_get().left)
		ship->angle += SHIP_ANGLE_SPEED * d_last_frame_time();
	if (d_keys_get().right)
		ship->angle -= SHIP_ANGLE_SPEED * d_last_frame_time();
	if (d_keys_get().up) {
		ship->velocity_x += (d_util_sin(ship->angle) * SHIP_VELOCITY_ACCELERATE) >> 16;
		ship->velocity_y += (d_util_sin(ship->angle - 900) * SHIP_VELOCITY_ACCELERATE) >> 16;
	} else if (d_keys_get().down) {
		ship->velocity_x -= (d_util_sin(ship->angle) * SHIP_VELOCITY_ACCELERATE) >> 16;
		ship->velocity_y -= (d_util_sin(ship->angle - 900) * SHIP_VELOCITY_ACCELERATE) >> 16;
	}

	if (d_keys_get().l) {
		keys = d_keys_zero();
		keys.l = 1;
		d_keys_set(keys);
		ship_spawn_bullet(ship);
	}

	if (ship->velocity_x > SHIP_VELOCITY_MAX) ship->velocity_x = SHIP_VELOCITY_MAX;
	if (ship->velocity_y > SHIP_VELOCITY_MAX) ship->velocity_y = SHIP_VELOCITY_MAX;
	if (ship->velocity_x < -SHIP_VELOCITY_MAX) ship->velocity_x = -SHIP_VELOCITY_MAX;
	if (ship->velocity_y < -SHIP_VELOCITY_MAX) ship->velocity_y = -SHIP_VELOCITY_MAX;

	ship->x += ship->velocity_x * d_last_frame_time();
	ship->y += ship->velocity_y * d_last_frame_time();

	for (bullet = &ship->bullet; *bullet; bullet = &(*bullet)->next) {
		if (!bullet_update(*bullet))
			bullet_remove(&ship->bullet, *bullet);
		if (!(*bullet))
			break;
	}

	if (ship->x < 0)
		ship->x += 800000;
	if (ship->x >= 800000)
		ship->x -= 800000;
	if (ship->y < 0)
		ship->y += 480000;
	if (ship->y >= 480000)
		ship->y -= 480000;
	
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


void coordinate_rotate(DARNIT_LINE *line, int *target_coord, int lines, int *coordinate, int angle, int *min_t, int *max_t) {
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

	*min_t = min;
	*max_t = max;

	return;
}


void get_wrap(int x, int y, int max, int min, int *w_x, int *w_y) {
	int e_x, e_y;

	e_x = x;
	e_y = y;
	
	if (min + x < 0)
		e_x = 800 + x;
	if (max + x >= 800)
		e_x = x - 800;
	if (min + y < 0)
		e_y = 480 + y;
	if (max + y >= 480)
		e_y = y - 480;
	
	*w_x = e_x;
	*w_y = e_y;

}


void render_line_wrap(DARNIT_LINE *line, int x, int y, int max, int min) {
	int e_x, e_y;

	get_wrap(x, y, max, min, &e_x, &e_y);
	
	if (e_x != x || e_y != y) {
		d_render_offset(-e_x, -e_y);
		d_render_line_draw(line, ~0);
	}

	d_render_offset(-x, -y);
	d_render_line_draw(line, ~0);

	d_render_offset(0, 0);
	
	return;
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
	SHIP *ship_o;
	DARNIT_STRINGTABLE *shipdata;
	SHAPE *ship_c;
	int i;

	if (!init())
		return -1;
	shipdata = d_stringtable_open("data/shipdata.stz");
	ship_c = shape_load(shipdata, "ship");
	ship_o = ship_new(100, 100, ship_c);
	

	for (i = 0;; i++) {
		d_render_begin();
		ship_loop(ship_o);
		coordinate_rotate(ship_o->line, ship_o->coords, ship_c->lines, ship_c->coordinate, ship_o->angle, &ship_o->min, &ship_o->max);
		render_line_wrap(ship_o->line, ship_o->x / 1000, ship_o->y / 1000, ship_o->max, ship_o->min);
		d_render_end();
		d_loop();
	}

	d_quit();
	return 0;
}
