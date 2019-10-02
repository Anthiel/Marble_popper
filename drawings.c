#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <gtk/gtk.h>
#include <math.h>
#include "util.h"
#include "curve.h"
#include "game.h"
#include "mydata.h"
#include "font.h"
#include "drawings.h"
#include "menus.h"


void init_canon(Game *M_game, Mydata *Mem_1){

	M_game->level_list.levels[M_game->current_level].canon_x = 600;
	M_game->level_list.levels[M_game->current_level].canon_y = 350;
}


void intro_screen(cairo_t *cr, Mydata *Mem_1){

			Game *Mem_game = &Mem_1->M_game;

			//logo
			int menu_logo_w = cairo_image_surface_get_width (Mem_game->menu_logo),
				menu_logo_h = cairo_image_surface_get_height (Mem_game->menu_logo);

			double cx = Mem_1->area_width/2, cy = 120;

			cairo_set_source_surface (cr, Mem_game->menu_logo, cx-(menu_logo_w/2.0), cy-(menu_logo_h/2.0));
			cairo_rectangle (cr, cx-(menu_logo_w/2.0), cy-(menu_logo_h/2.0), menu_logo_w, menu_logo_h);
			cairo_fill (cr);

			//bouton play
			int menu_play_w = cairo_image_surface_get_width (Mem_game->menu_play),
				menu_play_h = cairo_image_surface_get_height (Mem_game->menu_play);

			cx = Mem_1->area_width/2, cy = 250;

			cairo_set_source_surface (cr, Mem_game->menu_play, cx-(menu_play_w/2.0), cy-(menu_play_h/2.0));
			cairo_rectangle (cr, cx-(menu_play_w/2.0), cy-(menu_play_h/2.0), menu_play_w, menu_play_h);
			cairo_fill (cr);

			//bouton open
			int menu_open_w = cairo_image_surface_get_width (Mem_game->menu_open),
				menu_open_h = cairo_image_surface_get_height (Mem_game->menu_open);

			cx = Mem_1->area_width/2, cy = 400;

			cairo_set_source_surface (cr, Mem_game->menu_open, cx-(menu_open_w/2.0), cy-(menu_open_h/2.0));
			cairo_rectangle (cr, cx-(menu_open_w/2.0), cy-(menu_open_h/2.0), menu_open_w, menu_open_h);
			cairo_fill (cr);


			//bouton new
			int menu_new_w = cairo_image_surface_get_width (Mem_game->menu_new),
				menu_new_h = cairo_image_surface_get_height (Mem_game->menu_new);

			cx = Mem_1->area_width/2, cy = 550;

			cairo_set_source_surface (cr, Mem_game->menu_new, cx-(menu_new_w/2.0), cy-(menu_new_h/2.0));
			cairo_rectangle (cr, cx-(menu_new_w/2.0), cy-(menu_new_h/2.0), menu_new_w, menu_new_h);
			cairo_fill (cr);


			//bouton pouki
			int menu_pouki_w = cairo_image_surface_get_width (Mem_game->menu_pouki),
				menu_pouki_h = cairo_image_surface_get_height (Mem_game->menu_pouki);

			cx = Mem_1->area_width-150, cy = 620;

			cairo_set_source_surface (cr, Mem_game->menu_pouki, cx-(menu_pouki_w/2.0), cy-(menu_pouki_h/2.0));
			cairo_rectangle (cr, cx-(menu_pouki_w/2.0), cy-(menu_pouki_h/2.0), menu_pouki_w, menu_pouki_h);
			cairo_fill (cr);
}

void update_area1_with_transforms(Mydata *Mem_1){
	
	apply_image_transforms(Mem_1);	
	
	if (Mem_1->pixbuf2 == NULL)
	{
		printf("ERROR : pixbuf2 null dans la fonction update_area1_with_transforms\n");
		return;
	}
	int largeur_pix = gdk_pixbuf_get_width(Mem_1->pixbuf2);
	int hauteur_pix = gdk_pixbuf_get_height(Mem_1->pixbuf2);

	gtk_widget_set_size_request (Mem_1->area1, largeur_pix, hauteur_pix);
	refresh_area(Mem_1->area1);
}

