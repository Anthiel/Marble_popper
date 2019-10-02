#ifndef GAME_H
#define GAME_H
#include <math.h>

#define NB_COLOR 5
#define NB_BILLE 5

typedef enum { GS_HELLO, GS_PLAYING, GS_PAUSE, GS_WON, GS_LOST } Game_state;
typedef enum { TS_INTRO, TS_NORMAL, TS_COMBO1, TS_COMBO2, TS_COMBO3} Track_state;

#define SHOT_MAX       10
#define SHOT_SPEED     10.0
#define TRACK_MAX      10
#define MARBLE_MAX    200
#define SAMPLE_MAX   1000 
#define LEVEL_MAX      10
#define SAMPLE_THETA    0.05

typedef struct {
  double cx, cy;    // centre canon
  double angle;     // en radians
  int ammo1, ammo2;
} Canon;

typedef struct {
  double x, y;      // coordonnées centre
  double dx, dy;    // vecteur déplacement
  int color;
} Shot;

typedef struct {
  int shot_count;
  Shot shots[SHOT_MAX];
} Shot_list;

typedef struct {
  double x, y;      // coordonnées centre
  double t;         // paramètre dans l'échantillonnage
  int color;
  int is_combo_end; // ou encore, facteur vitesse et direction ?
  int step_explode;
} Marble;

typedef struct {
  int    sample_count;          // échantillonnage courbe
  double sample_x[SAMPLE_MAX], 
         sample_y[SAMPLE_MAX];
  int marble_count;
  int first_visible;
  Marble marbles[MARBLE_MAX];
  Track_state state;
} Track;

typedef struct {
  int track_count;
  Track tracks[TRACK_MAX];
} Track_list;

typedef struct {
  Curve_infos curve_infos;
  double canon_x, canon_y;
  int marbles_intro, marbles_total;
} Level;

typedef struct {
  int level_count;
  Level levels[LEVEL_MAX];
} Level_list;

typedef struct {
  Game_state state;
  int current_level;
  int score;
  int allow_shoot;
  Canon canon;
  Shot_list shot_list;
  Track_list track_list;
  Level_list level_list;

  cairo_surface_t *img_canon;
  cairo_surface_t *background;
  cairo_surface_t *trounoir;
  cairo_surface_t *stargate;

  cairo_surface_t *menu_logo;   
  cairo_surface_t *menu_play;
  cairo_surface_t *menu_open; 
  cairo_surface_t *menu_new;   
  cairo_surface_t *menu_pouki;  
  cairo_surface_t *menu_menu; 
  cairo_surface_t *menu_next; 

  cairo_surface_t *canonplanete[5];
  cairo_surface_t *shotplanete[5];
 

  GRand *rand;
    
  double col[5][3];
  
} Game;

void init_tracklist(Track_list *tl);
void init_level(Level_list *ll);
void update_angle_canon(Game *Mem_game, double clickx, double clicky);
void draw_canon(cairo_t *cr, Curve_infos *ci, Game *Mem_game);
int add_shoot(Curve_infos *ci, Game *Mem_game);
void color(Game *Mem_game);
void draw_shoot(cairo_t *cr, Curve_infos *ci, Game *Mem_game);
void init_train_bille(Track_list *tl, Game *M_game);
void sample_curve_to_track (Curve *curve, Track *track, double theta);
void affichage_track(cairo_t *cr, Track_list *track_list);
void draw_background(cairo_t *cr,Game *M_game);
void afficher_bille(cairo_t *cr, Curve_infos *ci,Game *M_game);

int dist_coord_bille(double x1, double y1, double x2, double y2, double rayon);
int dist_coord(double x1, double y1, double x2, double y2, double rayon);
void check_collision(Game *Mem_game);
void inverse_balls(Game *M_game);
void move_bille(Game *M_game);
void move_all_bille(Game *M_game);
void check_dist_secur(Game *M_game);
void reduire_mar(Track *track, int A, int B);
void reculer_bille(Track *tk, int i);
int dist_coord_x_y(double x1, double y1, double x2, double y2);
void init_step_expl(Track *track, int A, int B);
void incre_step_explode(Game *M_game);
void suppress_step_explode(Game *M_game);
void draw_effet_destruc_marble(cairo_t *cr, Curve_infos *ci, Game *M_game);
int move_canon (Game *M_game, double dx, double dy);
void draw_trounoir(cairo_t *cr, Game *Mem_game);
void afficher_score(cairo_t *cr, Game *M_game);
void suppr_all_shoot(Game *M_game);
#endif
