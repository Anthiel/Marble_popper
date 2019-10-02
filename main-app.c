#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <gtk/gtk.h>
#include <math.h>


#include "main-app.h"
#include "curve.h"
#include "game.h"
#include "mydata.h"


#include "util.h"
#include "drawings.h"
#include "menus.h"
#include "gui.h"




void on_app_activate (GtkApplication* app, gpointer user_data)
{	
	Mydata *Mem_1 = get_mydata(user_data);

	window_init(app, Mem_1); //créer la fênetre
	
	menus_init(Mem_1); // créer les menus
	area1_init(Mem_1); // créer l'image avec le scroll
	status_init(Mem_1); // créer le status en bas à gauche de la fenetre
	editing_init(Mem_1); // créer l'édition
	layout_init(Mem_1); // créer le vbox1
	win_scale_init(Mem_1);

	gtk_widget_show_all (Mem_1->window);
	g_timeout_add (20, on_timeout1, Mem_1);
	gtk_widget_hide (Mem_1->frame1);
}

int main (int argc, char *argv[])
{
	GtkApplication *app;
	int status;
	Mydata Mem_1;
	
    init_memory(&Mem_1);

	app = gtk_application_new (NULL, G_APPLICATION_FLAGS_NONE);
	g_signal_connect (app, "activate",G_CALLBACK(on_app_activate), &Mem_1);
	status = g_application_run (G_APPLICATION(app), argc, argv);

	g_object_unref (app);
	return status;
}
