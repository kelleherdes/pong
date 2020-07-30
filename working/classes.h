#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <allegro5/allegro5.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h>
#include <string>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>

using namespace std;

#define KEY_SEEN 1
#define KEY_RELEASED 2

#define BUFFER_W 320
#define BUFFER_H 240

#define DISP_SCALE 3
#define DISP_W (BUFFER_W * DISP_SCALE)
#define DISP_H (BUFFER_H * DISP_SCALE)
#define X_DIM 320
#define Y_DIM 240

void must_init(bool test, const char* description);
void disp_init(ALLEGRO_DISPLAY* &disp, ALLEGRO_BITMAP* &buffer);
void disp_deinit(ALLEGRO_DISPLAY* &disp, ALLEGRO_BITMAP* &buffer);
void disp_post_draw(ALLEGRO_DISPLAY* &disp, ALLEGRO_BITMAP* &buffer);
void disp_pre_draw(ALLEGRO_DISPLAY* &disp, ALLEGRO_BITMAP* &buffer);
class paddle;

class ball {
	public:
		int x, y, size, speed, x_d, y_d, count, speed_count, game_count;
		int start_speed, win_score, winner;
		bool deflecting, started;
		ball(int x_pos, int y_pos, int s);
		void draw();
		void move();
		void point(paddle& p1, paddle& p2);
		void detect_collision(paddle &p1, paddle &p2, ALLEGRO_SAMPLE* peep, ALLEGRO_SAMPLE* beep, ALLEGRO_SAMPLE* plop);
		void reset(int x_pos, int y_pos, int player);
		//void control(paddle& p1, paddle& p2, unsigned char* key);
};

class paddle {
	public:
		int x, y, w, h, score;
		int score_x, score_y;
		bool point_lost;
		paddle(int x_pos, int y_pos, int width, int height, int sc_x, int sc_y);
		void draw(ALLEGRO_FONT* &font);
		void move(int change);
};