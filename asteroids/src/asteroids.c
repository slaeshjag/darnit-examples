#include <darnit/darnit.h>
#include <stdlib.h>
#include <stdio.h>

#define	ROTATE_COORD_X(angle, x, y)		(((d_util_sin((angle) + 900) * (x)) - (d_util_sin((angle)) * (y))) >> 16)
#define	ROTATE_COORD_Y(angle, x, y)		(((d_util_sin((angle)) * (x)) + (d_util_sin((angle + 900)) * (y))) >> 16)
int ship_c[] = { 0, -24, 16, 24, 0, 8, -16, 24, 0, -24 };


void coordinate_rotate(DARNIT_LINE *line, int lines, int *coordinate, int angle) {
	int i, x, y, x2, y2;

	for (i = 0; i < lines; i++) {
		x = coordinate[i * 2];
		y = coordinate[i * 2 + 1];
		x2 = coordinate[i * 2 + 2];
		y2 = coordinate[i * 2 + 3];
		d_render_line_move(line, i, ROTATE_COORD_X(angle, x, y), ROTATE_COORD_Y(angle, x, y),
			ROTATE_COORD_X(angle, x2, y2), ROTATE_COORD_Y(angle, x2, y2));
	}

	return;
}


int main(int argc, char **argv) {
	DARNIT_LINE *ship;
	int i;

	d_init("Asteroids", "asteroids", NULL);
	
	ship = d_render_line_new(4, 1);

	for (i = 0;; i++) {
		coordinate_rotate(ship, 4, ship_c, i * 10);
		d_render_begin();
		d_render_offset(-256, -128);
		d_render_line_draw(ship, 4);
		d_render_end();
		d_loop();
	}

	d_quit();
	return 0;
}