void apply_image_transforms(Mydata *Mem_1) // rotate pour pixbuf2 et area avec un angle donné
{
	g_clear_object(&Mem_1->pixbuf2);

	if (Mem_1->pixbuf1 == NULL){
		printf("ERROR : pixbuf1 null dans la fonction apply_image_transforms\n");
		return;
	}

	GdkPixbuf *tmp;
	tmp = gdk_pixbuf_rotate_simple(Mem_1->pixbuf1, Mem_1->rotate_angle);
		
	int largeur_pix = gdk_pixbuf_get_width(tmp);
	int hauteur_pix = gdk_pixbuf_get_height(tmp);
	
	Mem_1->pixbuf2 = gdk_pixbuf_scale_simple(tmp, largeur_pix*(Mem_1->scale1_value), hauteur_pix*(Mem_1->scale1_value), GDK_INTERP_BILINEAR); // zoomer l'image rotate

	g_object_unref(tmp);
	gtk_widget_set_size_request (Mem_1->area1, largeur_pix, hauteur_pix);
	refresh_area(Mem_1->area1);	
	set_status(Mem_1->status,"Image rotated");
}
void draw_lose(cairo_t *cr, Mydata *Mem_1){

	PangoLayout *layout = pango_cairo_create_layout (cr);
	font_set_name (layout, "Sans, 30");
	cairo_set_source_rgb (cr, 1.0,0.2,0.0);	
	font_draw_text (cr, layout, FONT_TL, Mem_1->area_width/2-100, 500, "score : %d", Mem_1->M_game.score);

	PangoLayout *layout1 = pango_cairo_create_layout (cr);
	font_set_name (layout1, "Sans, 30");
	cairo_set_source_rgb (cr, 1.0,0.6,0.0);	
	font_draw_text (cr, layout1, FONT_TL, Mem_1->area_width/2-100, 50, "PERDU :(");


	Game *M_game = &Mem_1->M_game;
	double cx = Mem_1->area_width/2, cy = 300;
	int menu_w = cairo_image_surface_get_width (M_game->menu_menu),
		menu_h = cairo_image_surface_get_height (M_game->menu_menu);

	cairo_set_source_surface (cr, M_game->menu_menu, cx-(menu_w/2.0), cy-(menu_h/2.0));
	cairo_rectangle (cr, cx-(menu_w/2.0), cy-(menu_h/2.0), menu_w, menu_h);
	cairo_fill (cr);
}

void draw_won(cairo_t *cr, Mydata *Mem_1){
		
	PangoLayout *layout = pango_cairo_create_layout (cr);
	font_set_name (layout, "Sans, 30");
	cairo_set_source_rgb (cr, 1.0,0.2,0.0);	
	font_draw_text (cr, layout, FONT_TL, Mem_1->area_width/2-100, 150, "score : %d", Mem_1->M_game.score);

	PangoLayout *layout1 = pango_cairo_create_layout (cr);
	font_set_name (layout1, "Sans, 30");
	cairo_set_source_rgb (cr, 1.0,0.6,0.0);	
	font_draw_text (cr, layout1, FONT_TL, Mem_1->area_width/2-100, 50, "GAGNE ! :)");


	Game *M_game = &Mem_1->M_game;
	double cx = Mem_1->area_width/2, cy = 300;
	int menu_w = cairo_image_surface_get_width (M_game->menu_menu),
		menu_h = cairo_image_surface_get_height (M_game->menu_menu);

	cairo_set_source_surface (cr, M_game->menu_menu, cx-(menu_w/2.0), cy-(menu_h/2.0));
	cairo_rectangle (cr, cx-(menu_w/2.0), cy-(menu_h/2.0), menu_w, menu_h);
	cairo_fill (cr);

	if (M_game->current_level < M_game->level_list.level_count-1)
	{
		cx = Mem_1->area_width/2, cy = 450;
		int menu_next_w = cairo_image_surface_get_width (M_game->menu_next),
			menu_next_h = cairo_image_surface_get_height (M_game->menu_menu);

		cairo_set_source_surface (cr, M_game->menu_next, cx-(menu_next_w/2.0), cy-(menu_next_h/2.0));
		cairo_rectangle (cr, cx-(menu_next_w/2.0), cy-(menu_next_h/2.0), menu_next_w, menu_next_h);
		cairo_fill (cr);
	}
}


