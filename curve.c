#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <gtk/gtk.h>
#include <math.h>
#include "curve.h"



void init_curve_infos (Curve_infos *ci){
	ci->curve_list.curve_count = 0;
	ci->current_curve = -1;
	ci->current_control = -1;
}


int add_curve(Curve_infos *ci){
	if (ci->curve_list.curve_count>=CURVE_MAX){
		ci->current_curve = -1;
		return -1;
	}
	int n = ci->curve_list.curve_count;
	ci->curve_list.curve_count++;
	ci->curve_list.curves[n].control_count=0;
	ci->current_curve = n;
	ci->current_control = -1;

	ci->curve_list.curves[n].shift_x=0;
	ci->curve_list.curves[n].shift_y=0;
	
	return n;
}

int remove_curve(Curve_infos *ci){
	int n = ci->current_curve;

	if (n<0 || n>ci->curve_list.curve_count-1)
		return -1;
	
	memmove (ci->curve_list.curves+n, ci->curve_list.curves+n+1, // dest, source
		sizeof(Curve)*(ci->curve_list.curve_count-1-n)); //taille

	ci->curve_list.curve_count--;
	ci->current_curve = -1;
	return 0;
}

int add_control(Curve_infos *ci, double x, double y){
	int n = ci->current_curve;
	
	if (n < 0 || n > ci->curve_list.curve_count-1)
		return -1;


	if (ci->curve_list.curves[n].control_count>=CONTROL_MAX){
		ci->current_control = -1;
		return -1;
	}
	
	int k = ci->curve_list.curves[n].control_count;
	ci->curve_list.curves[n].control_count++;
	
	ci->curve_list.curves[n].controls[k].x = x;
	ci->curve_list.curves[n].controls[k].y = y;

	ci->current_control = k;
	return k;
}

int find_control (Curve_infos *ci, double x, double y){
		int i,j;
		int dx, dy;
	
		for (i = 0; i < ci->curve_list.curve_count; i++){
			for (j = 0; j < ci->curve_list.curves[i].control_count; j++){				
				dx = ci->curve_list.curves[i].controls[j].x - x;
				dy = ci->curve_list.curves[i].controls[j].y - y; // différence entre les coords de la souris et du point de controle

				if(dx*dx + dy*dy <= 5*5){// si y a une diff egale ou inf a 5
					ci->current_curve = i;
					ci->current_control = j;			
					return 0;
				}
			}
		}
		ci->current_curve = -1;
		ci->current_control = -1;
			
		return -1;	
}


int move_control (Curve_infos *ci, double dx, double dy){

	int cv = ci->current_curve;
	int cc = ci->current_control;

	if (cv<0 || cv >= ci->curve_list.curve_count) return -1;
	Curve *curve = &ci->curve_list.curves[cv];
		
	if(cc<0 || cc>=curve->control_count) return -1;
		
	curve->controls[cc].x = curve->controls[cc].x + dx;
	curve->controls[cc].y = curve->controls[cc].y + dy;
	return 0;
}

int move_curve (Curve_infos *ci, double dx, double dy){
		int cc = ci->current_curve;
		if ( cc < 0 ||cc > ci->curve_list.curve_count-1)
			return -1;
		
		int i;

		for (i = 0; i < ci->curve_list.curves[cc].control_count; i++){
			ci->current_control = i;
			ci->curve_list.curves[cc].controls[i].x = ci->curve_list.curves[cc].controls[i].x + dx;
			ci->curve_list.curves[cc].controls[i].y = ci->curve_list.curves[cc].controls[i].y + dy;
		}
		return 0;

}


int remove_control(Curve_infos *ci){
		int n = ci->current_curve;
		int k = ci->current_control;
		if (n<0 || n>ci->curve_list.curve_count-1)
			return -1;
		
		if (k<0 || k>ci->curve_list.curves[n].control_count-1)
			return -1;
		
		memmove (ci->curve_list.curves[n].controls+k, 
				ci->curve_list.curves[n].controls+k+1, // dest, source
				sizeof(Control)*(ci->curve_list.curves[n].control_count-1-k)); //taille

		ci->curve_list.curves[n].control_count--;
		ci->current_control = -1;

		if (ci->curve_list.curves[n].control_count==0)
			remove_curve(ci);
		
		return 0;
}


void convert_bsp3_to_bezier (double p[4], double b[4]){
	b[0] = (p[0] + 4*p[1] + p[2])/6.0;
	b[1] = (	   4*p[1] + 2*p[2])/6.0;
	b[2] = (	   2*p[1] + 4*p[2])/6.0;
	b[3] = (	   p[1] + 4*p[2] + p[3])/6.0;
}


void convert_bsp3_to_bezier_prolong_first (double p[3], double b[4]){
	b[0] = p[0];
	b[1] = (2*p[0]+p[1])/3.0;
	b[2] = (p[0] + 2*p[1])/3.0;
	b[3] = (p[0]+4*p[1]+p[2])/6.0;
}


