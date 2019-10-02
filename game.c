#include <gtk/gtk.h>
#include "curve.h"
#include "game.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "font.h"


void init_level(Level_list *ll){
	ll->level_count = 0;
}

void init_tracklist(Track_list *tl){
	tl->track_count = 0;
}

void inverse_balls(Game *M_game){
	int tmp = M_game->canon.ammo1;
	M_game->canon.ammo1 = M_game->canon.ammo2;
	M_game->canon.ammo2 = tmp;
}


void draw_trounoir(cairo_t *cr, Game *Mem_game){

	Track_list *tl = &Mem_game->track_list;

	for (int i = 0; i < tl->track_count; i++){
		Track *tk = &tl->tracks[i];
		double cx =  tk->sample_x[tk->sample_count-1];
		double cy =  tk->sample_y[tk->sample_count-1];

		int trounoir_w = cairo_image_surface_get_width (Mem_game->trounoir),
			trounoir_h = cairo_image_surface_get_height (Mem_game->trounoir);

		cairo_set_source_surface (cr, Mem_game->trounoir, cx-(trounoir_w/2.0), cy-(trounoir_h/2.0));
		cairo_rectangle (cr, cx-(trounoir_w/2.0), cy-(trounoir_h/2.0), trounoir_w, trounoir_h);
		cairo_fill (cr);

		cx =  tk->sample_x[0];
		cy =  tk->sample_y[0];

		int stargate_w = cairo_image_surface_get_width (Mem_game->stargate),
			stargate_h = cairo_image_surface_get_height (Mem_game->stargate);

		cairo_set_source_surface (cr, Mem_game->stargate, cx-(stargate_w/2.0), cy-(stargate_h/2.0));
		cairo_rectangle (cr, cx-(stargate_w/2.0), cy-(stargate_h/2.0), stargate_w, stargate_h);
		cairo_fill (cr);
	}
}

void init_train_bille(Track_list *tl, Game *M_game){
	int current_level = M_game->current_level;
	Level_list *ll = &M_game->level_list;
	M_game->score = 0;

	for (int i = 0; i < tl->track_count; i++){
		Track *tk = &tl->tracks[i];
		
		tk->marble_count = ll->levels[current_level].marbles_total;

		for (int j = 0; j < tk->marble_count; j++){
			tk->marbles[j].x = tk->sample_x[0];
			tk->marbles[j].y = tk->sample_y[0];
			tk->marbles[j].t = 0.0;
			tk->marbles[j].step_explode = 0;

			GRand *rand = g_rand_new();
			tk->marbles[j].color = g_rand_int_range (rand, 0, NB_COLOR);
		}
		tk->first_visible = tk->marble_count-1;
	}
}

int move_canon (Game *M_game, double dx, double dy){
		M_game->level_list.levels[M_game->current_level].canon_x = dx;
		M_game->level_list.levels[M_game->current_level].canon_y = dy;
		return 0;
}

void suppr_all_shoot(Game *M_game){
	M_game->shot_list.shot_count = 0;
}

