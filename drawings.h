#ifndef DRAWINGS_H
#define DRAWINGS_H



void apply_image_transforms(Mydata *Mem_1); // rotate pour pixbuf2 et area avec un angle donné
void update_area1_with_transforms(Mydata *Mem_1);


void draw_control_polygons (cairo_t *cr, Curve_infos *ci);

void init_canon(Game *M_game, Mydata *Mem_1);
void area1_init(Mydata *Mem_1); // créer l'image avec le scroll
void draw_control_labels(cairo_t *cr, PangoLayout *layout, Curve_infos *ci);
void draw_bezier_polygons_open (cairo_t *cr,Curve_infos *ci);
void draw_bezier_curve (cairo_t *cr, Control bez_points[4], double theta);
void draw_bezier_curves_open (cairo_t *cr, Curve_infos *ci, double theta);
void draw_bezier_curves_close (cairo_t *cr, Curve_infos *ci, double theta);
void draw_bezier_curves_prolong (cairo_t *cr, Curve_infos *ci, double theta);
void generate_bezier_path (cairo_t *cr, Control bez_points[4], double theta, int is_first);

void draw_bezier_curves_fill (cairo_t *cr, Curve_infos *ci, double theta);
void draw_bezier_curves_clip (cairo_t *cr, Curve_infos *ci, Mydata *Mem_1);
gboolean on_timeout1 (gpointer data);
gboolean suppress_far_shots (gpointer data);
gboolean on_marbl_intro_change_notify (GtkWidget *widget,GdkEvent *event, gpointer data);

gboolean on_marbl_total_change_notify(GtkWidget *widget,GdkEvent *event, gpointer data);
void draw_lose(cairo_t *cr, Mydata *Mem_1);
void draw_won(cairo_t *cr, Mydata *Mem_1);
#endif