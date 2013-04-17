#include "pong.h"


void ball_init() {
	p.ball.x = (400 - 16) * 1000;
	p.ball.y = (240 - 16) * 1000;
	p.ball.speed = 0;
	p.ball.angle = rand() % 360;
}

/*
int ball_loop() {
	*/