void move_bille(Game *M_game){
	 Track_list *tl = &M_game->track_list;
	 double t;
	 int marble_esp = -1; // la où l'espace entre deux marbles commence

	 int current_level = M_game->current_level;
	 Level_list *ll = &M_game->level_list;

	 for (int i = 0; i < tl->track_count; i++){
	 	Track *tk = &tl->tracks[i];

	 	for (int j = tk->first_visible; j < tk->marble_count; j++){
	 		t = tk->marbles[j].t;

	 		double mx = tk->marbles[tk->marble_count-1].x;
	 		double my = tk->marbles[tk->marble_count-1].y;
	 		double sx = tk->sample_x[tk->sample_count-1];
	 		double sy = tk->sample_y[tk->sample_count-1];
	 		int rayon = 5;

	 		if ((sx-mx)*(sx-mx) + (sy-my)*(sy-my) < rayon * rayon || M_game->state == GS_LOST){
	 			t = compute_distant_point_forward (tk->sample_x, tk->sample_y, t, tk->sample_count,
		  			20.0, &tk->marbles[j].x, &tk->marbles[j].y);
	 			suppr_all_shoot(M_game);

		 		double mx0 = tk->marbles[0].x;
		 		double my0 = tk->marbles[0].y;
		 		
	 			if ((sx-mx0)*(sx-mx0) + (sy-my0)*(sy-my0) < rayon * rayon)
	 				M_game->state = GS_LOST;
	 			
	 		}
	 		else if(marble_esp != -1 && marble_esp <=j){
	 			t = compute_distant_point_forward (tk->sample_x, tk->sample_y, t, tk->sample_count,
		  			0.0, &tk->marbles[j].x, &tk->marbles[j].y);
	 		}
	 		
	 		else{
	 			if(j> tk->marble_count-1 - ll->levels[current_level].marbles_intro){	 					
		 			t = compute_distant_point_forward (tk->sample_x, tk->sample_y, t, tk->sample_count,
			  			2.0, &tk->marbles[j].x, &tk->marbles[j].y);
	 			}
		 		else{
		 			M_game->allow_shoot = 1;
		 			t = compute_distant_point_forward (tk->sample_x, tk->sample_y, t, tk->sample_count,
		 				1.0, &tk->marbles[j].x, &tk->marbles[j].y);
		  		}
	 			if(j != tk->marble_count -1 && dist_coord_x_y(tk->marbles[j].x, tk->marbles[j].y, tk->marbles[j+1].x, tk->marbles[j+1].y)>20*20){
	 				marble_esp=j+1;
	 			}
	 		}

	 		tk->marbles[j].t = t;

	 		if(t==-1)
	 			tk->marble_count--;

	 	}
	 	if (tk->first_visible > 0){

	 		if (dist_coord_bille(tk->marbles[tk->first_visible].x, tk->marbles[tk->first_visible].y, // si la distance entre le départ et la dernière bille est suffisante, on rajoute une bille
	 						tk->sample_x[0], tk->sample_y[0], 20.0) == 0){
	 			tk->first_visible--;
	 		}
	 	}
	 }
}


void move_all_bille(Game *M_game){

	Track_list *tl = &M_game->track_list; 
	for (int i = 0; i < tl->track_count; i++)
		move_bille(M_game);
}


int dist_coord_bille(double x1, double y1, double x2, double y2, double rayon){//return positif si la distance est sup ou égal au rayon

	double x = x1-x2;
	double y = y1-y2;

	if((x*x)+(y*y) >= rayon * rayon)
		return 0;
	else
		return 1;
}


int dist_coord(double x1, double y1, double x2, double y2, double rayon){ //return positif si la distance est inf ou égal au rayon

	double x = x1-x2;
	double y = y1-y2;

	if((x*x)+(y*y) <= rayon * rayon)
		return 0;
	else
		return 1;
}

int dist_coord_x_y(double x1, double y1, double x2, double y2){// renvoi une distance

	double x = x1-x2;
	double y = y1-y2;

	return (x*x)+(y*y);	
}

void afficher_score(cairo_t *cr, Game *M_game){
	PangoLayout *layout = pango_cairo_create_layout (cr);
	font_set_name (layout, "Times, 20");
	cairo_set_source_rgb (cr, 1.0,0.2,0.0);	
	font_draw_text (cr, layout, FONT_TL, 1100, 50, "score : %d",M_game->score);
}


void afficher_bille(cairo_t *cr, Curve_infos *ci, Game *M_game){
	Track_list *tl = &M_game->track_list;

	 for (int i = 0; i < tl->track_count; i++){
	 
	 	Track *tk = &tl->tracks[i];
	 	
	 	for (int j = tk->first_visible; j < tk->marble_count; j++){
	 		
	 		Marble *mb = &tk->marbles[j];

			int color = mb->color;
			double cx =  mb->x;
			double cy =  mb->y;	

			int ammo_w = cairo_image_surface_get_width (M_game->shotplanete[color]),
				ammo_h = cairo_image_surface_get_height (M_game->shotplanete[color]);
	
			cairo_set_source_surface (cr, M_game->shotplanete[color], cx-(ammo_w/2.0), cy-(ammo_h/2.0));
			cairo_rectangle (cr, cx-(ammo_w/2.0), cy-(ammo_h/2.0), ammo_w, ammo_h);
			cairo_fill (cr);

	 	}
	 	
	 }
}