void draw_control_polygons (cairo_t *cr, Curve_infos *ci)
{
	int i,j;
	
	int n = ci->curve_list.curve_count; // nombre de courbe
	int k; // nombre de controle dans la courbe
	
	for (i = 0; i < n; i++){		
		k = ci->curve_list.curves[i].control_count;

		for (j = 0; j < k; j++){				
				cairo_set_line_width (cr, 1.5);

				if (j==ci->current_control && i == ci->current_curve)
					cairo_set_source_rgb (cr, 1.0, 0, 0);
				else
					cairo_set_source_rgb (cr, 0, 0, 1.0);

				cairo_rectangle (cr, ci->curve_list.curves[i].controls[j].x-3,ci->curve_list.curves[i].controls[j].y-3,6.0, 6.0);
				cairo_stroke(cr);
		}

		cairo_move_to(cr, ci->curve_list.curves[i].controls[0].x,ci->curve_list.curves[i].controls[0].y);

		for(j=0; j<k; j++){
			if (i == ci->current_curve)
				cairo_set_source_rgb (cr, 1.0, 1.0, 0.1);
			else
				cairo_set_source_rgb (cr, 0.6, 0.6, 0.6);
			
			cairo_line_to(cr, ci->curve_list.curves[i].controls[j].x,ci->curve_list.curves[i].controls[j].y);

		}
			cairo_stroke(cr);
	}
}


gboolean on_area1_draw(GtkWidget *area,cairo_t *cr, gpointer data){

	Mydata *Mem_1 = get_mydata(data);

	Curve_infos *ci= &Mem_1->curve_infos;
	PangoLayout *layout = pango_cairo_create_layout (cr);
	
	Mem_1->area1 = area;

	draw_background(cr,&Mem_1->M_game);

	if(Mem_1->M_game.state == GS_HELLO)
	{
		if (Mem_1->tmp_areadraw_init == 0){

			init_game_infos (&Mem_1->M_game,Mem_1, Mem_1->area1);
			*ci = Mem_1->M_game.level_list.levels[0].curve_infos;
			Mem_1->tmp_areadraw_init = 1;
		}
		intro_screen(cr, Mem_1);
	}
	if (Mem_1->M_game.state != GS_HELLO){
		
		PangoLayout *layout1 = pango_cairo_create_layout (cr);
		font_set_name (layout1, "Sans, 10");
		cairo_set_source_rgb (cr, 1.0,1.0,1.0);	
		font_draw_text (cr, layout1, FONT_TL, 10, 10, "Level actuel = %d",Mem_1->M_game.current_level);
	}

	if(Mem_1->M_game.state != GS_HELLO && Mem_1->show_edit == TRUE){
		draw_control_labels(cr, layout, ci);
		draw_control_polygons(cr,ci);
		draw_bezier_polygons_open (cr,ci);
		g_object_unref (layout);

		draw_bezier_curves_prolong(cr,ci,0.02);
		draw_canon(cr,ci, &Mem_1->M_game);			
	}
	else if(Mem_1->M_game.state == GS_PLAYING && Mem_1->show_edit == FALSE){
									
		affichage_track(cr, &Mem_1->M_game.track_list);
									
		draw_shoot(cr,ci, &Mem_1->M_game);
		draw_trounoir(cr, &Mem_1->M_game);
		afficher_bille(cr,ci,&Mem_1->M_game);
		draw_canon(cr,ci, &Mem_1->M_game); 
		draw_effet_destruc_marble(cr,ci, &Mem_1->M_game);
		afficher_score(cr, &Mem_1->M_game);	
	}
	else if(Mem_1->M_game.state == GS_LOST && Mem_1->show_edit == FALSE)
		draw_lose(cr,Mem_1);
	else if(Mem_1->M_game.state == GS_WON && Mem_1->show_edit == FALSE)
		draw_won(cr,Mem_1);

	refresh_area(Mem_1->area1);
	return TRUE;
}



