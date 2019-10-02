#ifndef MYDATA_H
#define MYDATA_H

#define MYDATA_MAGIC 0x46EA7E05
#define NB_COLOR 5




enum EDIT {EDIT_NONE, EDIT_ADD_CURVE, EDIT_MOVE_CURVE, EDIT_REMOVE_CURVE, EDIT_ADD_CONTROL, EDIT_MOVE_CONTROL, EDIT_REMOVE_CONTROL, EDIT_MOVE_CANON, EDIT_LAST};

enum BSP {BSP_FIRST, BSP_OPEN, BSP_CLOSE, BSP_PROLONG, BSP_FILL, BSP_CLIP, BSP_LAST};

enum GAME {GAME_FIRST, GAME_CANON, GAME_LAST};

typedef struct {

	unsigned int magic;

	int win_height;
	int win_width;

	int area_width;
	int area_height;

	int clip_image; //menu clic coché
	int show_edit; //menu editing coché

	double click_x; //coordonnées de la souris
	double click_y;
	int click_n; //numéro du bouton enfoncé de la souris, 0 pour aucun

	double scale1_value;
	double rotate_angle;


	GtkWidget *window;
	GtkWidget *vbox1;
	GtkWidget *hbox1; // pour editing
	GtkWidget *buttonbox; //bouton radio editing
	GtkWidget *item_clip;
	GtkWidget *item_editing;
	//GtkWidget *image1;
	GtkWidget *area1;
	GtkWidget *status;
	GtkWidget *win_scale;
	GtkWidget *scale1; //gtkscale horizontale
	GtkWidget *scroll; // le scroll
	GtkWidget *menu_bar; // bar menu
	GtkWidget *frame1; // frame editing
 	GtkWidget *b_marble_intro;
 	GtkWidget *b_marble_total;

	char *title;
	char *current_folder;


	int tmp_areadraw_init;
	GdkPixbuf *pixbuf1;
	GdkPixbuf *pixbuf2;

	char *auteur[2];


	gint edit_mode;
	GtkWidget *edit_radios[EDIT_LAST];

	Curve_infos curve_infos;

	double last_x, last_y;
	
	int bsp_mode;

	GtkWidget *bsp_radios[BSP_LAST];

	//game

	
	GtkWidget *game_radios[GAME_LAST];
	int game_mode;

	Game M_game;
	int tmp_game;

	int stop_billes;





} Mydata;







Mydata *get_mydata (gpointer data);
void init_memory(Mydata *memory);
void set_edit_mode (Mydata *Mem_1, int mode);
void init_game_infos (Game *M_game, Mydata *Mem_1, GtkWidget *area);
void level1(Mydata *Mem_1);
void translate_curve_to_track(Mydata *Mem_1);
gint grab_int_value (GtkSpinButton *button, gpointer user_data);


#endif