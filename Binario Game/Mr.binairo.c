#include <stdio.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>



int width = 1377;
int height = 768;
int FPS = 41;
int pos_x = 0, pos_y = 0;
int level = -1, difficulity = 0;
int s3 = 0;
int mtx[6][6] = { 0 }, mtx1[8][8] = { 0 }, mtx2[10][10] = { 0 };
int levels[9][6][6] = { 0 }, levels1[9][8][8] = { 0 }, levels2[9][10][10] = { 0 };

float exit_x1 = (1377 / 2) - 110, exit_x2 = (1377 / 2) + 100, exit_y1 = (768 / 2) + 220, exit_y2 = (768 / 2) + 300;
float start_x1 = (1377 / 2) - 200, start_x2 = (1377 / 2) + 200, start_y1 = (768 / 2), start_y2 = (768 / 2) + 80;
float easy_x1 = 20, easy_x2 = 125, easy_y1 = 70, easy_y2 = 100;
float medium_x1 = 20, medium_x2 = 135, medium_y1 = 105, medium_y2 = 135;
float hard_x1 = 20, hard_x2 = 125, hard_y1 = 140, hard_y2 = 175;

bool running = true;
bool check = true;
bool music = true;
bool locked[6][6] = { false }, locked1[8][8] = { false }, locked2[10][10] = { false };

ALLEGRO_DISPLAY* display;

ALLEGRO_TIMER* timer;

ALLEGRO_EVENT_QUEUE* queue;
ALLEGRO_EVENT ev;

ALLEGRO_BITMAP* background;
ALLEGRO_BITMAP* background2;
ALLEGRO_BITMAP* board;
ALLEGRO_BITMAP* choose_size;
ALLEGRO_BITMAP* pause_b;
ALLEGRO_BITMAP* pause_m;
ALLEGRO_BITMAP* music_off;
ALLEGRO_BITMAP* music_on;
ALLEGRO_BITMAP* next_level;
ALLEGRO_BITMAP* back_to_menu;
ALLEGRO_BITMAP* won;

ALLEGRO_FONT* font;
ALLEGRO_FONT* font2;
ALLEGRO_FONT* font3;

ALLEGRO_SAMPLE* click;
ALLEGRO_SAMPLE* error;
ALLEGRO_SAMPLE* song;
ALLEGRO_SAMPLE* end;
ALLEGRO_SAMPLE* pok;
ALLEGRO_SAMPLE* tss;
ALLEGRO_SAMPLE_INSTANCE* instance; 



int init();
void main_menu();
void how_to_play();
void choose_diff();
void choose_level();
void easy();
void medium();
void hard();
void win();
void pause_s();
int destroy();

void levels_easy();
void levels_medium();
void levels_hard();



int main() {
	
	init();

	return 0;
}

int init() {

	if (!al_init()) {
		return -1;
	}

	al_install_keyboard();
	al_install_mouse();
	al_install_audio();

	al_init_ttf_addon();
	al_init_font_addon();
	al_init_image_addon();
	al_init_primitives_addon();
	al_init_acodec_addon();

	al_reserve_samples(1);

	al_set_new_display_flags(ALLEGRO_WINDOWED);

	timer = al_create_timer(1.0 / FPS);
	display = al_create_display(width, height);
	queue = al_create_event_queue();

	song = al_load_sample("sound.wav");
	click = al_load_sample("click.wav");
	error = al_load_sample("no_access.wav");
	end = al_load_sample("end.wav");
	pok = al_load_sample("pok.wav");
	tss = al_load_sample("tss.wav");
	instance = al_create_sample_instance(song);
	al_set_sample_instance_playmode(instance, ALLEGRO_PLAYMODE_LOOP);
	al_attach_sample_instance_to_mixer(instance, al_get_default_mixer());

	font = al_load_ttf_font("ariel.ttf", 80, 0);
	font2 = al_load_ttf_font("ariel.ttf", 30, 0);
	font3 = al_load_ttf_font("ariel.ttf", 100, 0);

	background = al_load_bitmap("unnamed.jpg");
	background2 = al_load_bitmap("background2.jpg");
	board = al_create_bitmap(width - 500, height);
	choose_size = al_load_bitmap("dark.jpg");
	pause_b = al_load_bitmap("pause.png");
	pause_m = al_load_bitmap("pause menu.png");
	music_on = al_load_bitmap("soundOn.png");
	music_off = al_load_bitmap("soundOff.png");
	back_to_menu = al_load_bitmap("go to menu.png");
	next_level = al_load_bitmap("next level.png");
	won = al_load_bitmap("win.jpg");

	al_register_event_source(queue, al_get_timer_event_source(timer));
	al_register_event_source(queue, al_get_mouse_event_source());
	al_register_event_source(queue, al_get_keyboard_event_source());
	al_register_event_source(queue, al_get_display_event_source(display));



	al_set_window_title(display, "Mr.Binairo");
	al_start_timer(timer);
	
	
	while (running == true) {

		main_menu();
		al_flip_display();
		
	}
}
void main_menu() {
	al_wait_for_event(queue, &ev);

	FPS = 40;
	difficulity = 0;

	al_set_timer_speed(timer, 1.0 / FPS);
	al_draw_bitmap(background, 0, 0, 0);


	al_draw_text(font, al_map_rgb(255, 255, 255), start_x1 + 20, start_y1, 0, "Start new game");
	al_draw_text(font, al_map_rgb(255, 255, 255), start_x1 + 60, exit_y1 - 110, 0, "How to play");
	al_draw_text(font, al_map_rgb(255, 255, 255), exit_x1 + 60, exit_y1, 0, "Quit");

	al_stop_timer(timer);
	if (ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
		running = false;
	}

	if (ev.type == ALLEGRO_EVENT_MOUSE_AXES) {
		if (ev.mouse.x >= start_x1 && ev.mouse.x < start_x2 && ev.mouse.y >= start_y1 && ev.mouse.y < start_y2) {
			al_clear_to_color(al_map_rgb(0, 0, 0));
			al_draw_bitmap(background, 0, 0, 0);
			al_draw_text(font, al_map_rgb(0, 0, 255), start_x1 + 20, start_y1, 0, "Start new game :D");
			al_draw_text(font, al_map_rgb(255, 255, 255), start_x1 + 60, exit_y1 - 110, 0, "How to play");
			al_draw_text(font, al_map_rgb(255, 255, 255), exit_x1 + 60, exit_y1, 0, "Quit");
			
		}
		else if (ev.mouse.x >= start_x1 + 60 && ev.mouse.y >= exit_y1 - 110 && ev.mouse.x < exit_x2 + 40 && ev.mouse.y < exit_y2 - 120) {
			al_clear_to_color(al_map_rgb(0, 0, 0));
			al_draw_bitmap(background, 0, 0, 0);
			al_draw_text(font, al_map_rgb(255, 255, 255), start_x1 + 20, start_y1, 0, "Start new game");
			al_draw_text(font, al_map_rgb(0, 255, 125), start_x1 + 60, exit_y1 - 110, 0, "How to play ?");
			al_draw_text(font, al_map_rgb(255, 255, 255), exit_x1 + 60, exit_y1, 0, "Quit");
			
		}
		else if (ev.mouse.x >= exit_x1 && ev.mouse.y >= exit_y1 && ev.mouse.x < exit_x2 && ev.mouse.y < exit_y2) {
			al_clear_to_color(al_map_rgb(0, 0, 0));
			al_draw_bitmap(background, 0, 0, 0);
			al_draw_text(font, al_map_rgb(255, 255, 255), start_x1 + 20, start_y1, 0, "Start new game");
			al_draw_text(font, al_map_rgb(255, 255, 255), start_x1 + 60, exit_y1 - 110, 0, "How to play");
			al_draw_text(font, al_map_rgb(255, 0, 0), exit_x1 + 60, exit_y1, 0, "Quit :(");
		}
	}
	else if (ev.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN) {
		if (ev.mouse.button & 1) {
			if (ev.mouse.x >= exit_x1 && ev.mouse.y >= exit_y1 && ev.mouse.x < exit_x2 && ev.mouse.y < exit_y2) {
				running = false;
				destroy();
			}
			else if (ev.mouse.x >= start_x1 + 60 && ev.mouse.y >= exit_y1 - 100 && ev.mouse.x < exit_x2 + 40 && ev.mouse.y < exit_y2 - 115) {
				al_play_sample(click, 1, 0, 1, ALLEGRO_PLAYMODE_ONCE, NULL);
				check = true;
				how_to_play();
			}
			else if (ev.mouse.x >= start_x1 && ev.mouse.x < start_x2 && ev.mouse.y >= start_y1 && ev.mouse.y < start_y2) {
				al_play_sample(click, 1, 0, 1, ALLEGRO_PLAYMODE_ONCE, NULL);

				if (difficulity == 0 ) {
					check = true;
					choose_diff();
				}
				//easy game (6x6)
				if (difficulity == 1 && level >= 0) {
					check = true;
					easy();
				}
				//medium game (8x8)
				else if (difficulity == 2 && level >= 0) {
					check = true;
					medium();
				}
				//hard game (10x10)
				else if (difficulity == 3 && level >= 0) {
					check = true;
					hard();
				}

			}
		}
	}
}