gboolean on_area1_motion_notify (GtkWidget *widget,GdkEvent *event, gpointer data){
	Mydata *Mem_1 = get_mydata(data);
	
	GdkEventMotion *evm = &event->motion;
	Curve_infos *ci= &Mem_1->curve_infos;
	
	int current_level = Mem_1->M_game.current_level;
	Level_list *ll = &Mem_1->M_game.level_list;
		
	if (Mem_1->click_n == 1)
	{		
		Mem_1->last_x = Mem_1->click_x;
		Mem_1->last_y = Mem_1->click_y;

		Mem_1->click_x = evm->x;
		Mem_1->click_y = evm->y;		
	}

	if (Mem_1->click_n == 1 && Mem_1->show_edit == TRUE){
		switch (Mem_1->edit_mode) {
			case EDIT_ADD_CURVE :break;
			case EDIT_MOVE_CURVE : move_curve(ci, Mem_1->click_x- Mem_1->last_x, Mem_1->click_y-Mem_1->last_y); break;
			case EDIT_REMOVE_CURVE : break;
			case EDIT_ADD_CONTROL : break;
			case EDIT_MOVE_CONTROL : move_control(ci, Mem_1->click_x- Mem_1->last_x, Mem_1->click_y-Mem_1->last_y); break;
			case EDIT_REMOVE_CONTROL : break;
			case EDIT_MOVE_CANON : move_canon(&Mem_1->M_game, Mem_1->click_x, Mem_1->click_y);break; 
		}
		refresh_area(Mem_1->area1);

	}

	if (Mem_1->M_game.state == GS_PLAYING){ // faut pas clicker pour le canon
	
		Mem_1->click_x = evm->x;
		Mem_1->click_y = evm->y; 
		update_angle_canon(&Mem_1->M_game, Mem_1->click_x, Mem_1->click_y);
		refresh_area(Mem_1->area1);
	}
	
	if (Mem_1->click_n!=0) //mouvement avec bouton enfoncés
		refresh_area(Mem_1->area1);

	ll->levels[current_level].curve_infos = *ci;

	return TRUE;
}
gboolean on_area1_enter_notify (GtkWidget *widget,GdkEvent *event, gpointer data){
	Mydata *Mem_1 = get_mydata(data);
	GdkEventCrossing *evc = &event->crossing;
	printf ("%s: %.1f %.1f\n", __func__, evc->x, evc->y);
	// On avait peut-^etre perdu le focus
	gtk_widget_grab_focus (Mem_1->area1);
	return TRUE;
}
gboolean on_area1_leave_notify (GtkWidget *widget,GdkEvent *event, gpointer data){
	return TRUE;
}



gboolean on_area1_key_press (GtkWidget *widget,GdkEvent *event, gpointer data){
	Mydata *Mem_1 = get_mydata(data);
	GdkEventKey *evk = &event->key;

	if(Mem_1->M_game.state == GS_HELLO)
		Mem_1->M_game.state = GS_PLAYING;

	printf ("%s: GDK_KEY_%s\n",__func__, gdk_keyval_name(evk->keyval));

	switch (evk->keyval) {
		case GDK_KEY_q : printf("programme se ferme\n"); exit(0); break;
		case GDK_KEY_a : set_edit_mode (Mem_1, EDIT_ADD_CURVE);break;
		case GDK_KEY_b : set_edit_mode (Mem_1, EDIT_MOVE_CURVE);break;
		case GDK_KEY_c : set_edit_mode (Mem_1, EDIT_REMOVE_CURVE);break;
		case GDK_KEY_d : set_edit_mode (Mem_1, EDIT_ADD_CONTROL);break;
		case GDK_KEY_e : set_edit_mode (Mem_1, EDIT_MOVE_CONTROL);break;
		case GDK_KEY_f : set_edit_mode (Mem_1, EDIT_REMOVE_CONTROL);break;
		case GDK_KEY_g : set_edit_mode (Mem_1, 7);break;
		case GDK_KEY_space : inverse_balls(&Mem_1->M_game); break;
		case GDK_KEY_p : if(Mem_1->stop_billes == 0) Mem_1->stop_billes=1; else Mem_1->stop_billes = 0;
	
	}
	return TRUE;
}

gboolean on_area1_key_release (GtkWidget *widget,GdkEvent *event, gpointer data){
	return TRUE;
}

