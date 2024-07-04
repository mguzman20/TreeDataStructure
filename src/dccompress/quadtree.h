#pragma once
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>

#include "../imagelib/imagelib.h"

typedef struct Point {
	int x;
	int y;
} Point;
// a quadtree node for image compression
typedef struct Q_node {
	Point sup_izq;
	Point inf_der;
	int size;

	struct Q_node* topleft;
	struct Q_node* topright;
	struct Q_node* bottomleft;
	struct Q_node* bottomright;
} Q_node;

Q_node* create_node(Point sup_izq, Point inf_der);
void create_quadtree(Q_node* node);
Point create_point(int x, int y);
void free_quadtree(Q_node* node);
Color get_pixel(Q_node* node, Image* img);
void mean(Q_node* node, double *u_L, double *u_a, double *u_b,
                 double *sum2_L, double *sum2_a, double *sum2_b,
                 int *n_L, int *n_a, int *n_b, Image* img);
double find_gamma(Q_node* node, double *u_L, double *u_a, double *u_b,
                 double *sum2_L, double *sum2_a, double *sum2_b,
                 int *n_L, int *n_a, int *n_b, Image* img);
void filter(Q_node* node, int alpha, Image* img);
int find_alpha(Q_node* node, int h, Image* img, int alpha, int leaves);
void filter_without_paint(Q_node* node, int alpha, Image* img);
int count_leaves(Q_node* node);
Q_node* copy_quadtree(Q_node* node);