void draw_background(cairo_t *cr,Game *M_game){
	int bkgd_w = cairo_image_surface_get_width (M_game->background),
		bkgd_h = cairo_image_surface_get_height (M_game->background);
	cairo_save(cr);
	cairo_scale(cr,0.4,0.4);
	cairo_set_source_surface (cr, M_game->background, 0, 0);
	cairo_rectangle (cr, 0,0, bkgd_w, bkgd_h);
	cairo_fill (cr);
	cairo_restore(cr);

}

void incre_step_explode(Game *M_game){
	Track_list *tl = &M_game->track_list;

	for (int i = 0; i < tl->track_count; i++){
	 
	 	Track *tk = &tl->tracks[i];
	 	for (int j = tk->first_visible; j < tk->marble_count; j++){
	 		if (tk->marbles[j].step_explode >=1 && tk->marbles[j].step_explode < 20)
	 			tk->marbles[j].step_explode++;
	 		
	 	}
	}
}

void update_angle_canon(Game *Mem_game, double clickx, double clicky){

	double vx = clickx-Mem_game->canon.cx,
		   vy = clicky-Mem_game->canon.cy;

	double norme = sqrt((vx*vx)+(vy*vy));

	Mem_game->canon.angle = acos(vx/norme);

	if (vy/norme < 0)
		Mem_game->canon.angle = -1 * Mem_game->canon.angle;
		

}

void draw_effet_destruc_marble(cairo_t *cr, Curve_infos *ci, Game *M_game){
	Track_list *tl = &M_game->track_list;
		
	for (int i = 0; i < tl->track_count; i++){

	 	Track *tk = &tl->tracks[i];

	 	for (int j = tk->first_visible; j < tk->marble_count; j++){
	 		if (tk->marbles[j].step_explode > 0){
	 			Marble *mb = &tk->marbles[j];
		 		
			 	double ro = M_game->col[mb->color][0];
				double ve = M_game->col[mb->color][1];
				double bl = M_game->col[mb->color][2];

				double cx =  mb->x;
				double cy =  mb->y;
				cairo_set_source_rgb (cr, 1.0,1.0,1.0);
				cairo_arc (cr,
					   cx, cy, // centre
					   mb->step_explode/2, // rayon
					   0.0, G_PI*2); // en radians
				cairo_fill(cr);

				if (tk->marbles[j].step_explode>3)
				{
					cairo_set_source_rgb (cr, ro,ve,bl);
					cairo_arc (cr,
						   cx, cy, // centre
						   mb->step_explode/2-3, // rayon
						   0.0, G_PI*2); // en radians
					cairo_fill(cr);
				}

	 		}
	 	}
	} 
}

void draw_canon(cairo_t *cr, Curve_infos *ci, Game *Mem_game){	
	Mem_game->canon.cx = Mem_game->level_list.levels[Mem_game->current_level].canon_x;
	Mem_game->canon.cy = Mem_game->level_list.levels[Mem_game->current_level].canon_y;
	double cx = Mem_game->canon.cx;
	double cy = Mem_game->canon.cy;

	cairo_save(cr);
	int ammo1 = Mem_game->canon.ammo1;
	int ammo2 = Mem_game->canon.ammo2;
	double ro1 = Mem_game->col[Mem_game->canon.ammo1][0];
	double ve1 = Mem_game->col[Mem_game->canon.ammo1][1];
	double bl1 = Mem_game->col[Mem_game->canon.ammo1][2];
	cairo_set_source_rgb (cr, ro1,ve1,bl1);


		cairo_translate(cr, cx, cy);
		cairo_rotate(cr, Mem_game->canon.angle);
		cairo_translate(cr, -cx,-cy);

	//canon
	int ima_w = cairo_image_surface_get_width (Mem_game->img_canon),
		ima_h = cairo_image_surface_get_height (Mem_game->img_canon);
	
	cairo_set_source_surface (cr, Mem_game->img_canon, cx-(ima_w/2)+8, cy-(ima_h/2));
	cairo_rectangle (cr, cx-(ima_w/2)+8, cy-(ima_h/2), ima_w, ima_h);
	cairo_fill (cr);

	//ammo 1 & 2

	int ammo1_w = cairo_image_surface_get_width (Mem_game->canonplanete[ammo1]),
		ammo1_h = cairo_image_surface_get_height (Mem_game->canonplanete[ammo1]);
	
	cairo_set_source_surface (cr, Mem_game->canonplanete[ammo1], cx-(ima_w/2)+90, cy-(ima_h/2)+19.5);
	cairo_rectangle (cr, cx-(ima_w/2)+90, cy-(ima_h/2)+19.5, ammo1_w, ammo1_h);
	cairo_fill (cr);
	
	int ammo2_w = cairo_image_surface_get_width (Mem_game->canonplanete[ammo2]),
		ammo2_h = cairo_image_surface_get_height (Mem_game->canonplanete[ammo2]);
	
	cairo_set_source_surface (cr, Mem_game->canonplanete[ammo2], cx-(ima_w/2)+64, cy-(ima_h/2)+19.5);
	cairo_rectangle (cr, cx-(ima_w/2)+64, cy-(ima_h/2)+19.5, ammo2_w, ammo2_h);
	cairo_fill (cr);
	
	cairo_restore(cr);

}