void how_to_play() {

	font3 = al_load_ttf_font("ariel.ttf", 60, 0);

	al_start_timer(timer);
	al_clear_to_color(al_map_rgb(0, 0, 0));
	while (check) {
		al_draw_bitmap(choose_size, 0, 0, 0);
		al_wait_for_event(queue, &ev);
		al_draw_text(font3, al_map_rgb(25, 200, 255), (width / 2) - 650, (height / 2) - 300, 0, "Try to fill the whole table with respect to the next:");
		al_draw_text(font3, al_map_rgb(25, 200, 255), (width / 2) - 650, (height / 2) - 200, 0, "1- Don't put 3 same numbers beside one another.");
		al_draw_text(font3, al_map_rgb(25, 200, 255), (width / 2) - 650, (height / 2) - 130, 0, "2- There should be no columns or raws repeated.");
		al_draw_text(font3, al_map_rgb(25, 200, 255), (width / 2) - 650, (height / 2) - 60, 0, "3- Half of every column and raw should be 0's and half should be 1's.");
		al_draw_text(font, al_map_rgb(255, 255, 255), exit_x1 + 50, exit_y1, 0, "Back");
		al_stop_timer(timer);
		
		if (ev.mouse.x >= exit_x1 + 50 && ev.mouse.y >= exit_y1 && ev.mouse.x <= exit_x2 + 50 && ev.mouse.y <= exit_y2) {
			al_clear_to_color(al_map_rgb(0, 0, 0));
			al_draw_bitmap(choose_size, 0, 0, 0);
			al_draw_text(font3, al_map_rgb(25, 200, 255), (width / 2) - 650, (height / 2) - 300, 0, "Try to fill the whole table with respect to the next:");
			al_draw_text(font3, al_map_rgb(25, 200, 255), (width / 2) - 650, (height / 2) - 200, 0, "1- Don't put 3 same numbers beside one another.");
			al_draw_text(font3, al_map_rgb(25, 200, 255), (width / 2) - 650, (height / 2) - 130, 0, "2- There should be no columns or raws repeated.");
			al_draw_text(font3, al_map_rgb(25, 200, 255), (width / 2) - 650, (height / 2) - 60, 0, "3- Half of every column and raw should be 0's and half should be 1's.");
			al_draw_text(font, al_map_rgb(0, 0, 255), exit_x1 + 50, exit_y1, 0, "Back");
			if (ev.mouse.button & 1) {
				al_play_sample(click, 1, 0, 1, ALLEGRO_PLAYMODE_ONCE, NULL);
				check = false;
			}
		}
		al_flip_display();
		al_clear_to_color(al_map_rgb(0, 0, 0));
	}
}
void choose_diff() {
	al_start_timer(timer);

	while (check) {

		al_draw_bitmap(choose_size, 0, 0, 0);
		
		al_wait_for_event(queue, &ev);

		level = -1;

		al_draw_text(font2, al_map_rgb(0, 255, 125), (width / 2) - 125 + 20, (height / 2) - 100 + 15, 0, "Choose Difficulity:");
		al_draw_text(font2, al_map_rgb(0, 125, 125), (width / 2) - 125 + easy_x1, (height / 2) - 100 + easy_y1, 0, "Easy:6x6");
		al_draw_text(font2, al_map_rgb(0, 125, 125), (width / 2) - 125 + medium_x1, (height / 2) - 100 + medium_y1, 0, "Medium:8x8");
		al_draw_text(font2, al_map_rgb(0, 125, 125), (width / 2) - 125 + hard_x1, (height / 2) - 100 + hard_y1, 0, "Hard:10x10");
		
		al_stop_timer(timer);

		if (ev.type == ALLEGRO_EVENT_MOUSE_AXES) {
			if (ev.mouse.x >= ((width / 2) - 125) + easy_x1 && ev.mouse.x < ((width / 2) - 125) + easy_x2 && ev.mouse.y >= ((height / 2) - 100) + easy_y1 && ev.mouse.y < ((height / 2) - 100) + easy_y2) {
				al_draw_text(font2, al_map_rgb(0, 255, 125), (width / 2) - 125 + easy_x1, (height / 2) - 100 + easy_y1, 0, "Easy:6x6");
			}
			else if (ev.mouse.x >= ((width / 2) - 125) + medium_x1 && ev.mouse.x < ((width / 2) - 125) + medium_x2 && ev.mouse.y >= ((height / 2) - 100) + medium_y1 && ev.mouse.y < ((height / 2) - 100) + medium_y2) {
				al_draw_text(font2, al_map_rgb(0, 255, 125), (width / 2) - 125 + medium_x1, (height / 2) - 100 + medium_y1, 0, "Medium:8x8");
			}
			else if (ev.mouse.x >= ((width / 2) - 125) + hard_x1 && ev.mouse.x < ((width / 2) - 125) + hard_x2 && ev.mouse.y >= ((height / 2) - 100) + hard_y1 && ev.mouse.y < ((height / 2) - 100) + hard_y2) {
				al_draw_text(font2, al_map_rgb(0, 255, 125), (width / 2) - 125 + hard_x1, (height / 2) - 100 + hard_y1, 0, "Hard:10x10");
			}
		}
		else if (ev.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN) {
			if (ev.mouse.x >= ((width / 2) - 125) + easy_x1 && ev.mouse.x < ((width / 2) - 125) + easy_x2 && ev.mouse.y >= ((height / 2) - 100) + easy_y1 && ev.mouse.y < ((height / 2) - 100) + easy_y2) {
				al_play_sample(click, 1, 0, 1, ALLEGRO_PLAYMODE_ONCE, NULL);
				difficulity = 1;			
				choose_level();			
				check = false;
			}
			else if (ev.mouse.x >= ((width / 2) - 125) + medium_x1 && ev.mouse.x < ((width / 2) - 125) + medium_x2 && ev.mouse.y >= ((height / 2) - 100) + medium_y1 && ev.mouse.y < ((height / 2) - 100) + medium_y2) {
				al_play_sample(click, 1, 0, 1, ALLEGRO_PLAYMODE_ONCE, NULL);
				difficulity = 2;
				choose_level();
				check = false;
			}
			else if (ev.mouse.x >= ((width / 2) - 125) + hard_x1 && ev.mouse.x < ((width / 2) - 125) + hard_x2 && ev.mouse.y >= ((height / 2) - 100) + hard_y1 && ev.mouse.y < ((height / 2) - 100) + hard_y2) {
				al_play_sample(click, 1, 0, 1, ALLEGRO_PLAYMODE_ONCE, NULL);
				difficulity = 3;
				choose_level();
				check = false;
			}
		}

		if (ev.keyboard.keycode == ALLEGRO_KEY_ESCAPE) {
			check = false;
			al_clear_to_color(al_map_rgb(0, 0, 0));
		}

		al_flip_display();
		al_clear_to_color(al_map_rgb(0, 0, 0));
	}
}
void choose_level() {

	

	al_start_timer(timer);
	

	while (check) {

		int s = 284;
		int s2 = 480.5;
		int s3 = 1;

		al_draw_bitmap(choose_size, 0, 0, 0);

		al_wait_for_event(queue, &ev);

		al_draw_text(font2, al_map_rgb(0, 255, 0), (width / 2) - 125 + 20, (height / 2) - 100 + 15, 0, "Choose Level:");

		for (int i = 0; i < 3; i++) {
			for (int j = 0; j < 3; j++) {		

				al_draw_textf(font2, al_map_rgb(0, 255, 0), s2 + easy_x1, s + easy_y1, 0, "Level %d ", s3);		

				if (ev.mouse.x >= s2 + easy_x1 && ev.mouse.x < s2 + easy_x2 - 20 && ev.mouse.y >= s + easy_y1 && ev.mouse.y < s + easy_y2) {
					al_draw_textf(font2, al_map_rgb(255, 0, 0), s2 + easy_x1, s + easy_y1, 0, "Level %d", s3);
					al_wait_for_event(queue,&ev);
					if (ev.mouse.button & 1) {
						al_play_sample(click, 1, 0, 1, ALLEGRO_PLAYMODE_ONCE, NULL);
						level = s3 - 1;
						check = false;
					}
				}
				if (s2 < 825.5) {
					s2 = s2 + 115;
				}
				s3++;
			}
			if (s < 389) {
				s = s + 35;
			}
			s2 = 480.5;
		}

		al_stop_timer(timer);	
		if (ev.keyboard.keycode == ALLEGRO_KEY_ESCAPE) {
			check = false;
		}

		al_flip_display();
		al_clear_to_color(al_map_rgb(0, 0, 0));
	}
}