void convert_bsp3_to_bezier_prolong_last (double p[3],double b[4]){
	b[0] = (p[0] + 4*p[1] + p[2])/6.0;
	b[1] = (2*p[1]+p[2])/3.0;
	b[2] = (p[1] + 2*p[2])/3.0;
	b[3] = p[2];
}


void compute_bezier_points_open (Curve *curve, int i, Control bez_points[4]){
	int j;
	double px[4], py[4], bx[4], by[4];

	int n = i+4;

	for(j=0;i<n;i++, j++){
		px[j]=curve->controls[i].x;
		py[j]=curve->controls[i].y;
	}

	convert_bsp3_to_bezier (px, bx);
	convert_bsp3_to_bezier (py, by);

	for (i=0; i<4;i++){
		bez_points[i].x = bx[i];
		bez_points[i].y = by[i];
	}


}


void compute_bezier_points_close (Curve *curve, int i, Control bez_points[4]){
	int j;
	double px[4], py[4], bx[4], by[4];

	int n = 4;

	for(j=0;j<n;i++, j++){
		if (i>= (curve->control_count)-3)
			i = i%(curve->control_count);
		
		px[j]=curve->controls[i].x;
		py[j]=curve->controls[i].y;

	}

	convert_bsp3_to_bezier (px, bx);
	convert_bsp3_to_bezier (py, by);

	for (i=0; i<4;i++){
		bez_points[i].x = bx[i];
		bez_points[i].y = by[i];
	}
}

double compute_bezier_cubic (double b[4],double t){
	double f;
	f = pow(1-t,3) * b[0] 
		+ 3*pow(1-t,2)*t*b[1]
		+ 3*(1-t)*pow(t,2)*b[2]
		+ pow(t,3)*b[3];

	return f; 
}


void compute_bezier_points_prolong_first (Curve *curve, Control bez_points[4]){
	int j;
	double px[3], py[3], bx[4], by[4];
	int i = 0;
	int n = i+3;

	for(j=0;i<n;i++, j++){
		px[j]=curve->controls[i].x;
		py[j]=curve->controls[i].y;
	}

	convert_bsp3_to_bezier_prolong_first (px, bx);
	convert_bsp3_to_bezier_prolong_first (py, by);

	for (i=0; i<4;i++){
		bez_points[i].x = bx[i];
		bez_points[i].y = by[i];
	}

}

void compute_bezier_points_prolong_last (Curve *curve, Control bez_points[4]){
	int j;
	double px[3], py[3], bx[4], by[4];
	
	int n = curve->control_count;

	int i = curve->control_count-3;

	for(j=0;i<n;i++, j++){
		px[j]=curve->controls[i].x;
		py[j]=curve->controls[i].y;
	}

	convert_bsp3_to_bezier_prolong_last (px, bx);
	convert_bsp3_to_bezier_prolong_last (py, by);

	for (i=0; i<4;i++){
		bez_points[i].x = bx[i];
		bez_points[i].y = by[i];
	}

}


int move_shift (Curve_infos *ci, double dx, double dy){
	if (ci->current_curve < 0 || ci->current_curve >= ci->curve_list.curve_count)
		return FALSE;
	
	ci->curve_list.curves[ci->current_curve].shift_x += dx;
	ci->curve_list.curves[ci->current_curve].shift_y += dy;
	return TRUE;
}

int reset_shift (Curve_infos *ci){
	if (ci->current_curve < 0 || ci->current_curve >= ci->curve_list.curve_count)
		return FALSE;
	ci->curve_list.curves[ci->current_curve].shift_x = 0.0;
	ci->curve_list.curves[ci->current_curve].shift_y = 0.0;
	return TRUE;
}



/* Échantillonnage et stockage des courbes */

void store_sample (double x, double y,
  double sx[], double sy[], int *ind, int ind_max){
	if (*ind >= ind_max) {
	    fprintf (stderr, "%s: capacity exceeded \n", __func__);
	    return;
	}
	sx[*ind] = x;
	sy[*ind] = y;
	*ind += 1;
}


/* Échantillonne une courbe de Bézier avec le pas théta.
 * Stocke les points dans sx[0..ind_max-1], sy[0..ind_max-1] à partir de ind. 
 * Au retour, ind est le nouveau point d'insertion.
 * Fct inspirée de drawings.c:generate_bezier_path() du TP6 (supprimée ici)
*/
void sample_bezier_curve (Control bez_points[4], double theta,
  double sx[], double sy[], int *ind, int ind_max, int is_first)
{
  double x, y, bx[4], by[4], t;

  for (int j = 0; j <= 3 ; j++) {
      bx[j] = bez_points[j].x;
      by[j] = bez_points[j].y;
  }

  for (t = is_first ? 0.0 : theta; t < 1.0; t += theta) {
    x = compute_bezier_cubic (bx, t);
    y = compute_bezier_cubic (by, t);
    store_sample (x, y, sx, sy, ind, ind_max);
  }

  if (t < 1.0) {
    x = compute_bezier_cubic (bx, 1.0);
    y = compute_bezier_cubic (by, 1.0);
    store_sample (x, y, sx, sy, ind, ind_max);
  }
}


