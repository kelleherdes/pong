#include "classes.h"

using namespace std;
ball::ball(int x_pos, int y_pos, int s) {
    deflecting = false;
    started = true;
	x = x_pos;
	y = y_pos;
	size = s;
    speed = 2;
    start_speed = 2;
    win_score = 5;
    x_d = -1;
    y_d = 1;
    count = 0;
    speed_count = 0;
    game_count = 0;
    winner = 0;
}


void ball::draw() {
	al_draw_filled_rectangle(x, y, x + size, y + size, al_map_rgb(255, 255, 255));
}

void ball::move() {
    if (game_count == 5 && speed != 5) {
        speed++;
        cout << "Speed is" << speed << endl;
        game_count = 0;
    }

    if (count == speed_count) {
        x = x + x_d * speed;
        y = y + y_d * speed;
        count = 0;
    }
    else
        count++;
}

void ball::reset(int x_pos, int y_pos, int player) {
    game_count = 0;
    speed = start_speed;
    x = x_pos;
    y = y_pos;
    if (player == 1)
        x_d = -1;
    else
        x_d = 1;
    y_d = 0;
}

void ball::detect_collision(paddle& p1, paddle& p2, ALLEGRO_SAMPLE* peep, ALLEGRO_SAMPLE* beep, ALLEGRO_SAMPLE* plop) {
    int height, max_height = p1.h + size;
    double norm;

    //wall collision
    if (y <= 0) {
        y_d = y_d * -1;
        y = 1;
        al_play_sample(plop, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
    }

    else if (y >= (int)Y_DIM - size) {
        y_d = y_d * -1;
        y = (int)Y_DIM - size - 1;
        al_play_sample(plop, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
    }

    //collision with paddle
    if (p1.x + p1.w >= x) {
        game_count++;
        if (p1.y + p1.h >= y && y + size >= p1.y && x_d < 0) {
            height = (p1.y + p1.h) - (y - size);
            norm = (double)(height - (double)size / 2) / max_height;
            al_play_sample(beep, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
            if (norm < 0.5)
                y_d = 1;
            else 
                y_d = -1;

            x_d = x_d * -1;
            x = p1.x + p1.w + 1;
        }
        else if (x <= -size) {
            p1.point_lost = true;
            al_play_sample(peep, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
        }
    }

    if (p2.x < x + size) {
        game_count++;
        if (p2.y + p2.h >= y && y + size >= p2.y && x_d > 0) {
            height = (p2.y + p2.h) - (y - size);
            norm = (double) (height - (double) size/2) / max_height;
            al_play_sample(beep, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
            if (norm < 0.5)
                y_d = 1;
            else 
                y_d = -1;
            
            x_d = x_d * -1;
            x = p2.x - size;
        }
        else if (x >= X_DIM) {
            p2.point_lost = true;
            al_play_sample(peep, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
        }
    }
}

void ball::point(paddle& p1, paddle& p2) {
    //detect_collision(p1, p2);
    if (p1.point_lost) {
        cout << "Player 2 wins point" << endl;
        p1.point_lost = false;
        p2.score++;
        reset(X_DIM / 2, Y_DIM / 2, 0);
    }
    else if (p2.point_lost) {
        cout << "Player 1 wins point" << endl;
        p2.point_lost = false;
        p1.score++;
        reset(X_DIM / 2, Y_DIM / 2, 1);
    }
    if (p1.score == win_score) {
        winner = 1;
        started = false;
    }
    else if (p2.score == win_score) {
        winner = 2;
        started = false;
    }
}

paddle::paddle(int x_pos, int y_pos, int width, int height, int sc_x, int sc_y) {
    score_x = sc_x;
    score_y = sc_y;
	x = x_pos;
	y = y_pos;
	w = width;
	h = height;
    score = 0;
    point_lost = false;
}

void paddle::draw(ALLEGRO_FONT* &font) {
	al_draw_filled_rectangle(x, y, x + w, y + h, al_map_rgba_f(0, 0, 0.5, 0.5));
    al_draw_text(font, al_map_rgb(255, 255, 255), score_x, score_y, 0, to_string(score).c_str());
}

void paddle::move(int change) {
	y += change;
}

void must_init(bool test, const char* description)
{
    if (test)
		return;

    printf("couldn't initialize %s\n", description);
    return;
}

void disp_init(ALLEGRO_DISPLAY* &disp, ALLEGRO_BITMAP* &buffer)
{
    al_set_new_display_option(ALLEGRO_SAMPLE_BUFFERS, 1, ALLEGRO_SUGGEST);
    al_set_new_display_option(ALLEGRO_SAMPLES, 8, ALLEGRO_SUGGEST);

    disp = al_create_display(DISP_W, DISP_H);
    must_init(disp, "display");

    buffer = al_create_bitmap(BUFFER_W, BUFFER_H);
    must_init(buffer, "bitmap buffer");
}

void disp_deinit(ALLEGRO_DISPLAY* &disp, ALLEGRO_BITMAP* &buffer)
{
    al_destroy_bitmap(buffer);
    al_destroy_display(disp);
}

void disp_pre_draw(ALLEGRO_DISPLAY* &disp, ALLEGRO_BITMAP* &buffer)
{
    al_set_target_bitmap(buffer);
}

void disp_post_draw(ALLEGRO_DISPLAY* &disp, ALLEGRO_BITMAP* &buffer)
{
    al_set_target_backbuffer(disp);
    al_draw_scaled_bitmap(buffer, 0, 0, BUFFER_W, BUFFER_H, 0, 0, DISP_W, DISP_H, 0);

    al_flip_display();
}