void color(Game *Mem_game){
	double color[NB_COLOR][3] = {{1.0, 0.0, 0.0}, // rouge
								 {0.0, 0.75, 0.0},// vert
								 {1.0, 1.0, 0.0},// jaune
								 {0.0, 0.5, 0.5}, // bleu
								 {0.5, 0.0, 0.5}}; // violet
	
	for (int i = 0; i < 5; i++){
		Mem_game->col[i][0] = color[i][0];
		Mem_game->col[i][1] = color[i][1];
		Mem_game->col[i][2] = color[i][2];
	}
	
}


int add_shoot(Curve_infos *ci, Game *Mem_game){
	
	int shot_count = Mem_game->shot_list.shot_count;

	if (shot_count >= SHOT_MAX)
		return -1;

	Shot *cur_shoot = &Mem_game->shot_list.shots[shot_count];

	cur_shoot->dx=cos(Mem_game->canon.angle);
	cur_shoot->dy=sin(Mem_game->canon.angle);

	double dx = cur_shoot->dx;
	double dy = cur_shoot->dy;
	
	double cx = Mem_game->canon.cx;
	cur_shoot->x= cx + dx * 50.0;
	double cy = Mem_game->canon.cy;
	cur_shoot->y= cy + dy * 50.0;

	cur_shoot->color = Mem_game->canon.ammo1;
	Mem_game->canon.ammo1 = Mem_game->canon.ammo2;
	Mem_game->rand = g_rand_new();
	Mem_game->canon.ammo2 = g_rand_int_range (Mem_game->rand, 0, NB_COLOR);
	Mem_game->shot_list.shot_count++;

	return 0;
}


void draw_shoot(cairo_t *cr, Curve_infos *ci, Game *Mem_game){

	int shot_count = Mem_game->shot_list.shot_count;


	for (int i = 0; i < shot_count; i++){

		int color = Mem_game->shot_list.shots[i].color;
		
		double cx =  Mem_game->shot_list.shots[i].x;
		double cy =  Mem_game->shot_list.shots[i].y;

		int ammo_w = cairo_image_surface_get_width (Mem_game->shotplanete[color]),
			ammo_h = cairo_image_surface_get_height (Mem_game->shotplanete[color]);
	
		cairo_set_source_surface (cr, Mem_game->shotplanete[color], cx-(ammo_w/2.0), cy-(ammo_h/2.0));
		cairo_rectangle (cr, cx-(ammo_w/2.0), cy-(ammo_h/2.0), ammo_w, ammo_h);
		cairo_fill (cr);
	
	}
}


void sample_curve_to_track (Curve *curve, Track *track, double theta){
  Control bez_points[4];
  int ind = 0;

  if (curve->control_count < 3) return;

  compute_bezier_points_prolong_first (curve, bez_points);
  sample_bezier_curve (bez_points, theta, 
    track->sample_x, track->sample_y, &ind, SAMPLE_MAX, 1);

  for (int k = 0; k < curve->control_count-3; k++) {
    compute_bezier_points_open (curve, k, bez_points);
    sample_bezier_curve (bez_points, theta, 
      track->sample_x, track->sample_y, &ind, SAMPLE_MAX, 0);
  }

  compute_bezier_points_prolong_last (curve, bez_points);
  sample_bezier_curve (bez_points, theta, 
    track->sample_x, track->sample_y, &ind, SAMPLE_MAX, 0);

  track->sample_count = ind;
}


