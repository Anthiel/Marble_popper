#ifndef GUI_H
#define GUI_H

void window_init(GtkApplication* app, Mydata *Mem_1); // créer la fenetre
void layout_init(Mydata *Mem_1); // créer le vbox
void status_init(Mydata *Mem_1); // créer le status en bas à gauche de la fenetre

void on_scale1_value_changed(GtkWidget *widget,gpointer data);
void win_scale_init(Mydata *Mem_1);

void editing_init(Mydata *Mem_1);
void on_edit_radio_toggled(GtkWidget *widget, gpointer data);
void on_bsp_radio_toggled(GtkWidget *widget, gpointer data);
void on_game_radio_toggled(GtkWidget *widget, gpointer data);
#endif