void easy() {

	if (music == true) {
		al_play_sample_instance(instance);
	}
	font = al_load_ttf_font("ariel.ttf", 80, 0);
	font2 = al_load_ttf_font("ariel.ttf", 30, 0);
	font3 = al_load_ttf_font("ariel.ttf", 100, 0);

	al_set_target_bitmap(board);
	al_clear_to_color(al_map_rgb(0, 0, 0));
	al_convert_mask_to_alpha(board, al_map_rgb(0, 0, 0));
	al_set_target_backbuffer(display);


	

	int pos_x = 0, pos_y = 0;
	int x[6] = { 0 }, y[6] = { 0 };
	int c = 0, c2 = 0, r = 0, r2 = 0, d = 0, z = 0, o = 0;
	int time = 0;
	FPS = 1;

	for (int i = 0; i < 6; i++) {
		for (int j = 0; j < 6; j++) {
			mtx[i][j] = 2;
			locked[i][j] = false;
		}
	}
	al_start_timer(timer);
	// here we put the locked boxes(LEVEL):
	levels_easy();
	//

	while (check == true)
	{

		bool won = true, won2 = true, won3 = true;

		al_draw_bitmap(background2, 0, 0, 0);
		al_draw_bitmap(pause_b, 1230, 600, 0);

		if (music == true) {
			al_draw_bitmap(music_on, 1230, 680, 0);
		}
		else if (music == false) {
			al_draw_bitmap(music_off, 1230, 680, 0);
		}
		al_wait_for_event(queue, &ev);



		for (float i = 0; i <= height; i += height / 6) {
			al_set_target_bitmap(board);
			al_draw_line(0, i, width - 500, i, al_map_rgb(255, 255, 255), 3);
			if (y[c] == 0) {
				y[c] = i;
				c++;
			}
		}
		c = 0;
		for (float j = 1; j <= width - 500; j += (width - 500) / 6) {
			al_draw_line(j, 0, j, height, al_map_rgb(255, 255, 255), 3);
			if (x[c] == 0) {
				x[c] = j;
				c++;
			}
		}
		if (d == 0) {
			for (c = 0; c < 6; c++) {
				for (c2 = 0; c2 < 6; c2++) {
					if (mtx[c][c2] != 2 && locked[c][c2] == true) {
						al_draw_textf(font3, al_map_rgb(255, 255, 255), x[c] + 50, y[c2] + 5, 0, "%d", mtx[c][c2]);
					}
				}
			}
		}
		d = 1;

		al_set_timer_speed(timer, 1.0 / FPS);
		al_set_target_backbuffer(display);

		if (ev.type == ALLEGRO_EVENT_TIMER) {
			time++;
		}

		al_draw_textf(font2, al_map_rgb(255, 255, 255), 1150, 50, 0, "Time passed: %d", time);
		al_draw_textf(font2, al_map_rgb(255, 255, 255), 1150, 100, 0, "pos_x: %d", pos_x);
		al_draw_textf(font2, al_map_rgb(255, 255, 255), 1150, 150, 0, "pos_y: %d", pos_y);

		if (ev.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN) {
			if (ev.mouse.button & 1 && ev.mouse.x >= 251 && ev.mouse.x <= width - 250) {
				for (c = 0; c < 5; c++) {
					if (ev.mouse.x >= x[c] + 250 && ev.mouse.x < x[c + 1] + 250)
						break;
				}
				for (c2 = 0; c2 < 5; c2++) {
					if (ev.mouse.y >= y[c2] && ev.mouse.y < y[c2 + 1])
						break;
				}

				if (mtx[c][c2] == 2 && locked[c][c2] == false) {
					al_play_sample(pok, 1, 0, 1, ALLEGRO_PLAYMODE_ONCE, NULL);
					al_set_target_bitmap(board);
					al_draw_textf(font3, al_map_rgb(255, 255, 255), x[c] + 50, y[c2] + 5, 0, "0");
					mtx[c][c2] = 0;
				}

				else if (mtx[c][c2] == 0 && locked[c][c2] == false) {
					al_play_sample(pok, 1, 0, 1, ALLEGRO_PLAYMODE_ONCE, NULL);
					al_set_target_bitmap(board);
					al_draw_filled_rectangle(x[c] + 30, y[c2] + 15, x[c] + 100, y[c2] + 100, al_map_rgb(0, 0, 0));
					al_convert_mask_to_alpha(board, al_map_rgb(0, 0, 0));
					al_draw_text(font3, al_map_rgb(255, 255, 255), x[c] + 50, y[c2] + 5, 0, "1");				
					mtx[c][c2]++;
				}
				else if (mtx[c][c2] == 1 && locked[c][c2] == false) {
					al_play_sample(tss, 1, 0, 1, ALLEGRO_PLAYMODE_ONCE, NULL);
					al_set_target_bitmap(board);
					al_draw_filled_rectangle(x[c] + 30, y[c2] + 15, x[c] + 100, y[c2] + 100, al_map_rgb(0, 0, 0));
					al_convert_mask_to_alpha(board, al_map_rgb(0, 0, 0));				
					mtx[c][c2]++;

				}

			}
			if (ev.mouse.x >= 1230 && ev.mouse.y >= 600 && ev.mouse.x <= 1280 && ev.mouse.y <= 650) {
				pause_s();
			}
			else if (ev.mouse.x >= 1230 && ev.mouse.y >= 680 && ev.mouse.x <= 1280 && ev.mouse.y <= 720) {
				if (music == true) {
					if (ev.mouse.button & 1) {
						al_stop_sample_instance(instance);
						al_draw_bitmap(music_on, 1230, 680, 0);
						music = false;
					}
				}
				else if (music == false) {
					if (ev.mouse.button & 1) {
						al_play_sample_instance(instance);
						al_draw_bitmap(music_off, 1230, 680, 0);
						music = true;
					}
				}
			}
		}
		al_set_target_backbuffer(display);

		if (ev.type == ALLEGRO_EVENT_MOUSE_AXES) {
			pos_x = ev.mouse.x;
			pos_y = ev.mouse.y;
		}
		for (int i = 0; i < 6; i++) {
			for (int j = 0; j < 6; j++) {
				if (mtx[i][j] == 2) {
					won3 = false;
				}
			}
		}
		for (int c3 = 0; c3 < 5; c3++) {
			for (int c2 = c3 + 1; c2 < 6; c2++) {
				r = 0;
				for (int c = 0; c < 6; c++) {
					if (mtx[c3][c] != 2 && mtx[c3][c] == mtx[c2][c]) {
						r++;
						if (r == 6) {
							if (ev.mouse.button & 1 && ev.mouse.x >= 251 && ev.mouse.x <= width - 250) {
								al_play_sample(error, 3, 0, 1, ALLEGRO_PLAYMODE_ONCE, NULL);
								al_draw_line(x[c3] + 250 + 67, 20, x[c3] + 250 + 67, 750, al_map_rgb(255, 0, 0), 6);
								al_draw_line(x[c2] + 250 + 67, 20, x[c2] + 250 + 67, 750, al_map_rgb(255, 0, 0), 6);
							}
							al_draw_line(x[c3] + 250 + 67, 20, x[c3] + 250 + 67, 750, al_map_rgb(255, 255, 255), 3);
							al_draw_line(x[c2] + 250 + 67, 20, x[c2] + 250 + 67, 750, al_map_rgb(255, 255, 255), 3);
							won = false;
						}

					}

				}


			}

		}

		for (int c3 = 0; c3 < 5; c3++) {
			for (int c2 = c3 + 1; c2 < 6; c2++) {
				r2 = 0;
				for (int c = 0; c < 6; c++) {
					if (mtx[c][c3] != 2 && mtx[c][c3] == mtx[c][c2]) {
						r2++;
						if (r2 == 6) {
							if (ev.mouse.button & 1 && ev.mouse.x >= 251 && ev.mouse.x <= width - 250) {
								al_play_sample(error, 3, 0, 1, ALLEGRO_PLAYMODE_ONCE, NULL);
								al_draw_line(280, y[c3] + 60, 1086, y[c3] + 60, al_map_rgb(255, 0, 0), 7);
								al_draw_line(280, y[c2] + 60, 1086, y[c2] + 60, al_map_rgb(255, 0, 0), 7);
							}
							al_draw_line(280, y[c3] + 60, 1086, y[c3] + 60, al_map_rgb(255, 255, 255), 3);
							al_draw_line(280, y[c2] + 60, 1086, y[c2] + 60, al_map_rgb(255, 255, 255), 3);
							won = false;
						}

					}

				}


			}

		}
		
		for (c = 0; c < 6; c++) {
			for (c2 = 0; c2 < 4; c2++) {
				if (mtx[c][c2] != 2 && mtx[c][c2] == mtx[c][c2 + 1] && mtx[c][c2] == mtx[c][c2 + 2]) {
					if (ev.mouse.button & 1 && ev.mouse.x >= 251 && ev.mouse.x <= width - 250) {
						al_play_sample(error, 3, 0, 1, ALLEGRO_PLAYMODE_ONCE, NULL);
						al_draw_line(x[c] + 250 + 67, y[c2] + 20, x[c] + 250 + 67, y[c2 + 2] + 100, al_map_rgb(255, 0, 0), 7);
					}
					al_draw_line(x[c] + 250 + 67, y[c2] + 20, x[c] + 250 + 67, y[c2 + 2] + 100, al_map_rgb(255, 255, 255), 3);
					won2 = false;
				}
			}
		}

		for (c = 0; c < 6; c++) {
			for (c2 = 0; c2 < 4; c2++) {
				if (mtx[c2][c] != 2 && mtx[c2][c] == mtx[c2 + 1][c] && mtx[c2][c] == mtx[c2 + 2][c]) {
					if (ev.mouse.button & 1 && ev.mouse.x >= 251 && ev.mouse.x <= width - 250) {
						al_play_sample(error, 3, 0, 1, ALLEGRO_PLAYMODE_ONCE, NULL);
						al_draw_line(x[c2] + 280, y[c] + 60, x[c2 + 2] + 350, y[c] + 60, al_map_rgb(255, 0, 0), 7);
					}
					al_draw_line(x[c2] + 280, y[c] + 60, x[c2 + 2] + 350, y[c] + 60, al_map_rgb(255, 255, 255), 3);
					won2 = false;
				}
			}
		}
		for (c = 0; c < 6; c++) {
			z = 0;
			o = 0;
			for (c2 = 0; c2 < 6; c2++) {
				if (mtx[c2][c] == 1) {
					o++;
				}
				if (mtx[c2][c] == 0) {
					z++;				
				}
				if ((o > 3) || (z > 3)) {
					if (ev.mouse.button & 1 && ev.mouse.x >= 251 && ev.mouse.x <= width - 250) {
						al_play_sample(error, 3, 0, 1, ALLEGRO_PLAYMODE_ONCE, NULL);
						al_draw_line(280, y[c] + 60, 1081, y[c] + 60, al_map_rgb(255, 0, 0), 7);
					}
					al_draw_line(280, y[c] + 60, 1081, y[c] + 60, al_map_rgb(255, 255, 255), 3);
					won2 = false;
				}
			}
		}
		for (c2 = 0; c2 < 6; c2++) {
			z = 0;
			o = 0;
			for (c = 0; c < 6; c++) {
				if (mtx[c2][c] == 1) {
					o++;
				}
				if (mtx[c2][c] == 0) {
					z++;
				}
				if ((o > 3) || (z > 3)) {
					if (ev.mouse.button & 1 && ev.mouse.x >= 251 && ev.mouse.x <= width - 250) {
						al_play_sample(error, 3, 0, 1, ALLEGRO_PLAYMODE_ONCE, NULL);
						al_draw_line(x[c2] + 250 + 67, 20, x[c2] + 250 + 67, 750, al_map_rgb(255, 0, 0), 7);
					}
					al_draw_line(x[c2] + 250 + 67, 20, x[c2] + 250 + 67, 750, al_map_rgb(255, 255, 255), 3);
					won2 = false;
				}
			}
		}

		if (won == true && won2 == true && won3 == true) {
			win();
		}

		if (ev.keyboard.keycode == ALLEGRO_KEY_ESCAPE) {
			pause_s();
		}

		else if (ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
			check = false;
			running = false;
			destroy();
		}
		

		al_draw_bitmap(board, 250, 0, 0);
		al_flip_display();
		al_clear_to_color(al_map_rgb(0, 0, 0));
	}

}
void medium() {

	if (music == true) {
		al_play_sample_instance(instance);
	}

	font = al_load_ttf_font("ariel.ttf", 80, 0);
	font2 = al_load_ttf_font("ariel.ttf", 30, 0);
	font3 = al_load_ttf_font("ariel.ttf", 60, 0);

	al_set_target_bitmap(board);
	al_clear_to_color(al_map_rgb(0, 0, 0));
	al_convert_mask_to_alpha(board, al_map_rgb(0, 0, 0));
	al_set_target_backbuffer(display);

	al_start_timer(timer);

	int pos_x = 0, pos_y = 0;
	int x[8] = { 0 }, y[8] = { 0 };
	int c = 0, c2 = 0, r = 0, r2 = 0, d = 0, z = 0, o = 0;
	int time = 0;
	FPS = 1;
	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			mtx1[i][j] = 2;
			locked1[i][j] = false;
		}
	}

	// here we put the locked boxes:
	levels_medium();
	//

	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			if (mtx1[i][j] != 2) {
				locked1[i][j] = true;
			}
		}
	}
	while (check == true)
	{


		bool won = true, won2 = true, won3 = true;

		al_draw_bitmap(background2, 0, 0, 0);
		al_draw_bitmap(pause_b, 1230, 600, 0);

		if (music == true) {
			al_draw_bitmap(music_on, 1230, 680, 0);
		}
		else if (music == false) {
			al_draw_bitmap(music_off, 1230, 680, 0);
		}
		al_wait_for_event(queue, &ev);

		for (float i = 0; i <= height; i += height / 8) {
			al_set_target_bitmap(board);
			al_draw_line(0, i, width - 500, i, al_map_rgb(255, 255, 255), 3);
			if (y[c] == 0) {
				y[c] = i;
				c++;
			}
		}
		c = 0;
		for (float j = 1; j <= width - 500; j += (width - 500) / 8) {
			al_draw_line(j, 0, j, height, al_map_rgb(255, 255, 255), 3);
			if (x[c] == 0) {
				x[c] = j;
				c++;
			}
		}
		if (d == 0) {
			for (c = 0; c < 8; c++) {
				for (c2 = 0; c2 < 8; c2++) {
					if (mtx1[c][c2] != 2 && locked1[c][c2] == true) {
						al_draw_textf(font3, al_map_rgb(255, 255, 255), x[c] + 44, y[c2] + 15, 0, "%d", mtx1[c][c2]);
					}
				}
			}
		}
		d = 1;

		al_set_timer_speed(timer, 1.0 / FPS);
		al_set_target_backbuffer(display);
		if (ev.type == ALLEGRO_EVENT_TIMER) {
			time++;

		}

		al_draw_textf(font2, al_map_rgb(255, 255, 255), 1150, 50, 0, "Time passed: %d", time);
		al_draw_textf(font2, al_map_rgb(255, 255, 255), 1150, 100, 0, "pos_x: %d", pos_x);
		al_draw_textf(font2, al_map_rgb(255, 255, 255), 1150, 150, 0, "pos_y: %d", pos_y);

		if (ev.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN) {
			if (ev.mouse.button & 1 && ev.mouse.x >= 251 && ev.mouse.x <= width - 250) {
				for (c = 0; c < 7; c++) {
					if (ev.mouse.x >= x[c] + 250 && ev.mouse.x < x[c + 1] + 250)
						break;
				}
				for (c2 = 0; c2 < 7; c2++) {
					if (ev.mouse.y >= y[c2] && ev.mouse.y < y[c2 + 1])
						break;
				}

				if (mtx1[c][c2] == 2 && locked1[c][c2] == false) {
					al_set_target_bitmap(board);
					al_draw_textf(font3, al_map_rgb(255, 255, 255), x[c] + 44, y[c2] + 15, 0, "0"); 
					al_play_sample(pok, 1, 0, 1, ALLEGRO_PLAYMODE_ONCE, NULL);
					mtx1[c][c2] = 0;
				}

				else if (mtx1[c][c2] == 0 && locked1[c][c2] == false) {
					al_set_target_bitmap(board);
					al_draw_filled_rectangle(x[c] + 30, y[c2] + 15, x[c] + 100, y[c2] + 80, al_map_rgb(0, 0, 0));
					al_convert_mask_to_alpha(board, al_map_rgb(0, 0, 0));
					al_draw_text(font3, al_map_rgb(255, 255, 255), x[c] + 44, y[c2] + 15, 0, "1");
					al_play_sample(pok, 1, 0, 1, ALLEGRO_PLAYMODE_ONCE, NULL);
					mtx1[c][c2]++;
				}
				else if (mtx1[c][c2] == 1 && locked1[c][c2] == false) {
					al_set_target_bitmap(board);
					al_draw_filled_rectangle(x[c] + 30, y[c2] + 15, x[c] + 100, y[c2] + 80, al_map_rgb(0, 0, 0));
					al_convert_mask_to_alpha(board, al_map_rgb(0, 0, 0));
					al_play_sample(tss, 1, 0, 1, ALLEGRO_PLAYMODE_ONCE, NULL);
					mtx1[c][c2]++;
				}

			}
			if (ev.mouse.x >= 1230 && ev.mouse.y >= 600 && ev.mouse.x <= 1280 && ev.mouse.y <= 650) {
				pause_s();
			}
			else if (ev.mouse.x >= 1230 && ev.mouse.y >= 680 && ev.mouse.x <= 1280 && ev.mouse.y <= 720) {
				if (music == true) {
					if (ev.mouse.button & 1) {
						al_stop_sample_instance(instance);
						al_draw_bitmap(music_on, 1230, 680, 0);
						music = false;
					}
				}
				else if (music == false) {
					if (ev.mouse.button & 1) {
						al_play_sample_instance(instance);
						al_draw_bitmap(music_off, 1230, 680, 0);
						music = true;
					}
				}
			}
		}
		al_set_target_backbuffer(display);

		if (ev.type == ALLEGRO_EVENT_MOUSE_AXES) {
			pos_x = ev.mouse.x;
			pos_y = ev.mouse.y;
		}
		for (int i = 0; i < 8; i++) {
			for (int j = 0; j < 8; j++) {
				if (mtx1[i][j] == 2) {
					won3 = false;
				}
			}
		}
		for (int c3 = 0; c3 < 7; c3++) {
			for (int c2 = c3 + 1; c2 < 8; c2++) {
				r = 0;
				for (int c = 0; c < 8; c++) {
					if (mtx1[c3][c] != 2 && mtx1[c3][c] == mtx1[c2][c]) {
						r++;
						if (r == 8) {
							if (ev.mouse.button & 1) {
								al_play_sample(error, 3, 0, 1, ALLEGRO_PLAYMODE_ONCE, NULL);
								al_draw_line(x[c3] + 250 + 54.5, 20, x[c3] + 250 + 54, 750, al_map_rgb(255, 0, 0), 7);
								al_draw_line(x[c2] + 250 + 54.5, 20, x[c2] + 250 + 54, 750, al_map_rgb(255, 0, 0), 7);
							}
							al_draw_line(x[c3] + 250 + 54.5, 20, x[c3] + 250 + 54, 750, al_map_rgb(255, 255, 255), 3);
							al_draw_line(x[c2] + 250 + 54.5, 20, x[c2] + 250 + 54, 750, al_map_rgb(255, 255, 255), 3);
							won = false;
						}

					}

				}


			}

		}

		for (int c3 = 0; c3 < 7; c3++) {
			for (int c2 = c3 + 1; c2 < 8; c2++) {
				r2 = 0;
				for (int c = 0; c < 8; c++) {
					if (mtx1[c][c3] != 2 && mtx1[c][c3] == mtx1[c][c2]) {
						r2++;
						if (r2 == 8) {
							if (ev.mouse.button & 1) {
								al_play_sample(error, 3, 0, 1, ALLEGRO_PLAYMODE_ONCE, NULL);
								al_draw_line(285, y[c3] + 50, 1086, y[c3] + 50, al_map_rgb(255, 0, 0), 7);
								al_draw_line(285, y[c2] + 50, 1086, y[c2] + 50, al_map_rgb(255, 0, 0), 7);
							}
							al_draw_line(285, y[c3] + 50, 1086, y[c3] + 50, al_map_rgb(255, 255, 255), 3);
							al_draw_line(285, y[c2] + 50, 1086, y[c2] + 50, al_map_rgb(255, 255, 255), 3);
							won = false;
						}

					}

				}


			}

		}

		for (c = 0; c < 8; c++) {
			for (c2 = 0; c2 < 6; c2++) {
				if (mtx1[c][c2] != 2 && mtx1[c][c2] == mtx1[c][c2 + 1] && mtx1[c][c2] == mtx1[c][c2 + 2]) {
					if (ev.mouse.button & 1) {
						al_play_sample(error, 3, 0, 1, ALLEGRO_PLAYMODE_ONCE, NULL);
						al_draw_line(x[c] + 250 + 53.9, y[c2] + 20, x[c] + 250 + 54, y[c2 + 2] + 80, al_map_rgb(255, 0, 0), 7);
					}
					al_draw_line(x[c] + 250 + 53.9, y[c2] + 20, x[c] + 250 + 54, y[c2 + 2] + 80, al_map_rgb(255, 255, 255), 3);					
					won2 = false;
				}
			}
		}

		for (c = 0; c < 8; c++) {
			for (c2 = 0; c2 < 6; c2++) {
				if (mtx1[c2][c] != 2 && mtx1[c2][c] == mtx1[c2 + 1][c] && mtx1[c2][c] == mtx1[c2 + 2][c]) {
					if (ev.mouse.button & 1) {
						al_play_sample(error, 3, 0, 1, ALLEGRO_PLAYMODE_ONCE, NULL);
						al_draw_line(x[c2] + 285, y[c] + 50, x[c2 + 2] + 320, y[c] + 50, al_map_rgb(255, 0, 0), 7);
					}
					al_draw_line(x[c2] + 285, y[c] + 50, x[c2 + 2] + 320, y[c] + 50, al_map_rgb(255, 255, 255), 3);
					won2 = false;
				}
			}
		}
		for (c = 0; c < 8; c++) {
			z = 0;
			o = 0;
			for (c2 = 0; c2 < 8; c2++) {
				if (mtx1[c2][c] == 1) {
					o++;
				}
				if (mtx1[c2][c] == 0) {
					z++;
				}
				if ((o > 4) || (z > 4)) {
					if (ev.mouse.button & 1) {
						al_play_sample(error, 3, 0, 1, ALLEGRO_PLAYMODE_ONCE, NULL);
						al_draw_line(280, y[c] + 50, 1086, y[c] + 50, al_map_rgb(255, 0, 0), 7);
					}
					al_draw_line(280, y[c] + 50, 1086, y[c] + 50, al_map_rgb(255, 255, 255), 3);
					won2 = false;
				}
			}
		}
		for (c2 = 0; c2 < 8; c2++) {
			z = 0;
			o = 0;
			for (c = 0; c < 8; c++) {
				if (mtx1[c2][c] == 1) {
					o++;
				}
				if (mtx1[c2][c] == 0) {
					z++;
				}
				if ((o > 4) || (z > 4)) {
					if (ev.mouse.button & 1) {
						al_play_sample(error, 3, 0, 1, ALLEGRO_PLAYMODE_ONCE, NULL);
						al_draw_line(x[c2] + 250 + 53.9, 20, x[c2] + 250 + 53.9, 750, al_map_rgb(255, 0, 0), 7);
					}
					al_draw_line(x[c2] + 250 + 53.9, 20, x[c2] + 250 + 53.9, 750, al_map_rgb(255, 255, 255), 3);
					won2 = false;
				}
			}
		}

		if (won == true && won2 == true && won3 == true) {
			win();
		}

		if (ev.keyboard.keycode == ALLEGRO_KEY_ESCAPE) {
			pause_s();

		}

		else if (ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
			check = false;
			running = false;
			destroy();
		}


		al_draw_bitmap(board, 250, 0, 0);
		al_flip_display();
		al_clear_to_color(al_map_rgb(0, 0, 0));
	}

}
void hard() {

	if (music == true) {
		al_play_sample_instance(instance);
	}

	font = al_load_ttf_font("ariel.ttf", 80, 0);
	font2 = al_load_ttf_font("ariel.ttf", 30, 0);
	font3 = al_load_ttf_font("ariel.ttf", 50, 0);

	al_set_target_bitmap(board);
	al_clear_to_color(al_map_rgb(0, 0, 0));
	al_convert_mask_to_alpha(board, al_map_rgb(0, 0, 0));
	al_set_target_backbuffer(display);

	al_start_timer(timer);

	int pos_x = 0, pos_y = 0;
	int x[10] = { 0 }, y[10] = { 0 };
	int c = 0, c2 = 0, r = 0, r2 = 0, d = 0, z = 0, o = 0;
	int time = 0;
	FPS = 1;

	for (int i = 0; i < 10; i++) {
		for (int j = 0; j < 10; j++) {
			mtx2[i][j] = 2;
			locked2[i][j] = false;
		}
	}
	// here we put the level:
	levels_hard();
	//

	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			if (mtx2[i][j] != 2) {
				locked2[i][j] = true;
			}
		}
	}
	while (check)
	{
		if (music == true) {
			al_play_sample_instance(instance);
		}

		bool won = true, won2 = true, won3 = true;

		al_draw_bitmap(background2, 0, 0, 0);
		al_draw_bitmap(pause_b, 1230, 600, 0);

		if (music == true) {
			al_draw_bitmap(music_on, 1230, 680, 0);
		}
		else if (music == false) {
			al_draw_bitmap(music_off, 1230, 680, 0);
		}

		al_wait_for_event(queue, &ev);

		for (float i = 1; i <= height; i += (height / 10) + 0.5) {
			al_set_target_bitmap(board);
			al_draw_line(0, i, width - 500, i, al_map_rgb(255, 255, 255), 5);
			if (y[c] == 0) {
				y[c] = i + 1;
				c++;
			}
		}
		c = 0;
		for (float j = 1; j <= width - 500; j += ((width - 500) / 10) + 0.5) {
			al_draw_line(j, 0, j, height, al_map_rgb(255, 255, 255), 5);
			if (x[c] == 0) {
				x[c] = j + 1;
				c++;
			}
		}
		if (d == 0) {
			for (c = 0; c < 10; c++) {
				for (c2 = 0; c2 < 10; c2++) {
					if (mtx2[c][c2] != 2 && locked2[c][c2] == true) {
						al_draw_textf(font3, al_map_rgb(255, 255, 255), x[c] + 34, y[c2] + 7, 0, "%d", mtx2[c][c2]);
					}
				}
			}
		}
		d = 1;

		al_set_timer_speed(timer, 1.0 / FPS);
		al_set_target_backbuffer(display);

		if (ev.type == ALLEGRO_EVENT_TIMER) {
			time++;
		}

		al_draw_textf(font2, al_map_rgb(255, 255, 255), 1150, 50, 0, "Time passed: %d", time);
		al_draw_textf(font2, al_map_rgb(255, 255, 255), 1150, 100, 0, "pos_x: %d", pos_x);
		al_draw_textf(font2, al_map_rgb(255, 255, 255), 1150, 150, 0, "pos_y: %d", pos_y);


		if (ev.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN) {
			if (ev.mouse.button & 1 && ev.mouse.x >= 251 && ev.mouse.x <= width - 250) {
				for (c = 0; c < 9; c++) {
					if (ev.mouse.x >= x[c] + 250 && ev.mouse.x < x[c + 1] + 250)
						break;
				}
				for (c2 = 0; c2 < 9; c2++) {
					if (ev.mouse.y >= y[c2] && ev.mouse.y < y[c2 + 1])
						break;
				}

				if (mtx2[c][c2] == 2 && locked2[c][c2] == false) {
					al_set_target_bitmap(board);
					al_draw_textf(font3, al_map_rgb(255, 255, 255), x[c] + 34, y[c2] + 7, 0, "0");
					al_play_sample(pok, 1, 0, 1, ALLEGRO_PLAYMODE_ONCE, NULL);
					mtx2[c][c2] = 0;
				}

				else if (mtx2[c][c2] == 0 && locked2[c][c2] == false) {
					al_set_target_bitmap(board);
					al_draw_filled_rectangle(x[c] + 10, y[c2] + 10, x[c] + 70, y[c2] + 70, al_map_rgb(0, 0, 0));
					al_convert_mask_to_alpha(board, al_map_rgb(0, 0, 0));
					al_draw_text(font3, al_map_rgb(255, 255, 255), x[c] + 34, y[c2] + 7, 0, "1");
					al_play_sample(pok, 1, 0, 1, ALLEGRO_PLAYMODE_ONCE, NULL);
					mtx2[c][c2]++;
				}
				else if (mtx2[c][c2] == 1 && locked2[c][c2] == false) {
					al_set_target_bitmap(board);
					al_draw_filled_rectangle(x[c] + 10, y[c2] + 10, x[c] + 70, y[c2] + 70, al_map_rgb(0, 0, 0));
					al_convert_mask_to_alpha(board, al_map_rgb(0, 0, 0));
					al_play_sample(tss, 1, 0, 1, ALLEGRO_PLAYMODE_ONCE, NULL);
					mtx2[c][c2]++;

				}

			}
			if (ev.mouse.x >= 1230 && ev.mouse.y >= 600 && ev.mouse.x <= 1280 && ev.mouse.y <= 650) {
				pause_s();
			}
			else if (ev.mouse.x >= 1230 && ev.mouse.y >= 680 && ev.mouse.x <= 1280 && ev.mouse.y <= 720) {
				if (music == true) {
					if (ev.mouse.button & 1) {
						al_stop_sample_instance(instance);
						al_draw_bitmap(music_off, 1230, 680, 0);
						music = false;
					}
				}
				else if (music == false) {
					if (ev.mouse.button & 1) {
						al_play_sample_instance(instance);
						al_draw_bitmap(music_on, 1230, 680, 0);
						music = true;
					}
				}
			}
		}
		al_set_target_backbuffer(display);

		if (ev.type == ALLEGRO_EVENT_MOUSE_AXES) {
			pos_x = ev.mouse.x;
			pos_y = ev.mouse.y;
		}
		for (int i = 0; i < 10; i++) {
			for (int j = 0; j < 10; j++) {
				if (mtx2[i][j] == 2) {
					won3 = false;
				}
			}
		}
		for (int c3 = 0; c3 < 9; c3++) {
			for (int c2 = c3 + 1; c2 < 10; c2++) {
				r = 0;
				for (int c = 0; c < 10; c++) {
					if (mtx2[c3][c] != 2 && mtx2[c3][c] == mtx2[c2][c]) {
						r++;
						if (r == 10) {
							if (ev.mouse.button & 1) {
								al_play_sample(error, 3, 0, 1, ALLEGRO_PLAYMODE_ONCE, NULL);
								al_draw_line(x[c3] + 250 + 43, 15, x[c3] + 250 + 43, 750, al_map_rgb(255, 0, 0), 7);
								al_draw_line(x[c2] + 250 + 43, 15, x[c2] + 250 + 43, 750, al_map_rgb(255, 0, 0), 7);
							}
							al_draw_line(x[c3] + 250 + 43, 15, x[c3] + 250 + 43, 750, al_map_rgb(255, 255, 255), 3);
							al_draw_line(x[c2] + 250 + 43, 15, x[c2] + 250 + 43, 750, al_map_rgb(255, 255, 255), 3);
							won = false;
						}

					}

				}


			}

		}

		for (int c3 = 0; c3 < 9; c3++) {
			for (int c2 = c3 + 1; c2 < 10; c2++) {
				r2 = 0;
				for (int c = 0; c < 10; c++) {
					if (mtx2[c][c3] != 2 && mtx2[c][c3] == mtx2[c][c2]) {
						r2++;
						if (r2 == 10) {
							if (ev.mouse.button & 1) {
								al_play_sample(error, 3, 0, 1, ALLEGRO_PLAYMODE_ONCE, NULL);
								al_draw_line(280, y[c3] + 35, 1093, y[c3] + 35, al_map_rgb(255, 0, 0), 7);
								al_draw_line(280, y[c2] + 35, 1093, y[c2] + 35, al_map_rgb(255, 0, 0), 7);
							}
							al_draw_line(280, y[c3] + 35, 1093, y[c3] + 35, al_map_rgb(255, 255, 255), 3);
							al_draw_line(280, y[c2] + 35, 1093, y[c2] + 35, al_map_rgb(255, 255, 255), 3);
							won = false;
						}

					}

				}


			}

		}

		for (c = 0; c < 10; c++) {
			for (c2 = 0; c2 < 8; c2++) {
				if (mtx2[c][c2] != 2 && mtx2[c][c2] == mtx2[c][c2 + 1] && mtx2[c][c2] == mtx2[c][c2 + 2]) {
					if (ev.mouse.button & 1) {
						al_play_sample(error, 3, 0, 1, ALLEGRO_PLAYMODE_ONCE, NULL);
						al_draw_line(x[c] + 250 + 43, y[c2] + 10, x[c] + 250 + 43, y[c2 + 2] + 60, al_map_rgb(255, 0, 0), 7);
					}
					al_draw_line(x[c] + 250 + 43, y[c2] + 10, x[c] + 250 + 43, y[c2 + 2] + 60, al_map_rgb(255, 255, 255), 3);
					won2 = false;
				}
			}
		}

		for (c = 0; c < 10; c++) {
			for (c2 = 0; c2 < 8; c2++) {
				if (mtx2[c2][c] != 2 && mtx2[c2][c] == mtx2[c2 + 1][c] && mtx2[c2][c] == mtx2[c2 + 2][c]) {
					if (ev.mouse.button & 1) {
						al_play_sample(error, 3, 0, 1, ALLEGRO_PLAYMODE_ONCE, NULL);
						al_draw_line(x[c2] + 275, y[c] + 35, x[c2 + 2] + 310, y[c] + 35, al_map_rgb(255, 0, 0), 7);
					}
					al_draw_line(x[c2] + 275, y[c] + 35, x[c2 + 2] + 310, y[c] + 35, al_map_rgb(255, 255, 255), 3);
					won2 = false;
				}
			}
		}
		for (c = 0; c < 10; c++) {
			z = 0;
			o = 0;
			for (c2 = 0; c2 < 10; c2++) {
				if (mtx2[c2][c] == 1) {
					o++;
				}
				if (mtx2[c2][c] == 0) {
					z++;
				}
				if ((o > 5) || (z > 5)) {
					if (ev.mouse.button & 1) {
						al_play_sample(error, 3, 0, 1, ALLEGRO_PLAYMODE_ONCE, NULL);
						al_draw_line(280, y[c] + 35, 1093, y[c] + 35, al_map_rgb(255, 0, 0), 7);
					}
					al_draw_line(280, y[c] + 35, 1093, y[c] + 35, al_map_rgb(255, 255, 255), 3);
					won2 = false;
				}
			}
		}
		for (c2 = 0; c2 < 10; c2++) {
			z = 0;
			o = 0;
			for (c = 0; c < 10; c++) {
				if (mtx2[c2][c] == 1) {
					o++;
				}
				if (mtx2[c2][c] == 0) {
					z++;
				}
				if ((o > 5) || (z > 5)) {
					if (ev.mouse.button & 1) {
						al_play_sample(error, 3, 0, 1, ALLEGRO_PLAYMODE_ONCE, NULL);
						al_draw_line(x[c2] + 250 + 43, 10, x[c2] + 250 + 43, 750, al_map_rgb(255, 0, 0), 7);
					}
					al_draw_line(x[c2] + 250 + 43, 10, x[c2] + 250 + 43, 750, al_map_rgb(255, 255, 255), 3);
					won2 = false;
				}
			}
		}

		if (won == true && won2 == true && won3 == true) {
			win();
		}

		if (ev.keyboard.keycode == ALLEGRO_KEY_ESCAPE) {
			pause_s();
		}

		else if (ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
			check = false;
			running = false;
			destroy();
		}


		al_draw_bitmap(board, 250, 0, 0);
		al_flip_display();
		al_clear_to_color(al_map_rgb(0, 0, 0));
	}

}

