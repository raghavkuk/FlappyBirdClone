#include<allegro5\allegro.h>
#include<allegro5\allegro_image.h>
#include<allegro5\allegro_font.h>
#include<allegro5\allegro_ttf.h>
void move_hurdle(float *current, float *previous, float *next){
	current[0] -= 2;
	previous[0] -= 2;
	next[0] -= 2;
	if(previous[0] < -71){
		previous[0] = current[0];
		previous[2] = current[2];
		current[0] = next[0];
		current[2] = next[2];
		next[0] = current[0] + 170;
		next[2] = 80 + rand()%70;
	}
}
void move_base(float *base){
	base[0] -= 2;
	if(base[0] < -140)
		base[0] = 0;
}
void bird_up_down(float *y){
	*y += 1;
	if (*y == 231)
		*y = 229;
}
void bird_flap(float *bird){
	*bird += 54;
	if(*bird > 120)
		*bird = 0;
}
void bird_move(float *bird, int &count, float &a, bool fall){
	if(fall == true && bird[1] < 390){
		bird[1] += 10;
		return;
	}
	if(count > 0){
		count--;
		bird[1] -= 4;
	}
	else{
		a += 0.06;
		bird[1] += a;
	}
	if(bird[1] > 390)
		bird[1] = 390;
}
bool collision(float *bird, float *current, float *previous){
	if((bird[0] >= current[0] && bird[0] <= bird[0]+70) && ((bird[1] >= 0 && bird[1] <= current[2]) || (bird[1] >= 340-current[2] && bird[1] <= 395)))
		return true;
	else
		if((bird[0] >= previous[0] && bird[0] <= bird[0]+70) && ((bird[1] >= 0 && bird[1] <= previous[2]) || (bird[1] >= 340-previous[2] && bird[1] <= 395)))
		return true;
	return false;
}
int main(){

	al_init();
	al_install_keyboard();
	al_init_image_addon();
	al_init_font_addon();
	al_init_ttf_addon();

	ALLEGRO_DISPLAY *display = al_create_display(360, 480);
	ALLEGRO_BITMAP *cur_hurdle = al_load_bitmap("hurdle.png");
	ALLEGRO_BITMAP *prev_hurdle = al_load_bitmap("hurdle.png");
	ALLEGRO_BITMAP *next_hurdle = al_load_bitmap("hurdle.png");
	ALLEGRO_BITMAP *background = al_load_bitmap("background.png");
	ALLEGRO_BITMAP *basepic = al_load_bitmap("base.png");
	ALLEGRO_BITMAP *birdpic = al_load_bitmap("bird.png");
	ALLEGRO_BITMAP *tap = al_load_bitmap("tap.png");
	ALLEGRO_BITMAP *get_ready = al_load_bitmap("get_ready.png");
	ALLEGRO_BITMAP *game_over = al_load_bitmap("game_over.png");
	ALLEGRO_FONT *font = al_load_font("LucidaBrightItalic.ttf", 16, 0);
	ALLEGRO_KEYBOARD_STATE keystate;
	ALLEGRO_EVENT_QUEUE *queue = al_create_event_queue();
	ALLEGRO_TIMER *timer = al_create_timer(1.0/60);
	ALLEGRO_TIMER *ftimer = al_create_timer(1.0/10);

	al_register_event_source(queue, al_get_keyboard_event_source());
	al_register_event_source(queue, al_get_display_event_source(display));
	al_register_event_source(queue, al_get_timer_event_source(timer));
	al_register_event_source(queue, al_get_timer_event_source(ftimer));

	bool done = false, draw = true, active = false, start = false, fall = false;
	char* text;
	int count = 0, score = 0;
	float a;
	float current[3] = {620, 0, 80 + rand()%70 }, previous[3] = {460, 0, 80 +rand()%70}, next[3] = {780, 0, 80 + rand()%70}, base[2] = {0, 420}, bird[3] = {90, 230, 50};

	al_start_timer(timer);
	al_start_timer(ftimer);

	while(!done){
		ALLEGRO_EVENT event;
		al_wait_for_event(queue, &event);
		al_get_keyboard_state(&keystate);
		if(event.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
			done = true;
		else if(event.type == ALLEGRO_EVENT_TIMER){
			if(event.timer.source == ftimer){
				if(active == true && start == false && fall == false){
					bird_flap(&bird[2]);
					bird_up_down(&bird[1]);
				}
				if(start == true && active == false && fall == false)
				bird_flap(&bird[2]);
			}
			if(event.timer.source == timer){
				move_base(base);
			    al_clear_to_color(al_map_rgb(0,0,0));
				al_draw_bitmap(background, 0, 0, NULL);
				al_draw_bitmap(basepic, base[0], 420, NULL);
				if(fall== true && bird[1] == 390){
					
					al_draw_bitmap_region(prev_hurdle, 0, 374-previous[2], 70, previous[2], previous[0], 0, NULL);
					al_draw_bitmap_region(prev_hurdle, 70, 0, 70, 300-previous[2], previous[0], previous[2]+120, NULL);
			        al_draw_bitmap_region(cur_hurdle, 0, 374-current[2], 70, current[2], current[0], 0, NULL);
			        al_draw_bitmap_region(next_hurdle, 0, 374-next[2], 70, next[2], next[0], 0, NULL);
			        al_draw_bitmap_region(cur_hurdle, 70, 0, 70, 300-current[2], current[0], current[2]+120, NULL);
					al_draw_bitmap_region(next_hurdle, 70, 0, 70, 300-next[2], next[0], next[2]+120, NULL);
					al_draw_bitmap_region(birdpic, bird[2], 0, 50, 30, bird[0], bird[1], NULL);
					al_draw_bitmap(game_over, 80, 180, NULL);
					break;
				}				
				if(active == false && start == false && fall == false){
			        al_draw_bitmap(get_ready, 80, 160, NULL);
					al_draw_bitmap(tap, 115, 200, NULL);
				}
				if(al_key_down(&keystate, ALLEGRO_KEY_ENTER)){
					active = true;
				}
				if(active == true && start == false && fall == false){
					al_draw_bitmap_region(birdpic, bird[2], 0, 50, 30, bird[0], bird[1], NULL);
				}
				if(al_key_down(&keystate, ALLEGRO_KEY_SPACE) && active == true && start == false && fall == false){
					start = true;
					active = false;
				}
				if(start == true && al_key_down(&keystate, ALLEGRO_KEY_SPACE) && active == false && fall == false){
					count = 6;
					a = 1;
				}
				if(start == true && active == false){
					bird_move(bird, count, a, fall);
					move_hurdle(current, previous, next);
					al_draw_bitmap_region(prev_hurdle, 0, 374-previous[2], 70, previous[2], previous[0], 0, NULL);
			        al_draw_bitmap_region(cur_hurdle, 0, 374-current[2], 70, current[2], current[0], 0, NULL);
			        al_draw_bitmap_region(next_hurdle, 0, 374-next[2], 70, next[2], next[0], 0, NULL);
			        al_draw_bitmap_region(prev_hurdle, 70, 0, 70, 300-previous[2], previous[0], previous[2]+120, NULL);
			        al_draw_bitmap_region(cur_hurdle, 70, 0, 70, 300-current[2], current[0], current[2]+120, NULL);
					al_draw_bitmap_region(next_hurdle, 70, 0, 70, 300-next[2], next[0], next[2]+120, NULL);
					al_draw_bitmap_region(birdpic, bird[2], 0, 50, 30, bird[0], bird[1], NULL);
				}
				if(collision(bird, current, previous)){
					fall = true;
				}
				else
					score++;
			}
			
			al_flip_display();
		}
		
	}
	al_flip_display();
	while(1){
	al_get_keyboard_state(&keystate);
	if(al_key_down(&keystate, ALLEGRO_KEY_ESCAPE))
	return 0;
	}
}