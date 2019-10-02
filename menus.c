
#include <gtk/gtk.h>

#include "util.h"
#include "curve.h"
#include "game.h"
#include "mydata.h"
#include "gui.h"
#include "drawings.h"
#include "menus.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

void on_item_editing_activate(GtkWidget *widget, gpointer data){
	Mydata *Mem_1 = get_mydata(data);
	if (gtk_check_menu_item_get_active (GTK_CHECK_MENU_ITEM(Mem_1->item_editing))){
		Mem_1->show_edit = 1;
		gtk_widget_show(Mem_1->frame1); // on affiche le menu editing latéral
		set_status(Mem_1->status,"Editing is on");
		gtk_spin_button_set_value (GTK_SPIN_BUTTON(Mem_1->b_marble_intro), Mem_1->M_game.level_list.levels[Mem_1->M_game.current_level].marbles_intro);
		gtk_spin_button_set_value (GTK_SPIN_BUTTON(Mem_1->b_marble_total), Mem_1->M_game.level_list.levels[Mem_1->M_game.current_level].marbles_total);
	}
	else{
		Mem_1->show_edit= 0;
		init_train_bille(&Mem_1->M_game.track_list, &Mem_1->M_game);
   
   		translate_curve_to_track(Mem_1);
		gtk_widget_hide (Mem_1->frame1); //on cache le menu editing latéral
		set_status(Mem_1->status,"Editing is off");
	}
}

void on_item_quit_activate (GtkWidget *widget,gpointer data){
	Mydata *Mem_1 = get_mydata(data);
	printf ("Closing window and app\n");
	gtk_widget_destroy(Mem_1->window);
}

void on_item_rotate_activate (GtkWidget *widget,gpointer data){
	Mydata *Mem_1 = get_mydata(data);	
 	Mem_1->rotate_angle = Mem_1->rotate_angle - 90.0;
 	if (Mem_1->rotate_angle>=-360.0)
 		Mem_1->rotate_angle+=360.0;
 	
 	update_area1_with_transforms(Mem_1);
 	set_status(Mem_1->status,"Image rotated");
}

void init_authors(gpointer data){//initialise les auteurs

	Mydata *Mem_1 = get_mydata(data);
	Mem_1->auteur[0]="Antoine THIEL Antoine.thiel83@gmail.com";
	Mem_1->auteur[1]=NULL;

}

void on_item_about_activate(GtkWidget *widget,gpointer data){
	Mydata *Mem_1 = get_mydata(data);
	init_authors(data); //initialise les auteurs

	gtk_show_about_dialog (NULL,"program-name", Mem_1->title, "logo-icon-name", "face-wink","version", "2.4","website", "http://j.mp/optigra","authors", Mem_1->auteur, NULL);
}

void on_item_start_activate(GtkWidget *widget, gpointer data){
	Mydata *Mem_1 = get_mydata(data);
	if (Mem_1->M_game.state==GS_PLAYING){
		if(Mem_1->stop_billes == 1) Mem_1->stop_billes=0; 
	}
}

void on_item_pause_activate(GtkWidget *widget, gpointer data){
	Mydata *Mem_1 = get_mydata(data);
	if (Mem_1->M_game.state==GS_PLAYING){
		if(Mem_1->stop_billes == 0) Mem_1->stop_billes=1; 
	}
}

void on_item_new_activate(GtkWidget *widget, gpointer data){
	Mydata *Mem_1 = get_mydata(data);
	init_train_bille(&Mem_1->M_game.track_list, &Mem_1->M_game);
    translate_curve_to_track(Mem_1);
    Mem_1->M_game.state = GS_PLAYING;
}

void on_item_nextlevel_activate(GtkWidget *widget, gpointer data){
	Mydata *Mem_1 = get_mydata(data);

	if (Mem_1->M_game.state!=GS_HELLO && Mem_1->M_game.current_level <LEVEL_MAX){
		Mem_1->M_game.current_level ++;

		Level_list *ll = &Mem_1->M_game.level_list;
		if (ll->level_count-1<=Mem_1->M_game.current_level)
			ll->level_count = Mem_1->M_game.current_level + 1;
		
	}

	gtk_spin_button_set_value (GTK_SPIN_BUTTON(Mem_1->b_marble_intro), Mem_1->M_game.level_list.levels[Mem_1->M_game.current_level].marbles_intro);
	gtk_spin_button_set_value (GTK_SPIN_BUTTON(Mem_1->b_marble_total), Mem_1->M_game.level_list.levels[Mem_1->M_game.current_level].marbles_total);

	init_canon(&Mem_1->M_game, Mem_1);
	init_train_bille(&Mem_1->M_game.track_list, &Mem_1->M_game);
    translate_curve_to_track(Mem_1);
    Mem_1->M_game.state = GS_PLAYING;
}