/* Calcule les coordonnées (x,y) du point de coordonnées t à partir
 * de l'échantillonnage sx[],sy[]. t est un réel dans [0..tmax-1].
 * Renvoie TRUE si t est bien dans l'intervalle ;
 * sinon renvoie FALSE et met dans (x,y) le premier ou dernier point.
 * RQ actuellement on ne se sert pas du retour.
*/
int interpolate_samples (double sx[], double sy[], double t, int tmax,
  double *x, double *y)
{
  if (t <= 0) {
    *x = sx[0];
    *y = sy[0];
    return t == 0;
  } 
  if (t >= tmax-1) {
    *x = sx[tmax-1];
    *y = sy[tmax-1];
    return t == tmax-1;
  } 
  int i = t;
  double u = t-i;
  *x = sx[i]*(1-u) + sx[i+1]*u;
  *y = sy[i]*(1-u) + sy[i+1]*u;
  return 1;
}


/* Étant donné le point A de paramètre tA,
 * Calcule les coordonnées du point B(xB,yB) de paramètre tB,
 * situé à la distance |dist| de A, après ou avant.
 * Les tableaux sx[],sy[] ont leur indice dans [0..tmax-1].
 * Renvoie tB, ou -1 si tB est hors tableau.
*/

double compute_distant_point_forward (double sx[], double sy[], double tA, int tmax,
  double dist, double *xB, double *yB)
{
	  double xA, yA;
	  interpolate_samples (sx, sy, tA, tmax, &xA, &yA);

	  if (dist == 0) {
	    *xB = xA; *yB = yA;
	    return (tA >= 0 && tA <= tmax-1) ? tA : -1;
	  }

	  double t, m0, m1, dx, dy, d2 = 0, dist2 = dist*dist, epsilon = 0.1;

	  // Recherche un point qui dépasse |dist|.
	  // On fait une itération de plus (t <= tmax-1 +1) pour atteindre tmax-1
	  // en effet si t > tmax-1 alors interpolate_samples prend t = tmax-1
	  for (t = tA+1.0; t <= tmax-1 +1; t += 1.0) { 
	    interpolate_samples (sx, sy, t, tmax, xB, yB);
	    dx = *xB - xA; dy = *yB - yA; d2 = dx*dx + dy*dy;
	    if (d2 > dist2) break;
	  }
	  if (d2 < dist2) return -1;
	  m0 = t-1.0; m1 = t;

	  // Recherche dichotomique jusqu'à la précision epsilon
	  while (m1-m0 > 0) {
	    t = (m0+m1)/2.0;
	    interpolate_samples (sx, sy, t, tmax, xB, yB);
	    dx = *xB - xA; dy = *yB - yA; d2 = dx*dx + dy*dy;
	    if (d2 < dist2-epsilon) m0 = t;
	    else if (d2 > dist2+epsilon) m1 = t; 
	    else break; // Précision suffisante
	  }
	  return t;
}


double compute_distant_point_backward (double sx[], double sy[], double tA, int tmax,
  double dist, double *xB, double *yB)
{
	  double xA, yA;
	  interpolate_samples (sx, sy, tA, tmax, &xA, &yA);

	  if (dist == 0) {
	    *xB = xA; *yB = yA;
	    return (tA >= 0 && tA <= tmax-1) ? tA : -1;
	  }

	  double t, m0, m1, dx, dy, d2 = 0, dist2 = dist*dist, epsilon = 0.1;

	  // Recherche un point qui dépasse |dist|.
	  // On fait une itération de plus (t >= 0 -1) pour atteindre 0
	  // en effet si t < 0 alors interpolate_samples prend t = 0
	  for (t = tA-1.0; t >= 0 -1; t -= 1.0) {
	    interpolate_samples (sx, sy, t, tmax, xB, yB);
	    dx = *xB - xA; dy = *yB - yA; d2 = dx*dx + dy*dy;
	    if (d2 > dist2) break;
	  }
	  if (d2 < dist2) return -1;
	  m0 = t; m1 = t+1.0;

	  // Recherche dichotomique jusqu'à la précision epsilon
	  while (m1-m0 > 0) {
	    t = (m0+m1)/2.0;
	    interpolate_samples (sx, sy, t, tmax, xB, yB);
	    dx = *xB - xA; dy = *yB - yA; d2 = dx*dx + dy*dy;
	    if (d2 < dist2-epsilon) m1 = t;
	    else if (d2 > dist2+epsilon) m0 = t; 
	    else break; // Précision suffisante
	  }
	  return t;
}