gboolean on_area1_button_press (GtkWidget *widget,GdkEvent *event, gpointer data){
	Mydata *Mem_1 = get_mydata(data);
	Curve_infos *ci= &Mem_1->curve_infos;
	
	GdkEventButton *evb = &event->button;
	
	int res;

	Mem_1->click_x = evb->x;
	Mem_1->click_y = evb->y;
	Mem_1->click_n = evb->button;

	printf ("%s: %d %.1f %.1f\n",__func__, Mem_1->click_n, Mem_1->click_x, Mem_1->click_y);
	printf("edit_mode : %d\n", Mem_1->edit_mode);

	if(Mem_1->click_n == 1 && Mem_1->M_game.state == GS_HELLO && Mem_1->show_edit == FALSE){
	
		suppr_all_shoot(&Mem_1->M_game);
		if (Mem_1->click_x >= 459 && Mem_1->click_x<=819 &&  Mem_1->click_y >=205 && Mem_1->click_y<=288){
			//PLAY
			
			init_train_bille(&Mem_1->M_game.track_list, &Mem_1->M_game); // A METTRE DANS UNE CALLBACK SIGNAL RADIO GAME SINON MARCHE PAS
			Mem_1->M_game.state = GS_PLAYING;
			Mem_1->tmp_game = 1;

			open_the_file("levels/base.txt", data);
			translate_curve_to_track(Mem_1);
						
		}
		else if (Mem_1->click_x >= 459 && Mem_1->click_x<=819 &&  Mem_1->click_y >=351 && Mem_1->click_y<=440){
			//OPEN
			on_item_open_activate(widget, data);
		}
		else if (Mem_1->click_x >= 459 && Mem_1->click_x<=819 &&  Mem_1->click_y >=496 && Mem_1->click_y<=588){
			//NEW
			gtk_check_menu_item_set_active (GTK_CHECK_MENU_ITEM(Mem_1->item_editing), TRUE);
		
			init_train_bille(&Mem_1->M_game.track_list, &Mem_1->M_game); 	

			open_the_file("levels/new.txt", data);
			translate_curve_to_track(Mem_1);
			on_item_editing_activate(widget, data);
			init_canon(&Mem_1->M_game, Mem_1);
			Mem_1->M_game.state = GS_PLAYING;
			return TRUE;
		}
	}

	if (Mem_1->click_n == 1 && Mem_1->show_edit == FALSE && Mem_1->M_game.state == GS_PLAYING){
		if(Mem_1->M_game.allow_shoot == 1 && Mem_1->M_game.state != GS_LOST && evb->type == GDK_BUTTON_PRESS)
		 	add_shoot(ci, &Mem_1->M_game);
	}

	if(Mem_1->click_n == 1 && Mem_1->M_game.state == GS_LOST && Mem_1->show_edit == FALSE){
		if (Mem_1->click_x >= 452 && Mem_1->click_x<=822 &&  Mem_1->click_y >=247 && Mem_1->click_y<=338){
			Mem_1->M_game.state = GS_HELLO;
			Mem_1->show_edit = FALSE;
			gtk_check_menu_item_set_active (GTK_CHECK_MENU_ITEM(Mem_1->item_editing), FALSE);
		}
	}

	if(Mem_1->click_n == 1 && Mem_1->M_game.state == GS_WON && Mem_1->show_edit == FALSE){
		if (Mem_1->click_x >= 452 && Mem_1->click_x<=822 &&  Mem_1->click_y >=247 && Mem_1->click_y<=338){
			Mem_1->M_game.state = GS_HELLO;
			Mem_1->show_edit = FALSE;
			gtk_check_menu_item_set_active (GTK_CHECK_MENU_ITEM(Mem_1->item_editing), FALSE);
		}
		else if (Mem_1->click_x >= 452 && Mem_1->click_x<=830 &&  Mem_1->click_y >=404 && Mem_1->click_y<=487){
			on_item_nextlevel_activate(widget, data);
		}
	}



	if (Mem_1->click_n == 1 && Mem_1->show_edit == TRUE && Mem_1->M_game.state == GS_PLAYING){
		switch (Mem_1->edit_mode) {
			case EDIT_ADD_CURVE : 						
									res = add_curve(ci);

									if(res<0)
										break;
						
									set_edit_mode(Mem_1, EDIT_ADD_CONTROL); 
									add_control(ci, Mem_1->click_x, Mem_1->click_y);	
									break;

			case EDIT_MOVE_CURVE : find_control (ci, Mem_1->click_x, Mem_1->click_y); break;
			case EDIT_REMOVE_CURVE : if(find_control(ci, Mem_1->click_x, Mem_1->click_y)>=0){remove_curve(ci);}break;
			case EDIT_ADD_CONTROL : add_control(ci, Mem_1->click_x, Mem_1->click_y);break;
			case EDIT_MOVE_CONTROL : find_control (ci, Mem_1->click_x, Mem_1->click_y); break;
			case EDIT_REMOVE_CONTROL : if(find_control(ci, Mem_1->click_x, Mem_1->click_y)>=0){remove_control(ci);}break;
		}

	int current_level = Mem_1->M_game.current_level;
	Level_list *ll = &Mem_1->M_game.level_list;
	ll->levels[current_level].curve_infos = *ci;
	}

	refresh_area(Mem_1->area1);
	return TRUE;
}