void win() {
	if (difficulity == 1) {
		for (int i = 0; i < 6; i++) {
			for (int j = 0; j < 6; j++) {
				locked[i][j] = true;
			}
		}
	}
	else if (difficulity == 2) {
		for (int i = 0; i < 8; i++) {
			for (int j = 0; j < 8; j++) {
				locked1[i][j] = true;
			}
		}
	}
	else if (difficulity == 1) {
		for (int i = 0; i < 10; i++) {
			for (int j = 0; j < 10; j++) {
				locked2[i][j] = true;
			}
		}
	}	
	al_stop_sample_instance(instance);
	
	while (check) {
		
		al_play_sample(end, 1, 0, 1, ALLEGRO_PLAYMODE_ONCE, NULL);
		al_wait_for_event(queue, &ev);
		
		al_draw_bitmap(won, (width / 2) - 200, (height / 2) - 150, 0);
		al_draw_text(font, al_map_rgb(0, 0, 0), (width / 2) - 185, (height / 2) - 140, 0, "   well done !");
		al_draw_text(font2, al_map_rgb(0, 0, 0), (width / 2) - 190, (height / 2), 0, "Back to main menu");
		al_draw_bitmap(back_to_menu, (width / 2) - 150, (height / 2) + 40, 0);
		al_draw_text(font2, al_map_rgb(0, 0, 0), (width / 2) + 45, (height / 2), 0, "Next level");
		al_draw_bitmap(next_level, (width / 2) + 50, (height / 2) + 40, 0);

		al_stop_timer(timer);
		if ((ev.mouse.x >= (width / 2) - 150) && (ev.mouse.x <= (width / 2) - 70) && (ev.mouse.y >= (height / 2) + 40) && (ev.mouse.y <= ((height / 2) + 120))) {
			al_draw_text(font2, al_map_rgb(255, 0, 0), (width / 2) - 190, (height / 2), 0, "Back to main menu");
			if (ev.mouse.button & 1) {
				al_play_sample(click, 1, 0, 1, ALLEGRO_PLAYMODE_ONCE, NULL);
				check = false;
			}
		}
		else if ((ev.mouse.x >= (width / 2) + 50) && (ev.mouse.x <= (width / 2) + 130) && (ev.mouse.y >= (height / 2) + 40) && (ev.mouse.y <= ((height / 2) + 120))) {
			al_draw_text(font2, al_map_rgb(0, 255, 0), (width / 2) + 45, (height / 2), 0, "Next level");
			if (ev.mouse.button & 1) {
				al_play_sample(click, 1, 0, 1, ALLEGRO_PLAYMODE_ONCE, NULL);
				if (level != 8) {
					if (difficulity == 1) {
						level++;
						easy();
					}
					else if (difficulity == 2) {
						level++;
						medium();
					}
					else if (difficulity == 3) {
						level++;
						hard();
					}

				}
				else if (level == 8) {
					if (difficulity == 1) {
						level = 0;
						medium();
					}
					else if (difficulity == 2) {
						level = 0;
						hard();
					}
					else if (difficulity == 3) {
						level = 0;
						easy();
					}
				}
			}
		}
		al_flip_display();
	}
}
void pause_s() {
	bool pause = true;

	al_draw_bitmap(pause_m, (width / 2) - 240, (height / 2) - 160, 0);

	while (pause)
	{
		al_wait_for_event(queue, &ev);

		if (ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
			destroy();
		}

		if (ev.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN) {

			if ((ev.mouse.button & 1) && (ev.mouse.x >= (width / 2) - 63) && (ev.mouse.x <= (width / 2) + 63) && (ev.mouse.y >= (height / 2) - 37) && (ev.mouse.y <= ((height / 2) - 3))) {
				pause = false;
			}
			else if ((ev.mouse.x >= (width / 2) - 63) && (ev.mouse.x <= (width / 2) + 63) && (ev.mouse.y >= (height / 2) + 16) && (ev.mouse.y <= ((height / 2) + 50))) {
				pause = false;
				if (difficulity == 1) {
					easy();
				}
				else if (difficulity == 2) {
					medium();
				}
				else if (difficulity == 3) {
					hard();
				}
			}
			else if ((ev.mouse.x >= (width / 2) - 63) && (ev.mouse.x <= (width / 2) + 63) && (ev.mouse.y >= (height / 2) + 73) && (ev.mouse.y <= ((height / 2) + 107))) {
				pause = false;
				check = false;
				al_stop_samples();
				al_stop_sample_instance(instance);
				al_stop_timer(timer);
			}
		}
		al_flip_display();
	}
}