void affichage_track(cairo_t *cr, Track_list *track_list){
	cairo_save(cr);
	cairo_set_line_width(cr, 28);
	cairo_set_source_rgb (cr, 0.6,0.6,0.6);
	
	for (int j = 0; j < track_list->track_count; j++)
	{
		Track *track = &track_list->tracks[j];
		if (track->sample_count<=1)
			break;

		cairo_move_to(cr,track->sample_x[0], track->sample_y[0]);

		for (int i = 1; i < track->sample_count; i++)
		{
			cairo_line_to(cr,track->sample_x[i], track->sample_y[i]);
		}
		cairo_stroke(cr);
	}
	cairo_restore(cr);

	cairo_save(cr);
	cairo_set_line_width(cr, 24);
	cairo_set_source_rgb (cr, 0.5,1.0,1.0);
	
	for (int j = 0; j < track_list->track_count; j++)
	{
		Track *track = &track_list->tracks[j];
		if (track->sample_count<=1)
			break;

		cairo_move_to(cr,track->sample_x[0], track->sample_y[0]);

		for (int i = 1; i < track->sample_count; i++)
		{
			cairo_line_to(cr,track->sample_x[i], track->sample_y[i]);
		}
		cairo_stroke(cr);
	}
	cairo_restore(cr);


	cairo_save(cr);
	cairo_set_line_width(cr, 20);
	cairo_set_source_rgb (cr, 0.6,0.6,0.6);
	
	for (int j = 0; j < track_list->track_count; j++)
	{
		Track *track = &track_list->tracks[j];
		if (track->sample_count<=1)
			break;

		cairo_move_to(cr,track->sample_x[0], track->sample_y[0]);

		for (int i = 1; i < track->sample_count; i++)
		{
			cairo_line_to(cr,track->sample_x[i], track->sample_y[i]);
		}
		cairo_stroke(cr);
	}
	cairo_restore(cr);
	
	
}

void suppress_step_explode(Game *M_game){
	 Track_list *tl = &M_game->track_list;
	int sum = 0;
	 for (int i = 0; i < tl->track_count; i++){

	 	Track *tk = &tl->tracks[i];

	 	for (int j = tk->first_visible; j < tk->marble_count; j++){
	 		if (tk->marbles[j].step_explode >= 20){
	 			reduire_mar(tk,j,j+1);
	 			sum++;
	 		}
	 	}
	 	M_game->score += sum*10;
	} 	
}

void combo_0(Game *M_game, int bille, Track *tk){
	int depart = bille, arrivee = bille;
	int color = tk->marbles[bille].color;
	
	if(bille < 0 || bille > tk->marble_count-1){
		printf("error : combo_0\n");
		return;
	}
	else if(bille == 0){
		int tmp_bille = bille;
		while(tk->marbles[tmp_bille].color == color){
			arrivee++;
			tmp_bille++;
		}
		if (tk->marble_count == 2 || tk->marble_count == 1)
			return;
		if(arrivee-depart >= 3 && tk->marbles[bille+1].step_explode == 0)	
			init_step_expl(tk, depart, arrivee-1);
			
	}
	else if(bille == tk->marble_count-1){
		int tmp_bille = bille;
		while(tk->marbles[tmp_bille].color == color){
			depart--;
			tmp_bille--;
		}
		
		if(arrivee-depart >= 3&& tk->marbles[bille-1].step_explode == 0)
			init_step_expl(tk, depart+1, arrivee);		
	}
	else {
		int tmp_bille = bille;
		
		while(tk->marbles[tmp_bille-1].color == color){
			
			if (tmp_bille<=0 || depart <=0)break;
			
			depart--;
			tmp_bille--;
			
		}
		
		tmp_bille = bille;

		while(tk->marbles[tmp_bille+1].color == color){
			
			if (tmp_bille > tk->marble_count || arrivee > tk->marble_count)break;

			arrivee++;
			tmp_bille++;
			
		}
		
		if(arrivee-depart >= 2){
			
			if (tk->marbles[arrivee].step_explode == 0 && tk->marbles[depart].step_explode == 0){
				init_step_expl(tk, depart, arrivee);
			}
		}
	}


}