void on_item_previouslevel_activate(GtkWidget *widget, gpointer data){
	Mydata *Mem_1 = get_mydata(data);
	
	if (Mem_1->M_game.state!=GS_HELLO && Mem_1->M_game.current_level > 0){
		Mem_1->M_game.current_level --;
	}
	gtk_spin_button_set_value (GTK_SPIN_BUTTON(Mem_1->b_marble_intro), Mem_1->M_game.level_list.levels[Mem_1->M_game.current_level].marbles_intro);
	gtk_spin_button_set_value (GTK_SPIN_BUTTON(Mem_1->b_marble_total), Mem_1->M_game.level_list.levels[Mem_1->M_game.current_level].marbles_total);

	init_train_bille(&Mem_1->M_game.track_list, &Mem_1->M_game);
    translate_curve_to_track(Mem_1);
    Mem_1->M_game.state = GS_PLAYING;
}

int read_file(FILE *fichier, gpointer data){
	Mydata *Mem_1 = get_mydata(data);
	Level_list *ll = &Mem_1->M_game.level_list;
	int res;

    res = fscanf(fichier ,"%d", &ll->level_count); 
 	int level_count = ll->level_count;
   	

    for (int i = 0; i < level_count; i++){

    	res = fscanf(fichier , "%d", &ll->levels[i].curve_infos.curve_list.curve_count); 
    	int curve_count = ll->levels[i].curve_infos.curve_list.curve_count;
    	
    	for (int k = 0; k < curve_count; k++){

    		res = fscanf(fichier , "%d", &ll->levels[i].curve_infos.curve_list.curves[k].control_count); 
    		int control_count = ll->levels[i].curve_infos.curve_list.curves[k].control_count;
    	
    		for (int w = 0; w < control_count; w++){

    			res = fscanf(fichier , "%lf", &ll->levels[i].curve_infos.curve_list.curves[k].controls[w].x); 
    			res = fscanf(fichier , "%lf", &ll->levels[i].curve_infos.curve_list.curves[k].controls[w].y); 
       		}    		
    	}
    	res = fscanf(fichier , "%lf", &ll->levels[i].canon_x); 
    	res = fscanf(fichier , "%lf", &ll->levels[i].canon_y); 
    
    	res = fscanf(fichier , "%d", &ll->levels[i].marbles_intro); 
    	res = fscanf(fichier , "%d", &ll->levels[i].marbles_total); 
    
    }
   
    fclose(fichier);

    Mem_1->curve_infos = ll->levels[Mem_1->M_game.current_level].curve_infos;
    return res;
}

void open_the_file(char *filename, gpointer data){
	Mydata *Mem_1 = get_mydata(data);
	   
 	FILE* fichier = NULL;   
   
     fichier = fopen(filename, "r");
  
    if (fichier == NULL){
        printf("erreur, le fichier n'existe pas / n'est pas lisible\n");
        return;
    }   
  	Mem_1->M_game.current_level = 0;
 	init_train_bille(&Mem_1->M_game.track_list, &Mem_1->M_game);
    read_file(fichier,data);
    translate_curve_to_track(Mem_1);
    return;
    
 
}

void save_to_file(char *filename, gpointer data){
	Mydata *Mem_1 = get_mydata(data);
	Level_list *ll = &Mem_1->M_game.level_list;
    FILE* fichier = NULL;

    fichier = fopen(filename, "w+");
 
    if (fichier == NULL)
    	return;
        
 	int level_count = ll->level_count;
       
    fprintf(fichier, "%d\n", level_count); // level_count
   
    for (int i = 0; i < level_count; i++){

    	int curve_count = ll->levels[i].curve_infos.curve_list.curve_count;
    	fprintf(fichier, "%d\n", curve_count);

    	for (int k = 0; k < curve_count; k++){

    		int control_count = ll->levels[i].curve_infos.curve_list.curves[k].control_count;
    		fprintf(fichier, "%d\n", control_count);
    		for (int w = 0; w < control_count; w++){

    			double x = ll->levels[i].curve_infos.curve_list.curves[k].controls[w].x;
    			double y = ll->levels[i].curve_infos.curve_list.curves[k].controls[w].y;
    			fprintf(fichier, "%lf %lf\n", x, y);
    		}
    		
    	}
    	double canon_x = ll->levels[i].canon_x;
    	double canon_y = ll->levels[i].canon_y;
    	fprintf(fichier, "%lf %lf\n",canon_x, canon_y); // canon

    	int marble_intro = ll->levels[i].marbles_intro;
    	int marble_total = ll->levels[i].marbles_total;
    	fprintf(fichier, "%d\n",marble_intro); 
    	fprintf(fichier, "%d\n",marble_total);
    }

    fclose(fichier);
    return;
}

