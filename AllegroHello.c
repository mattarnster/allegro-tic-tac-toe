// AllegroHello.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <string.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_image.h>


// Game board
char board[9] = { ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ' };
int board_draw_loc[9][2] = { {258, 162}, {367, 162}, {481, 162},
							   {258, 278}, {367, 276}, {481, 276},
							   {258, 386}, {367, 388}, {481, 383} };

// Turn counter
int turn = 0;

// Turn text
char turn_text_x[] = "It is X's turn";
char turn_text_y[] = "It is O's turn";

// Mouse for debug
int mx = 0;
int my = 0;

// Current cell
int cx = 0;

// Gets turn text
const char* get_turn_text(int turn)
{
	if (turn % 2 == 0) {
		return "It is X's turn";
	}
	else {
		return "It is O's turn";
	}
}

// Gets turn character (X or O)
const char* get_turn_char(int turn)
{
	if (turn % 2 == 0) {
		return "X";
	}
	else {
		return "O";
	}
}

//const char* get_board_char(int i)
//{
//	return &board[i];
//}

const char* get_mouse_state_text()
{
	char mst[10];
	sprintf(mst, "%d, %d\n", mx, my);
	printf(mst, "%d, %d\n", mx, my);
	return mst;
}

bool check_for_win()
{
	// First row X
	if (board[0] == 'X' && board[1] == 'X' && board[2] == 'X')
	{
		return true;
	}
	// First col X
	if (board[0] == 'X' && board[3] == 'X' && board[6] == 'X')
	{
		return true;
	}
	// First row O
	if (board[0] == 'O' && board[1] == 'O' && board[2] == 'O')
	{
		return true;
	}
	// First col O
	if (board[0] == 'O' && board[3] == 'O' && board[6] == 'O')
	{
		return true;
	}
	// Second row X
	if (board[3] == 'X' && board[4] == 'X' && board[5] == 'X')
	{
		return true;
	}
	// Second col X
	if (board[1] == 'X' && board[4] == 'X' && board[7] == 'X')
	{
		return true;
	}
	// Second row O
	if (board[3] == 'O' && board[4] == 'O' && board[5] == 'O')
	{
		return true;
	}
	// Second col O
	if (board[1] == 'O' && board[4] == 'O' && board[7] == 'O')
	{
		return true;
	}
	// Third row X
	if (board[6] == 'X' && board[7] == 'X' && board[8] == 'X')
	{
		return true;
	}
	// Third col X
	if (board[2] == 'X' && board[5] == 'X' && board[8] == 'X')
	{
		return true;
	}
	// Third row O
	if (board[6] == 'O' && board[7] == 'O' && board[8] == 'O')
	{
		return true;
	}
	// Third col O
	if (board[2] == 'O' && board[5] == 'O' && board[8] == 'O')
	{
		return true;
	}
	// Diag lttb X
	if (board[0] == 'X' && board[4] == 'X' && board[8] == 'X')
	{
		return true;
	}
	// Diag lttb O
	if (board[0] == 'O' && board[4] == 'O' && board[8] == 'O')
	{
		return true;
	}
	// Diag rttb X
	if (board[2] == 'X' && board[4] == 'X' && board[6] == 'X')
	{
		return true;
	}
	// Diag rttb O
	if (board[2] == 'O' && board[4] == 'O' && board[6] == 'O')
	{
		return true;
	}
	return false;
}


int main(void) {
	printf("Init\n");
	int SCREEN_WIDTH = 800;
	int SCREEN_HEIGHT = 600;

	al_init();
	printf("Allegro init\n");
	al_install_mouse();
	al_install_keyboard();
	al_init_font_addon();
	al_init_ttf_addon();
	al_init_image_addon();
	al_init_primitives_addon();
	printf("Allegro addons init finished\n");

	// Antialiasing
	al_set_new_display_option(ALLEGRO_SAMPLE_BUFFERS, 1, ALLEGRO_SUGGEST);
	al_set_new_display_option(ALLEGRO_SAMPLES, 8, ALLEGRO_SUGGEST);
	al_set_new_bitmap_flags(ALLEGRO_MIN_LINEAR | ALLEGRO_MAG_LINEAR);

	ALLEGRO_TIMER* timer = al_create_timer(1.0 / 30.0);
	ALLEGRO_EVENT_QUEUE* queue = al_create_event_queue();
	ALLEGRO_DISPLAY *display = al_create_display(SCREEN_WIDTH, SCREEN_HEIGHT);
	ALLEGRO_FONT *font = al_load_ttf_font("VCR_OSD_MONO.ttf", 24, 0);
	ALLEGRO_FONT* char_font = al_load_ttf_font("VCR_OSD_MONO.ttf", 50, 0);
	ALLEGRO_BITMAP *grid = al_load_bitmap("grid.bmp");

	printf("Allegro timer, event queue, display, font and bitmap loaded\n");

	al_register_event_source(queue, al_get_keyboard_event_source());
	al_register_event_source(queue, al_get_mouse_event_source());
	al_register_event_source(queue, al_get_display_event_source(display));
	al_register_event_source(queue, al_get_timer_event_source(timer));

	printf("Allegro event sources configured\n");

	bool redraw = true;
	bool done = false;
	bool win = false;

	ALLEGRO_EVENT event;
	ALLEGRO_MOUSE_STATE mouse_state;

	al_start_timer(timer);

	printf("Allegro timer started\n");

	while (true) {
		al_wait_for_event(queue, &event);

		switch (event.type)
		{
			case ALLEGRO_EVENT_TIMER:
				//redraw = true;
				if (check_for_win())
				{
					win = true;
					redraw = true;
				}
				// Game logic goes here.
				break;



			case ALLEGRO_EVENT_KEY_DOWN:
				if (event.keyboard.keycode == ALLEGRO_KEY_SPACE)
				{
					board[cx] = *get_turn_char(turn);
					cx = 0;
					turn++;
					redraw = true;
					break;
				}
				if (event.keyboard.keycode == ALLEGRO_KEY_RIGHT) {
					if (cx <= 8) {
						cx++;
					}
					else {
						cx = 0;
					}
					printf("%d\n", cx);
					redraw = true;
					break;
				}
				if (event.keyboard.keycode == ALLEGRO_KEY_R)
				{
					turn = 0;
					cx = 0;
					for (int i = 0; i < 9; i++)
					{
						board[i] = ' ';
					}
					win = false;
					redraw = true;
					break;
				}
				if (event.keyboard.keycode == ALLEGRO_KEY_ESCAPE)
				{
					done = true;
					break;
				}
				if (event.keyboard.keycode != ALLEGRO_KEY_ESCAPE)
				{
					break;
				}
			case ALLEGRO_EVENT_MOUSE_AXES:
				al_get_mouse_state(&mouse_state);
				mx = mouse_state.x;
				my = mouse_state.y;
				break;

			case ALLEGRO_EVENT_DISPLAY_CLOSE:
				done = true;
				break;

		}

		if (done)
			break;

		if (redraw && al_is_event_queue_empty(queue))
		{
			// Clear to grey
			al_clear_to_color(al_map_rgb(50, 50, 50));
			
			// Draw board
			al_draw_bitmap(grid, 0, 0, 0);
			
			al_draw_text(font, al_map_rgb(255, 255, 255), 0, 0, 0, "Tic-Tac-Toe");
			al_draw_text(font, al_map_rgb(255, 255, 255), 0, 50, 0, get_turn_text(turn));

			if (win)
			{
				al_draw_text(char_font, al_map_rgb(0, 255, 0), 0, 500, 0, "WIN! Press 'R' to restart.");
			}

			for (int counter = 0; counter < 9; counter++) {
				al_draw_glyph(char_font, al_map_rgb(255, 0, 0), board_draw_loc[counter][0], board_draw_loc[counter][1], board[counter]);
			}
			
			al_draw_text(char_font, al_map_rgb(0, 0, 255), board_draw_loc[cx][0], board_draw_loc[cx][1], 0, get_turn_char(turn));

			al_flip_display();
			redraw = false;
		}

		
	}

	al_destroy_display(display);
	al_destroy_font(font);
	al_destroy_timer(timer);
	al_destroy_event_queue(queue);

	return 0;
}