void reduire_mar(Track *track, int A, int B){ // deplace de 1 marble en arrière, de i jusqu'à marble count
	int longueur = B-A;
	if(track->marble_count+1>=MARBLE_MAX)
		return;
	for(int a=A; a<track->marble_count; a++){
		track->marbles[a] = track->marbles[a+longueur];	
	}
	track->marble_count -= longueur;
}

void init_step_expl(Track *track, int A, int B){ // deplace de 1 marble en arrière, de i jusqu'à marble count
	
	
	if(track->marble_count+1>=MARBLE_MAX)
		return;
	for(int a=A; a<=B; a++)		
		track->marbles[a].step_explode = 1;	
	
}

void agrandir_mar(Track *track, int i){ // deplace de 1 marble, de i jusqu'à marble count

	if(track->marble_count+1>=MARBLE_MAX)
		return;
	for(int a= track->marble_count; a>i; a--){
		track->marbles[a] = track->marbles[a-1];	
	}
}

void reculer_bille(Track *tk, int i){ // fait avancer tes billes de i+1 jusqu'à marble_count

	double t;	
	if (dist_coord(tk->sample_x[0], tk->sample_y[0], tk->marbles[0].x, tk->marbles[0].y, 20) == 0){
		
		return;
	}
	
	for (i=i; i<tk->marble_count ; i++){
		
		t = tk->marbles[i].t;
		t = compute_distant_point_backward (tk->sample_x, tk->sample_y, t, tk->sample_count,
		  			20.0, &tk->marbles[i].x, &tk->marbles[i].y);
		tk->marbles[i].t = t;
	}
}

void avancer_bille(Track *tk, int i){ // fait avancer tes billes de i+1 jusqu'à marble_count

	double t;	
	for (i=i; i<tk->marble_count ; i++){
		t = tk->marbles[i].t;
		t = compute_distant_point_forward (tk->sample_x, tk->sample_y, t, tk->sample_count,
		  			20.0, &tk->marbles[i].x, &tk->marbles[i].y);
		tk->marbles[i].t = t;
	}
}

void put_last_ball (Game *M_game, int j, Track *tk, int k){
	Shot_list *sl = &M_game->shot_list;
	int bille_proche = j;
	agrandir_mar(tk, bille_proche);
	tk->marble_count++;
						
	avancer_bille(tk, bille_proche+1);
						
	tk->marbles[bille_proche].color = sl->shots[k].color;
	tk->marbles[bille_proche].step_explode = 0;
	memmove(M_game->shot_list.shots+k, M_game->shot_list.shots+k+1, sizeof(Shot)*(M_game->shot_list.shot_count-1-k));
	M_game->shot_list.shot_count--;
}

void put_first_ball (Game *M_game, int j, Track *tk, int k){
	Shot_list *sl = &M_game->shot_list;
	int bille_proche = j;
					
	avancer_bille(tk, bille_proche+1);					
	tk->marbles[bille_proche+1].color = sl->shots[k].color;

	double t = tk->marbles[bille_proche].t;
	tk->marble_count++;
	t = compute_distant_point_forward (tk->sample_x, tk->sample_y, t, tk->sample_count,
		  													20.0, &tk->marbles[bille_proche+1].x, &tk->marbles[bille_proche+1].y);	
	tk->marbles[bille_proche+1].t = t;
	tk->marbles[bille_proche+1].step_explode = 0;
						
	memmove(M_game->shot_list.shots+k, M_game->shot_list.shots+k+1, sizeof(Shot)*(M_game->shot_list.shot_count-1-k));
	M_game->shot_list.shot_count--;
}