void on_item_menu_activate(GtkWidget *widget, gpointer data){
	Mydata *Mem_1 = get_mydata(data);
	Mem_1->M_game.state = GS_HELLO;
	Mem_1->show_edit = FALSE;
	gtk_check_menu_item_set_active (GTK_CHECK_MENU_ITEM(Mem_1->item_editing), FALSE);
}

void on_item_open_activate(GtkWidget *widget, gpointer data){
	Mydata *Mem_1 = get_mydata(data);
	GtkWidget *dialog;

	GtkFileChooserAction action = GTK_FILE_CHOOSER_ACTION_OPEN;
	gint res;

	dialog = gtk_file_chooser_dialog_new("Open File", GTK_WINDOW(Mem_1->window), action, ("Cancel"),GTK_RESPONSE_CANCEL, ("Open"),GTK_RESPONSE_ACCEPT,NULL);
	
	GtkFileChooser *chooser = GTK_FILE_CHOOSER (dialog);
	if (Mem_1->current_folder!=NULL)
			gtk_file_chooser_set_current_folder(chooser,Mem_1->current_folder);
		
	res = gtk_dialog_run (GTK_DIALOG (dialog));
	
	if (res == GTK_RESPONSE_ACCEPT){
	    char *filename;
	    
	    filename = gtk_file_chooser_get_filename (chooser);
		open_the_file (filename, data);
	    Mem_1->M_game.state = GS_PLAYING;

	    g_free(Mem_1->current_folder);
	  	Mem_1->current_folder=gtk_file_chooser_get_current_folder(chooser);
	       
		g_free (filename);	 
	}
	gtk_widget_destroy (dialog);
}

void on_item_save_activate(GtkWidget *widget, gpointer data){

	Mydata *Mem_1 = get_mydata(data);

	GtkWidget *dialog;
	GtkFileChooser *chooser;
	GtkFileChooserAction action = GTK_FILE_CHOOSER_ACTION_SAVE;
	gint res;

	dialog = gtk_file_chooser_dialog_new ("Save File",GTK_WINDOW(Mem_1->window),action,("_Cancel"),GTK_RESPONSE_CANCEL,("_Save"),GTK_RESPONSE_ACCEPT,NULL);
	chooser = GTK_FILE_CHOOSER (dialog);
	if (Mem_1->current_folder!=NULL)
		gtk_file_chooser_set_current_folder(chooser,Mem_1->current_folder);
	
	gtk_file_chooser_set_do_overwrite_confirmation (chooser, TRUE);

	res = gtk_dialog_run (GTK_DIALOG (dialog));
	if (res == GTK_RESPONSE_ACCEPT){
	    char *filename;

	    filename = gtk_file_chooser_get_filename (chooser);
	    save_to_file (filename, data);

	    g_free(Mem_1->current_folder);
	  	Mem_1->current_folder=gtk_file_chooser_get_current_folder(chooser);

	    g_free (filename);
	}
	gtk_widget_destroy (dialog);
	return;
}