int destroy() {

	check = false;
	running = false;
	
	al_uninstall_keyboard();
	al_uninstall_mouse();

	al_destroy_display(display);
	al_destroy_event_queue(queue);
	al_destroy_timer(timer);

	al_destroy_font(font);
	al_destroy_font(font2);
	al_destroy_font(font3);

	al_destroy_bitmap(background);
	al_destroy_bitmap(background2);
	al_destroy_bitmap(board);
	al_destroy_bitmap(choose_size);
	al_destroy_bitmap(pause_b);
	al_destroy_bitmap(pause_m);
	al_destroy_bitmap(music_off);
	al_destroy_bitmap(music_on);
	al_destroy_bitmap(won);



	al_destroy_sample(click);
	al_destroy_sample(error);
	al_destroy_sample(song);
	al_destroy_sample(end);
	al_destroy_sample(pok);
	al_destroy_sample(tss);
	al_destroy_sample_instance(instance);

	return 0;
	
}

void levels_easy() {
	for (int i = 0; i < 6; i++) {
		for (int j = 0; j < 6; j++) {			
			levels[level][i][j] = 2;			
		}
	}

	//level 1
	{
	levels[0][0][0] = 1;
	levels[0][1][1] = 0;
	levels[0][2][1] = 1;
	levels[0][3][1] = 0;
	levels[0][3][2] = 0;
	levels[0][5][2] = 0;
	levels[0][1][3] = 0;
	levels[0][1][4] = 1;
	levels[0][2][4] = 0;
	levels[0][0][5] = 0;
	levels[0][2][5] = 0;
	levels[0][4][5] = 1;
	}	
	//level 2
	{
		levels[1][0][0] = 1;
		levels[1][1][1] = 0;
		levels[1][2][1] = 1;
		levels[1][3][0] = 1;
		levels[1][4][0] = 1;
		levels[1][3][2] = 0;
		levels[1][5][2] = 0;
		levels[1][1][4] = 1;
		levels[1][2][4] = 1;
		levels[1][2][5] = 1;
		levels[1][5][5] = 0;		
	}
	//level 3
	{
		levels[2][1][0] = 0;
		levels[2][5][0] = 1;
		levels[2][0][2] = 0;
		levels[2][3][2] = 0;
		levels[2][5][2] = 0;
		levels[2][0][3] = 0;
		levels[2][3][3] = 1;
		levels[2][5][3] = 0;
		levels[2][2][5] = 0;
		levels[2][4][5] = 0;
	}
	//level 4
	{
	levels[3][1][0] = 0;
	levels[3][3][0] = 0;
	levels[3][4][1] = 0;
	levels[3][4][2] = 1;
	levels[3][1][3] = 1;
	levels[3][0][4] = 0;
	levels[3][4][4] = 0;
	levels[3][2][5] = 1;
	levels[3][3][5] = 0;
	}
	//level 5
	{
		levels[4][1][0] = 0;
		levels[4][2][0] = 0;
		levels[4][3][1] = 1;
		levels[4][5][1] = 1;
		levels[4][0][3] = 0;
		levels[4][1][3] = 0;
		levels[4][4][3] = 0;
		levels[4][5][3] = 1;
	}
	//level 6
	{
		levels[5][1][0] = 1;
		levels[5][1][1] = 1;
		levels[5][5][0] = 0;
		levels[5][5][2] = 0;
		levels[5][5][3] = 0;
		levels[5][3][3] = 0;
		levels[5][4][5] = 1;
	}
	//level 7
	{
		levels[6][4][0] = 1;
		levels[6][5][0] = 1;
		levels[6][2][2] = 0;
		levels[6][5][2] = 0;
		levels[6][3][4] = 1;
		levels[6][4][4] = 1;
		levels[6][0][5] = 1;
		levels[6][2][5] = 0;
	}
	//level 8
	{
		levels[7][0][0] = 1;
		levels[7][5][1] = 1;
		levels[7][5][2] = 1;
		levels[7][0][2] = 1;
		levels[7][3][2] = 1;
		levels[7][4][3] = 0;
		levels[7][2][5] = 1;
	}
	//level 9
	{
		levels[8][0][0] = 0;
		levels[8][1][0] = 0;
		levels[8][4][0] = 0;
		levels[8][3][1] = 1;
		levels[8][5][1] = 1;
		levels[8][0][2] = 1;
		levels[8][4][3] = 0;
		levels[8][2][4] = 1;
	}
	for (int i = 0; i < 6; i++) {
		for (int j = 0; j < 6; j++) {
			if (levels[level][i][j] != 2) {
				mtx[i][j] = levels[level][i][j];
				locked[i][j] = true;
			}
		}
	}
}
void levels_medium() {
	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			levels1[level][i][j] = 2;
		}
	}

	// level 1
	{
		levels1[0][3][0] = 0;
		levels1[0][4][0] = 0;
		levels1[0][0][1] = 0;
		levels1[0][2][1] = 1;
		levels1[0][3][1] = 0;
		levels1[0][5][1] = 0;
		levels1[0][7][1] = 1;
		levels1[0][1][2] = 1;
		levels1[0][2][2] = 0;
		levels1[0][3][2] = 1;
		levels1[0][1][3] = 1;
		levels1[0][3][4] = 1;
		levels1[0][5][4] = 0;
		levels1[0][7][4] = 0;
		levels1[0][0][5] = 1;
		levels1[0][3][5] = 0;
		levels1[0][6][5] = 1;
		levels1[0][2][6] = 0;
		levels1[0][4][6] = 0;
		levels1[0][5][6] = 1;
		levels1[0][1][7] = 0;
		levels1[0][5][7] = 0;
	}
	// level 2
	{
		levels1[1][1][0] = 1;
		levels1[1][4][0] = 0;
		levels1[1][0][1] = 0;
		levels1[1][4][1] = 0;
		levels1[1][5][1] = 1;
		levels1[1][0][2] = 1;
		levels1[1][1][2] = 0;
		levels1[1][2][2] = 0;
		levels1[1][6][2] = 0;
		levels1[1][2][3] = 0;
		levels1[1][7][3] = 0;
		levels1[1][1][4] = 0;
		levels1[1][4][4] = 0;
		levels1[1][0][5] = 0;
		levels1[1][1][5] = 0;
		levels1[1][2][5] = 1;
		levels1[1][7][5] = 1;
		levels1[1][2][6] = 0;
		levels1[1][5][6] = 1;
		levels1[1][6][6] = 0;
		levels1[1][5][7] = 1;
		levels1[1][6][7] = 1;
	}
	// level 3
	{
		levels1[2][1][0] = 0;
		levels1[2][2][0] = 0;
		levels1[2][0][2] = 0;
		levels1[2][1][2] = 0;
		levels1[2][5][2] = 0;
		levels1[2][0][3] = 0;
		levels1[2][1][3] = 1;
		levels1[2][5][3] = 1;
		levels1[2][2][4] = 0;
		levels1[2][6][4] = 0;
		levels1[2][7][4] = 1;
		levels1[2][0][5] = 1;
		levels1[2][2][5] = 0;
		levels1[2][7][4] = 1;
		levels1[2][0][5] = 1;
		levels1[2][2][5] = 0;
		levels1[2][4][5] = 1;
		levels1[2][5][5] = 1;
		levels1[2][4][6] = 0;
		levels1[2][5][6] = 0;
		levels1[2][6][6] = 1;
		levels1[2][4][7] = 1;
	}
	// level 4
	{
		levels1[3][0][1] = 0;
		levels1[3][2][1] = 1;
		levels1[3][0][2] = 0;
		levels1[3][2][2] = 1;
		levels1[3][3][2] = 0;
		levels1[3][6][2] = 0;
		levels1[3][3][3] = 0;
		levels1[3][0][4] = 0;
		levels1[3][6][4] = 0;
		levels1[3][2][5] = 1;
		levels1[3][4][5] = 0;
		levels1[3][5][6] = 0;
		levels1[3][7][6] = 1;
		levels1[3][0][7] = 0;
		levels1[3][6][7] = 0;
		levels1[3][7][7] = 0;
	}
	// level 5
	{
		levels1[4][4][0] = 1;
		levels1[4][5][0] = 1;
		levels1[4][7][0] = 1;
		levels1[4][0][1] = 1;
		levels1[4][5][1] = 0;
		levels1[4][3][2] = 0;
		levels1[4][6][2] = 1;
		levels1[4][5][3] = 0;
		levels1[4][0][4] = 1;
		levels1[4][3][4] = 1;
		levels1[4][6][4] = 0;
		levels1[4][4][5] = 1;
		levels1[4][6][5] = 0;
		levels1[4][1][6] = 1;
		levels1[4][7][6] = 1;
		levels1[4][2][7] = 0;
	}
	// level 6
	{
		levels1[5][6][0] = 1;
		levels1[5][3][1] = 0;
		levels1[5][4][1] = 0;
		levels1[5][7][1] = 0;
		levels1[5][1][2] = 1;
		levels1[5][1][3] = 1;
		levels1[5][4][3] = 0;
		levels1[5][5][3] = 0;
		levels1[5][7][3] = 1;
		levels1[5][0][4] = 0;
		levels1[5][4][6] = 0;
		levels1[5][2][7] = 1;
	}
	// level 7
	{
		levels1[6][1][0] = 1;
		levels1[6][7][2] = 1;
		levels1[6][0][3] = 0;
		levels1[6][4][3] = 0;
		levels1[6][7][3] = 0;
		levels1[6][0][4] = 0;
		levels1[6][2][4] = 0;
		levels1[6][4][4] = 0;
		levels1[6][7][5] = 1;
		levels1[6][3][6] = 1;
		levels1[6][2][7] = 1;
		levels1[6][3][7] = 1;
	}
	// level 8
	{
		levels1[7][0][1] = 0;
		levels1[7][6][1] = 1;
		levels1[7][7][1] = 1;
		levels1[7][1][2] = 1;
		levels1[7][5][2] = 0;
		levels1[7][3][3] = 1;
		levels1[7][0][4] = 0;
		levels1[7][2][4] = 0;
		levels1[7][3][5] = 1;
		levels1[7][4][6] = 0;
		levels1[7][5][6] = 0;
		levels1[7][7][6] = 1;
		levels1[7][0][7] = 0;
		levels1[7][2][7] = 0;
	}
	// level 9
	{
		levels1[8][4][0] = 0;
		levels1[8][6][0] = 0;
		levels1[8][1][1] = 1;
		levels1[8][6][1] = 0;
		levels1[8][1][2] = 1;
		levels1[8][2][2] = 1;
		levels1[8][5][2] = 0;
		levels1[8][0][3] = 0;
		levels1[8][1][4] = 0;
		levels1[8][3][5] = 1;
		levels1[8][3][6] = 0;
		levels1[8][2][7] = 1;
	}

	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			if (levels1[level][i][j] != 2) {
				mtx1[i][j] = levels1[level][i][j];
				locked1[i][j] = true;
			}
		}
	}
}
void levels_hard() {
	for (int i = 0; i < 10; i++) {
		for (int j = 0; j < 10; j++) {
			levels2[level][i][j] = 2;
		}
	}

	// level 1
	{
		levels2[0][1][0] = 0;
		levels2[0][4][0] = 1;
		levels2[0][2][1] = 0;
		levels2[0][4][1] = 1;
		levels2[0][0][2] = 1;
		levels2[0][1][2] = 1;
		levels2[0][1][3] = 1;
		levels2[0][7][3] = 0;
		levels2[0][0][4] = 0;
		levels2[0][5][4] = 0;
		levels2[0][9][5] = 1;
		levels2[0][3][6] = 1;
		levels2[0][5][6] = 0;
		levels2[0][6][6] = 0;
		levels2[0][5][7] = 1;
		levels2[0][6][7] = 1;
		levels2[0][2][8] = 0;
		levels2[0][4][9] = 0;
		
	}
	// level 2
	{
		levels2[1][5][0] = 1;
		levels2[1][6][0] = 1;
		levels2[1][9][0] = 1;
		levels2[1][4][1] = 1;
		levels2[1][0][2] = 1;
		levels2[1][1][2] = 1;
		levels2[1][1][3] = 1;
		levels2[1][7][3] = 0;
		levels2[1][0][4] = 0;
		levels2[1][5][4] = 0;
		levels2[1][9][5] = 1;
		levels2[1][3][6] = 1;
		levels2[1][5][6] = 0;
		levels2[1][6][6] = 0;
		levels2[1][5][7] = 1;
		levels2[1][6][7] = 1;
		levels2[1][2][8] = 0;
		levels2[1][4][9] = 0;
	}
	// level 3
	{
		levels2[2][5][0] = 1;
		levels2[2][6][0] = 1;
		levels2[2][9][0] = 1;
		levels2[2][6][1] = 1;
		levels2[2][0][2] = 0;
		levels2[2][3][2] = 1;
		levels2[2][1][3] = 1;
		levels2[2][4][3] = 0;
		levels2[2][7][3] = 1;
		levels2[2][6][4] = 1;
		levels2[2][7][4] = 1;
		levels2[2][9][5] = 0;
		levels2[2][3][6] = 0;
		levels2[2][5][6] = 0;
		levels2[2][9][6] = 0;
		levels2[2][1][7] = 0;
		levels2[2][6][7] = 1;
		levels2[2][8][7] = 1;
		levels2[2][1][8] = 0;
		levels2[2][3][8] = 1;
		levels2[2][7][8] = 0;
		levels2[2][0][9] = 0;
	}
	// level 4
	{
		levels2[3][1][0] = 1;
		levels2[3][4][1] = 0;
		levels2[3][0][2] = 1;
		levels2[3][6][2] = 1;
		levels2[3][1][3] = 0;
		levels2[3][2][3] = 1;
		levels2[3][5][3] = 0;
		levels2[3][6][3] = 0;
		levels2[3][8][3] = 0;
		levels2[3][2][4] = 1;
		levels2[3][9][4] = 1;
		levels2[3][5][5] = 1;
		levels2[3][0][6] = 1;
		levels2[3][1][6] = 1;
		levels2[3][3][6] = 1;
		levels2[3][8][6] = 0;
		levels2[3][0][7] = 1;
		levels2[3][7][7] = 1;
		levels2[3][9][7] = 1;
		levels2[3][9][8] = 1;
		levels2[3][6][9] = 0;
	}
	// level 5
	{
		levels2[4][1][0] = 0;
		levels2[4][3][0] = 1;
		levels2[4][5][0] = 0;
		levels2[4][6][0] = 0;
		levels2[4][0][1] = 1;
		levels2[4][4][1] = 1;
		levels2[4][6][1] = 0;
		levels2[4][8][1] = 0;
		levels2[4][3][2] = 0;
		levels2[4][5][3] = 1;
		levels2[4][8][3] = 0;
		levels2[4][9][3] = 0;
		levels2[4][3][4] = 0;
		levels2[4][9][4] = 0;
		levels2[4][4][5] = 1;
		levels2[4][2][7] = 0;
		levels2[4][7][7] = 1;
		levels2[4][2][8] = 0;
		levels2[4][3][8] = 0;
		levels2[4][8][9] = 1;
	}
	// level 6
	{
		levels2[5][1][0] = 0;
		levels2[5][2][0] = 0;
		levels2[5][8][0] = 1;
		levels2[5][2][1] = 0;
		levels2[5][6][1] = 0;
		levels2[5][7][1] = 0;
		levels2[5][1][2] = 1;
		levels2[5][4][2] = 0;
		levels2[5][6][2] = 0;
		levels2[5][1][3] = 1;
		levels2[5][2][3] = 1;
		levels2[5][4][4] = 1;
		levels2[5][0][5] = 0;
		levels2[5][1][5] = 0;
		levels2[5][6][6] = 0;
		levels2[5][8][6] = 1;
		levels2[5][9][7] = 0;
		levels2[5][3][8] = 0;
		levels2[5][2][9] = 1;
		levels2[5][4][9] = 1;
		levels2[5][5][9] = 0;
	}
	// level 7
	{
		levels2[6][2][0] = 0;
		levels2[6][6][0] = 0;
		levels2[6][9][0] = 0;
		levels2[6][3][1] = 1;
		levels2[6][5][1] = 0;
		levels2[6][1][2] = 0;
		levels2[6][4][2] = 0;
		levels2[6][5][2] = 1;
		levels2[6][1][3] = 0;
		levels2[6][2][3] = 0;
		levels2[6][4][3] = 0;
		levels2[6][5][3] = 0;
		levels2[6][0][4] = 1;
		levels2[6][4][5] = 1;
		levels2[6][8][5] = 1;
		levels2[6][1][6] = 0;
		levels2[6][2][7] = 0;
		levels2[6][3][7] = 1;
		levels2[6][5][7] = 1;
		levels2[6][9][7] = 0;
		levels2[6][8][8] = 0;
		levels2[6][9][8] = 0;
		levels2[6][0][9] = 0;
		levels2[6][6][9] = 0;
		levels2[6][8][9] = 0;
	}
	// level 8
	{
		levels2[7][2][2] = 0;
		levels2[7][3][2] = 0;
		levels2[7][7][2] = 1;
		levels2[7][8][2] = 1;
		levels2[7][0][3] = 0;
		levels2[7][4][3] = 0;
		levels2[7][5][3] = 0;
		levels2[7][2][4] = 0;
		levels2[7][8][4] = 0;
		levels2[7][0][5] = 1;
		levels2[7][1][5] = 1;
		levels2[7][4][5] = 0;
		levels2[7][5][5] = 1;
		levels2[7][2][7] = 0;
		levels2[7][3][7] = 0;
		levels2[7][1][8] = 1;
		levels2[7][6][8] = 0;
		levels2[7][0][9] = 1;
		levels2[7][2][9] = 1;
		levels2[7][7][9] = 1;
		levels2[7][9][9] = 1;
		levels2[7][0][6] = 1;
		levels2[7][7][5] = 0;
	}
	// level 9
	{
		levels2[8][7][0] = 1;
		levels2[8][9][0] = 1;
		levels2[8][2][1] = 0;
		levels2[8][6][1] = 0;
		levels2[8][6][2] = 0;
		levels2[8][8][2] = 1;
		levels2[8][9][2] = 0;
		levels2[8][0][3] = 0;
		levels2[8][3][3] = 0;
		levels2[8][1][4] = 1;
		levels2[8][2][4] = 1;
		levels2[8][4][4] = 1;
		levels2[8][9][4] = 1;
		levels2[8][4][5] = 0;
		levels2[8][8][5] = 0;
		levels2[8][7][6] = 0;
		levels2[8][8][6] = 1;
		levels2[8][9][6] = 0;
		levels2[8][2][7] = 1;
		levels2[8][5][7] = 1;
		levels2[8][9][7] = 1;
		levels2[8][5][8] = 0;
		levels2[8][9][8] = 1;
		levels2[8][2][9] = 1;
		levels2[8][6][9] = 0;
	}
	for (int i = 0; i < 10; i++) {
		for (int j = 0; j < 10; j++) {
			if (levels2[level][i][j] != 2) {
				mtx2[i][j] = levels2[level][i][j];
				locked2[i][j] = true;
			}
		}
	}
}