gboolean on_area1_button_release (GtkWidget *widget,GdkEvent *event, gpointer data){
	Mydata *Mem_1 = get_mydata(data);

	Mem_1->click_n = 0;
	refresh_area(Mem_1->area1);
	return TRUE;	
}

gboolean on_marbl_total_change_notify(GtkWidget *widget,GdkEvent *event, gpointer data){
	Mydata *Mem_1 = get_mydata(data);
	int current_level = Mem_1->M_game.current_level;
	Level_list *ll = &Mem_1->M_game.level_list;
	
	ll->levels[current_level].marbles_total = grab_int_value (GTK_SPIN_BUTTON(Mem_1->b_marble_total), data);
	return TRUE;
}

gboolean on_marbl_intro_change_notify (GtkWidget *widget,GdkEvent *event, gpointer data){
	Mydata *Mem_1 = get_mydata(data);
	int current_level = Mem_1->M_game.current_level;
	Level_list *ll = &Mem_1->M_game.level_list;
	
	ll->levels[current_level].marbles_intro = grab_int_value (GTK_SPIN_BUTTON(Mem_1->b_marble_intro), data);
	return TRUE;
}

void area1_init(Mydata *Mem_1){ // créer l'image avec le scroll

	Mem_1->area1 = gtk_drawing_area_new (); //création du widget de zone de dessin
	
	g_signal_connect (Mem_1->area1, "draw",G_CALLBACK(on_area1_draw), Mem_1);
	g_signal_connect (Mem_1->area1, "key-press-event",G_CALLBACK(on_area1_key_press), Mem_1);
	g_signal_connect (Mem_1->area1, "key-release-event",G_CALLBACK(on_area1_key_release), Mem_1);
	g_signal_connect (Mem_1->area1, "button-press-event",G_CALLBACK(on_area1_button_press), Mem_1);
	g_signal_connect (Mem_1->area1, "button-release-event",G_CALLBACK(on_area1_button_release), Mem_1);
	g_signal_connect (Mem_1->area1, "motion-notify-event",G_CALLBACK(on_area1_motion_notify), Mem_1);
	g_signal_connect (Mem_1->area1, "enter-notify-event",G_CALLBACK(on_area1_enter_notify), Mem_1);
	g_signal_connect (Mem_1->area1, "leave-notify-event",G_CALLBACK(on_area1_leave_notify), Mem_1);

	gtk_widget_add_events (Mem_1->area1,GDK_KEY_PRESS_MASK | GDK_KEY_RELEASE_MASK |GDK_FOCUS_CHANGE_MASK |GDK_BUTTON_PRESS_MASK | GDK_BUTTON_RELEASE_MASK |
						GDK_POINTER_MOTION_MASK | GDK_ENTER_NOTIFY_MASK | GDK_LEAVE_NOTIFY_MASK );

	gtk_widget_set_can_focus (Mem_1->area1, TRUE);
}


void draw_control_labels(cairo_t *cr, PangoLayout *layout, Curve_infos *ci){ // text au dessus du point de control (1,2 ...)

	int cc = ci->current_control;
	int c_curve = ci->current_curve;
	int x = ci->curve_list.curves[c_curve].controls[cc].x;
	int y = ci->curve_list.curves[c_curve].controls[cc].y;

	font_set_name (layout, "Sans, 8");
	cairo_set_source_rgb (cr, 0.37,0.37,0.37);

	int nb_courbe = ci->curve_list.curve_count;
	int point;
	int i,j;

	for(i=0; i<nb_courbe; i++){
		point = ci->curve_list.curves[i].control_count;

		for(j=0; j<point;j++){
			x = ci->curve_list.curves[i].controls[j].x;
			y = ci->curve_list.curves[i].controls[j].y;
			font_draw_text (cr, layout, FONT_TL, x-2, y-18, "%d", j);
		}
	}
}