void menus_init(Mydata *Mem_1){ // créer les menus

	//Menu
	
	GtkWidget *item_game, *item_level, *item_help;
	GtkWidget *sub_game, *item_start, *item_pause, *item_new, *item_nextlevel, *item_previouslevel, *item_menu, *item_quit;
	GtkWidget *sub_level, *item_save, *item_open;
	GtkWidget *sub_help, *item_About;
	//FIN Menu

	//MENU

	//création du menu
	/*#*/ Mem_1->menu_bar = gtk_menu_bar_new ();
	/*#*/ 
	/*#*/ g_object_set (gtk_settings_get_default(),"gtk-shell-shows-menubar", FALSE, NULL);

	//création des catégories du menu
	/*#*/ item_game = gtk_menu_item_new_with_label ("Game");
	/*#*/ gtk_menu_shell_append (GTK_MENU_SHELL(Mem_1->menu_bar), item_game);

					//création du sous menu de file
	/*#*//*#*//*#*/ sub_game = gtk_menu_new ();
	/*#*//*#*//*#*/ gtk_menu_item_set_submenu (GTK_MENU_ITEM(item_game), sub_game);

	/*#*//*#*//*#*/ item_start = gtk_menu_item_new_with_label ("Start");
	/*#*//*#*//*#*/ gtk_menu_shell_append (GTK_MENU_SHELL(sub_game), item_start);
	/*#*//*#*//*#*/ g_signal_connect (item_start, "activate",G_CALLBACK(on_item_start_activate), Mem_1);


	/*#*//*#*//*#*/ item_pause = gtk_menu_item_new_with_label ("Pause");
	/*#*//*#*//*#*/ gtk_menu_shell_append (GTK_MENU_SHELL(sub_game), item_pause);
	/*#*//*#*//*#*/ g_signal_connect (item_pause, "activate",G_CALLBACK(on_item_pause_activate), Mem_1);

	/*#*//*#*//*#*/ item_new = gtk_menu_item_new_with_label ("Restart");
	/*#*//*#*//*#*/ gtk_menu_shell_append (GTK_MENU_SHELL(sub_game), item_new);
	/*#*//*#*//*#*/ g_signal_connect (item_new, "activate",G_CALLBACK(on_item_new_activate), Mem_1);

	/*#*//*#*//*#*/ item_nextlevel = gtk_menu_item_new_with_label ("Next level");
	/*#*//*#*//*#*/ gtk_menu_shell_append (GTK_MENU_SHELL(sub_game), item_nextlevel);
	/*#*//*#*//*#*/ g_signal_connect (item_nextlevel, "activate",G_CALLBACK(on_item_nextlevel_activate), Mem_1);

	/*#*//*#*//*#*/ item_previouslevel = gtk_menu_item_new_with_label ("Previous level");
	/*#*//*#*//*#*/ gtk_menu_shell_append (GTK_MENU_SHELL(sub_game), item_previouslevel);
	/*#*//*#*//*#*/ g_signal_connect (item_previouslevel, "activate",G_CALLBACK(on_item_previouslevel_activate), Mem_1);

		/*#*//*#*//*#*/ item_menu = gtk_menu_item_new_with_label ("Revenir au menu");
	/*#*//*#*//*#*/ gtk_menu_shell_append (GTK_MENU_SHELL(sub_game), item_menu);
	/*#*//*#*//*#*/ g_signal_connect (item_menu, "activate",G_CALLBACK(on_item_menu_activate), Mem_1);


	/*#*//*#*//*#*/ item_quit = gtk_menu_item_new_with_label ("Quit");
	/*#*//*#*//*#*/ gtk_menu_shell_append (GTK_MENU_SHELL(sub_game), item_quit);
	/*#*//*#*//*#*/ g_signal_connect (item_quit, "activate",G_CALLBACK(on_item_quit_activate), Mem_1);


	/*#*/ item_level = gtk_menu_item_new_with_label ("Level");
	/*#*/ gtk_menu_shell_append (GTK_MENU_SHELL(Mem_1->menu_bar), item_level);

					//création du sous menu de Tools
	/*#*//*#*//*#*/ sub_level = gtk_menu_new ();
	/*#*//*#*//*#*/ gtk_menu_item_set_submenu (GTK_MENU_ITEM(item_level), sub_level);

	/*#*//*#*//*#*/ item_open = gtk_menu_item_new_with_label ("Open");
	/*#*//*#*//*#*/ gtk_menu_shell_append (GTK_MENU_SHELL(sub_level), item_open);
	/*#*//*#*//*#*/ g_signal_connect (item_open, "activate",G_CALLBACK(on_item_open_activate), Mem_1);


	/*#*//*#*//*#*/ item_save = gtk_menu_item_new_with_label ("Save");
	/*#*//*#*//*#*/ gtk_menu_shell_append (GTK_MENU_SHELL(sub_level), item_save);
	/*#*//*#*//*#*/ g_signal_connect (item_save, "activate",G_CALLBACK(on_item_save_activate), Mem_1);


	/*#*//*#*//*#*/ Mem_1->item_editing = gtk_check_menu_item_new_with_label ("Edit");
	/*#*//*#*//*#*/ gtk_menu_shell_append (GTK_MENU_SHELL(sub_level), Mem_1->item_editing);
	/*#*//*#*//*#*/ g_signal_connect (Mem_1->item_editing, "activate",G_CALLBACK(on_item_editing_activate), Mem_1);


	/*#*/ item_help = gtk_menu_item_new_with_label ("Help");
	/*#*/ gtk_menu_shell_append (GTK_MENU_SHELL(Mem_1->menu_bar), item_help);

					//création du sous menu de Help
	/*#*//*#*//*#*/ sub_help = gtk_menu_new ();
	/*#*//*#*//*#*/ gtk_menu_item_set_submenu (GTK_MENU_ITEM(item_help), sub_help);

	/*#*//*#*//*#*/ item_About = gtk_menu_item_new_with_label ("About");
	/*#*//*#*//*#*/ gtk_menu_shell_append (GTK_MENU_SHELL(sub_help), item_About);
	/*#*//*#*//*#*/ g_signal_connect (item_About, "activate",G_CALLBACK(on_item_about_activate), Mem_1);
}