void put_after_ball (Game *M_game, int j, Track *tk, int k){
	Shot_list *sl = &M_game->shot_list;
	int bille_proche = j+1;
	
	agrandir_mar(tk, bille_proche);						
						
	if(tk->marble_count == MARBLE_MAX){printf("erreur marble_count == marble_max");return;}
						
	tk->marbles[bille_proche].color = sl->shots[k].color;
	double t = tk->marbles[bille_proche+1].t;
	tk->marble_count++;

	t = compute_distant_point_forward (tk->sample_x, tk->sample_y, t, tk->sample_count,
			  							20.0, &tk->marbles[bille_proche+1].x, &tk->marbles[bille_proche+1].y);
		 				
	tk->marbles[bille_proche+1].t = t;
	tk->marbles[bille_proche+1].step_explode = 0;

	avancer_bille(tk, bille_proche+2);

	memmove(M_game->shot_list.shots+k, M_game->shot_list.shots+k+1, sizeof(Shot)*(M_game->shot_list.shot_count-1-k));
	M_game->shot_list.shot_count--;
}

void put_before_ball (Game *M_game, int j, Track *tk, int k){
	Shot_list *sl = &M_game->shot_list;
	int bille_proche = j-1;
	
	agrandir_mar(tk, bille_proche);
			
	if(tk->marble_count == MARBLE_MAX){printf("erreur marble_count == marble_max");return;}
		
	tk->marbles[bille_proche+1].color = sl->shots[k].color;
	double t = tk->marbles[bille_proche+1].t;
	tk->marble_count++;

	t = compute_distant_point_forward (tk->sample_x, tk->sample_y, t, tk->sample_count,
						 			20.0, &tk->marbles[bille_proche+1].x, &tk->marbles[bille_proche+1].y);
	tk->marbles[bille_proche+1].t = t;
	tk->marbles[bille_proche+1].step_explode = 0;
	avancer_bille(tk, bille_proche+2);

	memmove(M_game->shot_list.shots+k, M_game->shot_list.shots+k+1, sizeof(Shot)*(M_game->shot_list.shot_count-1-k));
	M_game->shot_list.shot_count--;
}

void check_collision(Game *M_game){

	 Track_list *tl = &M_game->track_list;
	 Shot_list *sl = &M_game->shot_list;
	
	 for (int i = 0; i < tl->track_count; i++){

	 	Track *tk = &tl->tracks[i];

	 	for (int j = tk->first_visible; j < tk->marble_count; j++){

	 		int tx = tk->marbles[j].x;
	 		int ty = tk->marbles[j].y;	 		

	 		for (int k = 0; k < sl->shot_count; k++){
	 			int xs = sl->shots[k].x;
	 			int ys = sl->shots[k].y;
	 				 			
	 			if (dist_coord(xs,ys, tx, ty, 20.0) == 0){
	 				
					if (tk->marbles[j].step_explode != 0){						
						suppr_all_shoot(M_game);
						return;
					}
					if (j == 0 && dist_coord_x_y(xs, ys, tk->marbles[j+1].x, tk->marbles[j+1].y)>20.0*20.0){ // quand ça touche la dernière bille
						printf("last balls\n");
						put_last_ball(M_game, j, tk, k);
						combo_0(M_game, j, tk);
						return;						
					}
					else if(j == tk->marble_count-1 && dist_coord_x_y(xs, ys, tk->marbles[j-1].x, tk->marbles[j-1].y)>20.0*20.0){ // quand ça touche la première bille
						printf("first balls\n");
						put_first_ball(M_game, j, tk, k);	
						combo_0(M_game, j+1, tk);					
						return;						
					}		
					else if (dist_coord_x_y(xs, ys, tk->marbles[j+1].x, tk->marbles[j+1].y)<dist_coord_x_y(xs, ys, tk->marbles[j-1].x, tk->marbles[j-1].y)){
						printf("put after balls\n");
						put_after_ball(M_game, j, tk, k);
						combo_0(M_game, j+1, tk);
						return;
					}
					else if (dist_coord_x_y(xs, ys, tk->marbles[j+1].x, tk->marbles[j+1].y)>=dist_coord_x_y(xs, ys, tk->marbles[j-1].x, tk->marbles[j-1].y)){
						printf("put before balls\n");
						put_before_ball(M_game, j, tk, k);
						combo_0(M_game, j, tk);
						return;
					}
				}
	 		}
	  	}
	 }

}