void draw_bezier_polygons_open (cairo_t *cr,Curve_infos *ci){
	int nb_courbe = ci->curve_list.curve_count;
	int i,j;
	
	for(i=0; i<nb_courbe;i++){
		Curve *curve = &ci->curve_list.curves[i];		
		for(j=0;j<curve->control_count-3;j++){

			Control bez_points[4];			
			compute_bezier_points_open(curve, j, bez_points);
			cairo_set_source_rgb (cr, 0, 1.0, 0);
			cairo_move_to(cr, bez_points[0].x,bez_points[0].y);
			cairo_line_to(cr, bez_points[1].x,bez_points[1].y);
			cairo_move_to(cr, bez_points[2].x,bez_points[2].y);
			cairo_line_to(cr, bez_points[3].x,bez_points[3].y);

			cairo_stroke(cr);	
		}
	}
}

void draw_bezier_curve (cairo_t *cr, Control bez_points[4], double theta){
	int i;
	double bx[4], by[4];

	for (i=0; i<4;i++){
		bx[i] = bez_points[i].x;
		by[i] = bez_points[i].y;
	}
	double x,y;
	x = compute_bezier_cubic(bx, 0);
	y = compute_bezier_cubic(by, 0);

	cairo_set_source_rgb(cr,1.0,0.0,1.0);
	cairo_set_line_width(cr,2.0);
	cairo_move_to(cr,x,y);
	double t;

	for (t=theta; t<1.0; t+=theta){
		x = compute_bezier_cubic(bx, t);
		y = compute_bezier_cubic(by, t);
		cairo_line_to(cr,x,y);
	}

	x = compute_bezier_cubic(bx, 1.0);
	y = compute_bezier_cubic(by, 1.0);

	cairo_line_to(cr,x,y);

	cairo_stroke(cr);
}


void draw_bezier_curves_open (cairo_t *cr, Curve_infos *ci, double theta){
	int nb_courbe = ci->curve_list.curve_count;
	int i,j;	

	for(i=0; i<nb_courbe;i++){
		Curve *curve = &ci->curve_list.curves[i];		
		for(j=0;j<curve->control_count-3;j++){
			Control bez_points[4];			
			compute_bezier_points_open(curve, j, bez_points);
			draw_bezier_curve(cr, bez_points, theta);	
		}
	}
}


void draw_bezier_curves_close (cairo_t *cr, Curve_infos *ci, double theta){
	int nb_courbe = ci->curve_list.curve_count;
	int i,j;
	
	for(i=0; i<nb_courbe;i++)	{
		Curve *curve = &ci->curve_list.curves[i];		
		for(j=0;j<curve->control_count;j++){
			Control bez_points[4];			
			compute_bezier_points_close(curve, j, bez_points);
			draw_bezier_curve(cr, bez_points, theta);	
		}
	}
}

void draw_bezier_curves_prolong (cairo_t *cr, Curve_infos *ci, double theta){
	int nb_courbe = ci->curve_list.curve_count;
	int i,j;

	for(i=0; i<nb_courbe;i++){
		Curve *curve = &ci->curve_list.curves[i];
		Control bez_points[4];

		if (curve->control_count>3){
					compute_bezier_points_prolong_first (curve, bez_points);
					draw_bezier_curve(cr, bez_points, theta);
					compute_bezier_points_prolong_last (curve, bez_points);
					draw_bezier_curve(cr, bez_points, theta);

			for(j=0;j<curve->control_count-3;j++){

				Control bez_points[4];				
				compute_bezier_points_open(curve, j, bez_points);
				draw_bezier_curve(cr, bez_points, theta);		
			}
		}		
	}
}


