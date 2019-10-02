#include <gtk/gtk.h>


#include "util.h"
#include "curve.h"
#include "game.h"
#include "mydata.h"

#include "drawings.h"
#include "menus.h"
#include "gui.h"






void window_init(GtkApplication* app, Mydata *Mem_1){ // créer la fenetre

	Mem_1->pixbuf1 = NULL;		
	Mem_1->window = gtk_application_window_new (app); //création de la fenetre

	gtk_window_set_title (GTK_WINDOW(Mem_1->window), Mem_1->title); // on met un titre à la fênetre
	gtk_window_set_default_size (GTK_WINDOW(Mem_1->window), Mem_1->win_width,Mem_1->win_height); //taille de la fenetre
}

void layout_init(Mydata *Mem_1){ // créer le vbox

	Mem_1->vbox1 = gtk_box_new (GTK_ORIENTATION_VERTICAL, 0); //création de la zone pour le bouton
	Mem_1->hbox1 = gtk_box_new (GTK_ORIENTATION_HORIZONTAL, 0); 

	gtk_container_add(GTK_CONTAINER (Mem_1->window), Mem_1->vbox1);
	gtk_box_pack_start (GTK_BOX (Mem_1->vbox1), Mem_1->menu_bar, FALSE, FALSE, 0); // Menu

	Mem_1->scroll = gtk_scrolled_window_new(NULL,NULL);	
	gtk_box_pack_start (GTK_BOX(Mem_1->vbox1),Mem_1->hbox1,TRUE,TRUE,0);
	gtk_box_pack_start (GTK_BOX(Mem_1->hbox1),Mem_1->frame1,FALSE,TRUE,2); //ajoute la boite editing
	gtk_box_pack_start (GTK_BOX(Mem_1->hbox1),Mem_1->scroll,TRUE,TRUE,0); //ajouter le scroll a vbox
	
	gtk_container_add(GTK_CONTAINER (Mem_1->scroll), Mem_1->area1); //scroll
	gtk_box_pack_start (GTK_BOX (Mem_1->vbox1), Mem_1->status, FALSE,FALSE, 0);	
	g_signal_connect (Mem_1->b_marble_total, "value-changed",G_CALLBACK(on_marbl_total_change_notify), Mem_1);
	g_signal_connect (Mem_1->b_marble_intro, "value-changed",G_CALLBACK(on_marbl_intro_change_notify), Mem_1);
	
}

void status_init(Mydata *Mem_1){ // créer le status en bas à gauche de la fenetre

	Mem_1->status = gtk_statusbar_new(); // création du widget pour le status en bas à gauche de la fênetre
	set_status(Mem_1->status,"WELCOME DANS LE JEU");
}

void on_scale1_value_changed(GtkWidget *widget,gpointer data){
	Mydata *Mem_1 = get_mydata(data);
	double tmp_scale=gtk_range_get_value(GTK_RANGE(widget));
	if (Mem_1->scale1_value==tmp_scale)
		return;
	
	Mem_1->scale1_value=tmp_scale;

	set_status (Mem_1->status, "Scale %.2lf",Mem_1->scale1_value); // on met le status scale

	update_area1_with_transforms(Mem_1);
}

void win_scale_init(Mydata *Mem_1){
	Mem_1->win_scale = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	Mem_1->scale1_value = 1.0;

	gtk_window_set_title (GTK_WINDOW(Mem_1->win_scale), "Image scale"); 
	
	gtk_window_set_default_size (GTK_WINDOW(Mem_1->win_scale), 300,100); //taille de la fenetre

	GtkWidget *box = gtk_box_new (GTK_ORIENTATION_HORIZONTAL, 4); //création de la zone pour le bouton
	gtk_container_add(GTK_CONTAINER (Mem_1->win_scale), box);

	GtkWidget *label = gtk_label_new("Scale:");
	gtk_box_pack_start (GTK_BOX(box),label,FALSE,TRUE,4);

	Mem_1->scale1 = gtk_scale_new_with_range (GTK_ORIENTATION_HORIZONTAL, 0.02, 20.0, 0.02);
	gtk_box_pack_start (GTK_BOX(box),Mem_1->scale1,TRUE,TRUE,10);


	gtk_range_set_value(GTK_RANGE(Mem_1->scale1), 1.0);

	g_signal_connect (Mem_1->scale1, "value-changed",G_CALLBACK(on_scale1_value_changed), Mem_1);
	gtk_widget_show_all(Mem_1->win_scale);
	gtk_widget_hide(Mem_1->win_scale);
}

