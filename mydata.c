
#include <gtk/gtk.h>
#include "curve.h"
#include "game.h"
#include "mydata.h"






Mydata *get_mydata (gpointer data){
	if (data == NULL) 
	{
		fprintf (stderr, "get_mydata: NULL data\n"); return NULL;
	}
	if (((Mydata *)data)->magic != MYDATA_MAGIC) 
	{
		fprintf (stderr, "get_mydata: bad magic number\n"); 
		return NULL;
	}
	return data;
}


void init_game_infos (Game *M_game, Mydata *Mem_1, GtkWidget *area){
	Mem_1->area_width = gtk_widget_get_allocated_width (area);
	Mem_1->area_height = gtk_widget_get_allocated_height (area);
}

void level1(Mydata *Mem_1){

	Level_list *ll = &Mem_1->M_game.level_list;
	Curve_infos *ci = &ll->levels[0].curve_infos;

	Mem_1->M_game.current_level = 0;

	ll->level_count = 1;
	Level *level = &ll->levels[0];

	add_curve(ci);
	add_control(ci, 50.0,50.0);
	add_control(ci, 75.0,90.0);
	add_control(ci, 350.0, 200.0);

	level->marbles_intro = 5;
	level->marbles_total = 20;
	level->canon_x = Mem_1->area_width/2.0;
	level->canon_y = Mem_1->area_height/2.0;
	printf("level->canon_x = %lf & level->canon_y = %lf\n",level->canon_x, level->canon_y);

	if (ci->curve_list.curve_count ==0)
	{
		printf("on return\n");
		return;
	}
	level->curve_infos = *ci;
	printf("TON ONCLE\n");
}

void init_memory(Mydata *memory)
{
	memory->magic=MYDATA_MAGIC;
	memory->title="HawawSpace";
	memory->win_height=720;
	memory->win_width=1280;
	memory->current_folder=NULL;
	memory->pixbuf1=NULL;
	memory->pixbuf2=NULL;
	memory->rotate_angle=0.0;

	memory->click_x = 0.0;
	memory->click_y = 0.0;
	memory->click_n = 0;

	memory->clip_image = FALSE;
	memory->show_edit = FALSE;

	memory->edit_mode = EDIT_ADD_CURVE;
	memory->bsp_mode = BSP_OPEN;
	memory->game_mode = GAME_CANON;

	init_curve_infos (&memory->curve_infos);
	//init_game_infos(&memory->M_game, memory);

	color(&memory->M_game);
	memory->M_game.rand = g_rand_new();

	init_level(&memory->M_game.level_list);
	memory->M_game.current_level = 0;
	init_tracklist(&memory->M_game.track_list);

	GRand *rand = g_rand_new();
	memory->M_game.canon.ammo1 = g_rand_int_range (rand, 0, NB_COLOR-1);
	rand = g_rand_new();
	memory->M_game.canon.ammo2 = g_rand_int_range (rand, 0, NB_COLOR-1);


	memory->tmp_game = 0;
	memory->stop_billes = 0;
	
	memory->tmp_areadraw_init = 0;
	memory->M_game.allow_shoot = 0;
	memory->M_game.canon.cx = 100;
	memory->M_game.canon.cy = 100;

	memory->M_game.img_canon = cairo_image_surface_create_from_png ("img/canon.png");
	memory->M_game.background = cairo_image_surface_create_from_png ("img/background.png");
	memory->M_game.trounoir = cairo_image_surface_create_from_png ("img/trounoir.png");
	memory->M_game.stargate = cairo_image_surface_create_from_png ("img/stargate.png");

	memory->M_game.menu_logo = cairo_image_surface_create_from_png ("img/logo.png");
	memory->M_game.menu_play = cairo_image_surface_create_from_png ("img/menu_play.png");
	memory->M_game.menu_open = cairo_image_surface_create_from_png ("img/menu_open.png");
	memory->M_game.menu_new = cairo_image_surface_create_from_png ("img/menu_create.png");
	memory->M_game.menu_pouki = cairo_image_surface_create_from_png ("img/menu_pouki.png");
	memory->M_game.menu_menu = cairo_image_surface_create_from_png ("img/menu_menu.png");
	memory->M_game.menu_next = cairo_image_surface_create_from_png ("img/menu_next.png");

	memory->M_game.canonplanete[0] = cairo_image_surface_create_from_png ("img/canon-planete-1.png");
	memory->M_game.canonplanete[1] = cairo_image_surface_create_from_png ("img/canon-planete-2.png");
	memory->M_game.canonplanete[2] = cairo_image_surface_create_from_png ("img/canon-planete-3.png");
	memory->M_game.canonplanete[3] = cairo_image_surface_create_from_png ("img/canon-planete-4.png");
	memory->M_game.canonplanete[4] = cairo_image_surface_create_from_png ("img/canon-planete-5.png");


	memory->M_game.shotplanete[0] = cairo_image_surface_create_from_png ("img/shot-planete-1.png");
	memory->M_game.shotplanete[1] = cairo_image_surface_create_from_png ("img/shot-planete-2.png");
	memory->M_game.shotplanete[2] = cairo_image_surface_create_from_png ("img/shot-planete-3.png");
	memory->M_game.shotplanete[3] = cairo_image_surface_create_from_png ("img/shot-planete-4.png");
	memory->M_game.shotplanete[4] = cairo_image_surface_create_from_png ("img/shot-planete-5.png");

	Level_list *ll = &memory->M_game.level_list;
	ll->level_count = 1;
}


void set_edit_mode (Mydata *Mem_1, int mode)
{
	if (mode <= EDIT_NONE || mode >= EDIT_LAST){
		printf("error mode en dehors de EDIT_NONE ou EDIT_LAST\n");
		return;
	}	
	gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON(Mem_1->edit_radios[mode-1]), TRUE);
}

gint grab_int_value (GtkSpinButton *button, gpointer user_data){
  return gtk_spin_button_get_value_as_int (button);
}


void translate_curve_to_track(Mydata *Mem_1){
	int current_level = Mem_1->M_game.current_level;
	
	Level_list *ll = &Mem_1->M_game.level_list;
	Curve_infos *ci= &ll->levels[current_level].curve_infos;

	init_tracklist(&Mem_1->M_game.track_list);
	for (int i = 0; i < ci->curve_list.curve_count; i++)
	{
		Mem_1->M_game.track_list.track_count = ci->curve_list.curve_count;
		sample_curve_to_track (&ci->curve_list.curves[i], &Mem_1->M_game.track_list.tracks[i], SAMPLE_THETA);
	}
	init_train_bille(&Mem_1->M_game.track_list, &Mem_1->M_game);
	Mem_1->curve_infos = *ci;
}