void generate_bezier_path (cairo_t *cr, Control bez_points[4], double theta, int is_first){
	int i;
	double bx[4], by[4];
	for (i=0; i<4;i++){
		bx[i] = bez_points[i].x;
		by[i] = bez_points[i].y;
	}
	double x,y;
	x = compute_bezier_cubic(bx, 0);
	y = compute_bezier_cubic(by, 0);

	if (is_first==1)
		cairo_move_to(cr,x,y);
	double t;

	for (t=theta; t<1.0; t+=theta){

		x = compute_bezier_cubic(bx, t);
		y = compute_bezier_cubic(by, t);
		cairo_line_to(cr,x,y);
	}

	x = compute_bezier_cubic(bx, 1.0);
	y = compute_bezier_cubic(by, 1.0);

	cairo_line_to(cr,x,y);
}
	
void draw_bezier_curves_fill (cairo_t *cr, Curve_infos *ci, double theta){
	int nb_courbe = ci->curve_list.curve_count;
	int i,j;
	cairo_set_source_rgb(cr,1.0,0.0,1.0);
	cairo_set_line_width(cr,2.0);

	for(i=0; i<nb_courbe;i++){
		Curve *curve = &ci->curve_list.curves[i];

		
		for(j=0;j<curve->control_count;j++){

			Control bez_points[4];			
			compute_bezier_points_close(curve, j, bez_points);		
			generate_bezier_path(cr, bez_points, theta, j==0);		
		}
		cairo_fill(cr);
	}	
}

void draw_bezier_curves_clip (cairo_t *cr, Curve_infos *ci, Mydata *Mem_1){
	int nb_courbe = ci->curve_list.curve_count;
	int i,j;

	cairo_set_line_width(cr,2.0);

	for(i=0; i<nb_courbe;i++){
		Curve *curve = &ci->curve_list.curves[i];

		if (Mem_1->pixbuf2 != NULL)
			gdk_cairo_set_source_pixbuf(cr,Mem_1->pixbuf2,curve->shift_x,curve->shift_y);
		else
			cairo_set_source_rgb(cr,1.0,0.0,1.0);

		
		for(j=0;j<curve->control_count;j++){
			Control bez_points[4];
			
			compute_bezier_points_close(curve, j, bez_points);
		
			generate_bezier_path(cr, bez_points, 0.02, j==0);
		
		}
		cairo_fill(cr);
	}
}


gboolean on_timeout1 (gpointer data){

	Mydata *Mem_1 = get_mydata(data);
	Game *M_game = &Mem_1->M_game;
	int shot_count = M_game->shot_list.shot_count;
	double dx,dy;

	if (M_game->state == GS_HELLO || Mem_1->show_edit == TRUE)
		return TRUE;

	int verif = 0;
	if (M_game->state == GS_PLAYING){		
		for(int k = 0; k<M_game->track_list.track_count ; k++){
			if(M_game->track_list.tracks[k].marble_count == 0)
				verif = 0;
			else{
				verif = 1;
				break;
			}
		}
	}

	if (verif==0 && M_game->state == GS_PLAYING)
		M_game->state = GS_WON;
	
	for(int i= 0; i<shot_count; i++){
		dx = M_game->shot_list.shots[i].dx;
		dy = M_game->shot_list.shots[i].dy;
		
		M_game->shot_list.shots[i].x += dx * SHOT_SPEED;
		M_game->shot_list.shots[i].y += dy * SHOT_SPEED;
	}

	if(Mem_1->stop_billes == 0)
		move_all_bille(M_game);

	suppress_far_shots(data);
	
	suppress_step_explode(M_game);

	check_collision(M_game);
	incre_step_explode(M_game);
	refresh_area (Mem_1->area1);
	return TRUE;
}


gboolean suppress_far_shots (gpointer data){
	Mydata *Mem_1 = get_mydata(data);
	Game *M_game = &Mem_1->M_game;
	
	for(int i= 0; i<M_game->shot_list.shot_count;i++){
		if (M_game->shot_list.shots[i].x<0 || M_game->shot_list.shots[i].x > Mem_1->area_width ||
			M_game->shot_list.shots[i].y<0 || M_game->shot_list.shots[i].y > Mem_1->area_height)
		{
			memmove(M_game->shot_list.shots+i, M_game->shot_list.shots+i+1, sizeof(Shot)*(M_game->shot_list.shot_count-1-i));
			M_game->shot_list.shot_count--;
		}

	}
	return TRUE;
}