void editing_init(Mydata *Mem_1){
	GtkWidget *separator;
	Mem_1->frame1 = gtk_frame_new ("Editing");
	separator = gtk_separator_new (GTK_ORIENTATION_HORIZONTAL);
	Mem_1->buttonbox = gtk_box_new (GTK_ORIENTATION_VERTICAL, 0);

	int nb_bouton_edit = 7;
	char *edit_noms[7] = {"Add curve", "Move curve", "Remove curve","Add control", "Move control", "Remove control", "Move canon"};

	for (int i = 0; i < nb_bouton_edit; i++){
		Mem_1->edit_radios[i] = gtk_radio_button_new_with_label_from_widget (i == 0 ? NULL : GTK_RADIO_BUTTON(Mem_1->edit_radios[0]),edit_noms[i]);
		g_object_set_data (G_OBJECT(Mem_1->edit_radios[i]), "mode", GINT_TO_POINTER(i+1));
		g_signal_connect (Mem_1->edit_radios[i], "toggled",	G_CALLBACK(on_edit_radio_toggled), Mem_1);
		gtk_box_pack_start (GTK_BOX (Mem_1->buttonbox),	Mem_1->edit_radios[i], FALSE, FALSE, 0);
	}

	gtk_box_pack_start (GTK_BOX (Mem_1->buttonbox),	separator, FALSE, FALSE, 0);

    GtkAdjustment *adjustment;
    adjustment = gtk_adjustment_new (0.0, 0.0, MARBLE_MAX, 1.0, 5.0, 0.0);

    GtkAdjustment *adjustment1;
    adjustment1 = gtk_adjustment_new (0.0, 0.0, MARBLE_MAX/2, 1.0, 5.0, 0.0);

  
 	GtkWidget *label1 = gtk_label_new ("marbles intro");
 	gtk_box_pack_start (GTK_BOX (Mem_1->buttonbox), label1, FALSE, FALSE, 0);

    Mem_1->b_marble_intro = gtk_spin_button_new (adjustment, 1.0, 0);
    gtk_box_pack_start (GTK_BOX (Mem_1->buttonbox), Mem_1->b_marble_intro, FALSE, FALSE, 0);

    GtkWidget *label2 = gtk_label_new ("marbles total");
    gtk_box_pack_start (GTK_BOX (Mem_1->buttonbox), label2, FALSE, FALSE, 0);

    Mem_1->b_marble_total = gtk_spin_button_new (adjustment1, 1.0, 0);
    gtk_box_pack_start (GTK_BOX (Mem_1->buttonbox), Mem_1->b_marble_total, FALSE, FALSE, 0);

	gtk_container_add(GTK_CONTAINER (Mem_1->frame1), Mem_1->buttonbox);
}

void on_edit_radio_toggled(GtkWidget *widget, gpointer data){	
	Mydata *Mem_1 = get_mydata(data);
	Mem_1->edit_mode = GPOINTER_TO_INT(g_object_get_data (G_OBJECT(widget), "mode"));
}

void on_bsp_radio_toggled(GtkWidget *widget, gpointer data){
	
	Mydata *Mem_1 = get_mydata(data);
	
	Mem_1->bsp_mode = GPOINTER_TO_INT(g_object_get_data (G_OBJECT(widget), "mode"));
	refresh_area(Mem_1->area1);
}


void on_game_radio_toggled(GtkWidget *widget, gpointer data){
	
	Mydata *Mem_1 = get_mydata(data);
		
	Mem_1->game_mode = GPOINTER_TO_INT(g_object_get_data (G_OBJECT(widget), "mode"));
	refresh_area(Mem_1->area1);
	printf("%d\n", Mem_1->game_mode);
}



