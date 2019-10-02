#ifndef MENUS_H
#define MENUS_H

#include <string.h>

void menus_init(Mydata *Mem_1); // créer les menus
void open_the_file(char *filename, gpointer data);
void on_item_open_activate(GtkWidget *widget, gpointer data);
void on_item_editing_activate(GtkWidget *widget, gpointer data);
void on_item_nextlevel_activate(GtkWidget *widget, gpointer data);
#endif