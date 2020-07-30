#include "classes.h"


int main(){
    
    must_init(al_init(), "allegro");
    must_init(al_install_keyboard(), "keyboard");
    must_init(al_install_audio(), "audio");
    must_init(al_init_acodec_addon(), "audio codecs");
    must_init(al_reserve_samples(16), "reserve samples");

    ALLEGRO_SAMPLE* peep = al_load_sample("peep.ogg");
    ALLEGRO_SAMPLE* plop = al_load_sample("plop.ogg");
    ALLEGRO_SAMPLE* beep = al_load_sample("beep.ogg");

    ALLEGRO_TIMER * timer = al_create_timer(1.0 / 60.0);
    must_init(timer, "timer");

    ALLEGRO_EVENT_QUEUE * queue = al_create_event_queue();
    must_init(queue, "queue");

    /*ALLEGRO_DISPLAY * disp = al_create_display(X_DIM, Y_DIM);
    must_init(disp, "display");*/

    ALLEGRO_FONT * font = al_create_builtin_font();
    must_init(font, "font");

    ALLEGRO_BITMAP * buffer;
    ALLEGRO_DISPLAY * disp;

    disp_init(disp, buffer);

    must_init(al_init_primitives_addon(), "primitives");

    al_register_event_source(queue, al_get_keyboard_event_source());
    al_register_event_source(queue, al_get_display_event_source(disp));
    al_register_event_source(queue, al_get_timer_event_source(timer));

    bool done = false;
    bool redraw = true;
    ALLEGRO_EVENT event;
    
    al_start_timer(timer);

    unsigned char key[ALLEGRO_KEY_MAX];
    memset(key, 0, sizeof(key));
    //paddles
    int w = 5, h = 50, change = 4;
    int x1 = 10, y1 = 10, x2 = X_DIM - 10 - w, y2 = 10;
    int p1_scorex = 50, p1_scorey = 20, p2_scorex = X_DIM - 50, p2_scorey = 20;
    paddle p1(x1, y1, w, h, p1_scorex, p1_scorey), p2(x2, y2, w, h, p2_scorex, p2_scorey);

    //ball
    int bx1 = X_DIM / 2, by1 = Y_DIM / 2, size = 10;
    ball game_ball(bx1, by1, size);
    string win_message;
    while (1)
    {
        al_wait_for_event(queue, &event);
        switch (event.type)
        {
        case ALLEGRO_EVENT_TIMER:
            if (game_ball.started) {
                game_ball.detect_collision(p1, p2, peep, beep, plop);
                game_ball.point(p1, p2);
                game_ball.move();

            }
           
            if (key[ALLEGRO_KEY_UP])
                p2.move(-change);
                
            if (key[ALLEGRO_KEY_DOWN])
                p2.move(change);

            if (key[ALLEGRO_KEY_W])
                p1.move(-change);

            if (key[ALLEGRO_KEY_S])
                p1.move(change);

            if (key[ALLEGRO_KEY_ESCAPE])
                done = true;

            for (int i = 0; i < ALLEGRO_KEY_MAX; i++)
                key[i] &= KEY_SEEN;
    
            redraw = true;
            break;

        case ALLEGRO_EVENT_KEY_DOWN:
            key[event.keyboard.keycode] = KEY_SEEN | KEY_RELEASED;
            break;
        case ALLEGRO_EVENT_KEY_UP:
            key[event.keyboard.keycode] &= KEY_RELEASED;
            break;

        case ALLEGRO_EVENT_DISPLAY_CLOSE:
            done = true;
            break;
        }

        if (done)
            break;

        if (redraw && al_is_event_queue_empty(queue))
        {
            disp_pre_draw(disp, buffer);
            al_clear_to_color(al_map_rgb(0, 0, 0));
            if (game_ball.started) {
                p1.draw(font);
                p2.draw(font);
                game_ball.draw();
            }
            
            else {
                win_message = "WINNER IS PLAYER " + to_string(game_ball.winner);
                p1.draw(font);
                p2.draw(font);
                al_draw_text(font, al_map_rgb(255, 255, 255), (double)X_DIM / 2 - 30, (double)Y_DIM / 2 - 10, 0, "GAME OVER");
                al_draw_text(font, al_map_rgb(255, 255, 255), (double)X_DIM / 2 - 60, (double)Y_DIM / 2 + 10, 0, win_message.c_str());
            }
            disp_post_draw(disp, buffer);
            al_flip_display();
            redraw = false;
            

        }
    }

    al_destroy_font(font);
    al_destroy_display(disp);
    al_destroy_timer(timer);
    al_destroy_sample(plop);
    al_destroy_sample(peep);
    al_destroy_sample(beep);
    al_destroy_event_queue(queue);
    disp_deinit(disp, buffer);
    return 0;
}