#include "quadtree.h"

Q_node* create_node(Point sup_izq, Point inf_der) {
	Q_node* node = (Q_node*) malloc(sizeof(Q_node));
	node->sup_izq = sup_izq;
	node->inf_der = inf_der;
    node->size = inf_der.x - sup_izq.x;
	node->topleft = NULL;
	node->topright = NULL;
	node->bottomleft = NULL;
	node->bottomright = NULL;
	return node;
}

Point create_point(int x, int y) {
    Point p;
    p.x = x;
    p.y = y;
    return p;
}

void create_quadtree(Q_node* node){
    if (node->size > 1) {
        Point sup_izq = node->sup_izq;
        Point inf_der = node->inf_der;
        int width = inf_der.x - sup_izq.x;
        int height = inf_der.y - sup_izq.y;
        
        node->topleft = create_node(sup_izq, create_point(sup_izq.x + width / 2, sup_izq.y + height / 2));
        node->topright = create_node(create_point(sup_izq.x + width / 2, sup_izq.y), create_point(inf_der.x, sup_izq.y + height / 2));
        node->bottomleft = create_node(create_point(sup_izq.x, sup_izq.y + height / 2), create_point(sup_izq.x + width / 2, inf_der.y));
        node->bottomright = create_node(create_point(sup_izq.x + width / 2, sup_izq.y + height / 2), inf_der);
        create_quadtree(node->topleft);
        create_quadtree(node->topright);
        create_quadtree(node->bottomleft);
        create_quadtree(node->bottomright);
    }
}

//quadtree free
void free_quadtree(Q_node* node) {
    if (node == NULL) {
        return;
    }
    free_quadtree(node->topleft);
    free_quadtree(node->topright);
    free_quadtree(node->bottomleft);
    free_quadtree(node->bottomright);
    free(node);
}

void mean(Q_node* node, double *u_L, double *u_a, double *u_b,
                 double *sum2_L, double *sum2_a, double *sum2_b,
                 int *n_L, int *n_a, int *n_b, Image* img) {
    if (node->size == 1){
        Color color = get_pixel(node, img);

        *n_L += 1;
        *u_L = *u_L + ((color.L - *u_L)/ *n_L);
        *sum2_L += pow(color.L, 2);

        *n_a += 1;
        *u_a = *u_a + ((color.a - *u_a)/ *n_a);
        *sum2_a += pow(color.a, 2);

        *n_b += 1;
        *u_b = *u_b + ((color.b - *u_b)/ *n_b);
        *sum2_b += pow(color.b, 2);
    }
    else{
        mean(node->topleft, u_L, u_a, u_b, sum2_L, sum2_a, sum2_b, n_L, n_a, n_b, img);
        mean(node->topright, u_L, u_a, u_b, sum2_L, sum2_a, sum2_b, n_L, n_a, n_b, img);
        mean(node->bottomleft, u_L, u_a, u_b, sum2_L, sum2_a, sum2_b, n_L, n_a, n_b, img);
        mean(node->bottomright, u_L, u_a, u_b, sum2_L, sum2_a, sum2_b, n_L, n_a, n_b, img);
    }
    
}

//function that returns a pixel from a Q_node and an image
Color get_pixel(Q_node* node, Image* img) {
    if (node->size == 1) {
        Color p;
        p.L = img->pixels[node->sup_izq.x][node->sup_izq.y].L;
        p.a = img->pixels[node->sup_izq.x][node->sup_izq.y].a;
        p.b = img->pixels[node->sup_izq.x][node->sup_izq.y].b;
        return p;
    }
    
}

double find_gamma(Q_node* node, double *u_L, double *u_a, double *u_b,
                 double *sum2_L, double *sum2_a, double *sum2_b,
                 int *n_L, int *n_a, int *n_b, Image* img) {

                
    mean(node, u_L, u_a, u_b, sum2_L, sum2_a, sum2_b, n_L, n_a, n_b, img);

    double sigma_L = sqrt(abs((*sum2_L / *n_L) - pow(*u_L, 2)));
    double sigma_a = sqrt(abs((*sum2_a / *n_a) - pow(*u_a, 2)));
    double sigma_b = sqrt(abs((*sum2_b / *n_b) - pow(*u_b, 2)));

    double gamma = (sigma_L + sigma_a + sigma_b) / 3;

    return gamma;
    
}

void filter(Q_node* node, int alpha, Image* img){
    if (node != NULL) {
        double u_L = 0;
        double sum2_L = 0;
        int n_L = 0;
        
        double u_a = 0;
        double sum2_a = 0;
        int n_a = 0;

        double u_b = 0;
        double sum2_b = 0;
        int n_b = 0;

        double gamma = find_gamma(node, &u_L, &u_a, &u_b , &sum2_L, &sum2_a, &sum2_b, &n_L, &n_a, &n_b, img);
        if (gamma <= alpha){
            free_quadtree(node->topleft);
            free_quadtree(node->topright);
            free_quadtree(node->bottomleft);
            free_quadtree(node->bottomright);
            node->topleft = NULL;
            node->topright = NULL;
            node->bottomleft = NULL;
            node->bottomright = NULL;
            img_square_paint(img, node->sup_izq.x, node->sup_izq.y, node->size, (Color){u_L, u_a, u_b});
        }
        else {
            filter(node->topleft, alpha, img);
            filter(node->topright, alpha, img);
            filter(node->bottomleft, alpha, img);
            filter(node->bottomright, alpha, img);
        }
    }
}

void filter_without_paint(Q_node* node, int alpha, Image* img){
    if (node!= NULL) {
        double u_L = 0;
        double sum2_L = 0;
        int n_L = 0;
        
        double u_a = 0;
        double sum2_a = 0;
        int n_a = 0;

        double u_b = 0;
        double sum2_b = 0;
        int n_b = 0;

        double gamma = find_gamma(node, &u_L, &u_a, &u_b , &sum2_L, &sum2_a, &sum2_b, &n_L, &n_a, &n_b, img);
        if (gamma <= alpha){
            free_quadtree(node->topleft);
            free_quadtree(node->topright);
            free_quadtree(node->bottomleft);
            free_quadtree(node->bottomright);
            node->topleft = NULL;
            node->topright = NULL;
            node->bottomleft = NULL;
            node->bottomright = NULL;
        }
        else {
            filter_without_paint(node->topleft, alpha, img);
            filter_without_paint(node->topright, alpha, img);
            filter_without_paint(node->bottomleft, alpha, img);
            filter_without_paint(node->bottomright, alpha, img);
        }
    }
}
//funtion that count the leaves in a quadtree
int count_leaves(Q_node* node) {
    if (node == NULL) {
        return 0;
    }
    if (node->topleft == NULL && node->topright == NULL && node->bottomleft == NULL && node->bottomright == NULL) {
        return 1;
    }
    return count_leaves(node->topleft) + count_leaves(node->topright) + count_leaves(node->bottomleft) + count_leaves(node->bottomright);
}

//copy quadtree
Q_node* copy_quadtree(Q_node* node) {
    if (node == NULL) {
        return NULL;
    }
    Q_node* new_node = create_node(node->sup_izq, node->inf_der);
    new_node->topleft = copy_quadtree(node->topleft);
    new_node->topright = copy_quadtree(node->topright);
    new_node->bottomleft = copy_quadtree(node->bottomleft);
    new_node->bottomright = copy_quadtree(node->bottomright);
    return new_node;
}

int find_alpha(Q_node* node, int max_leaves, Image* img, int min, int max){
    Q_node* node_copy = copy_quadtree(node);
    int mid = (min + max) / 2;
    filter_without_paint(node_copy, mid, img);
    int leaves = count_leaves(node_copy);
    free_quadtree(node_copy);
    int leaves_2;
    Q_node* node_copy_2;

    if (leaves >= max_leaves){
        find_alpha(node, max_leaves, img, mid, max);
    }
    else if (leaves < max_leaves){
        node_copy_2 = copy_quadtree(node);
        filter_without_paint(node_copy_2, mid - 1, img);
        leaves_2 = count_leaves(node_copy_2);
        free_quadtree(node_copy_2);
        if (leaves_2 < max_leaves && leaves_2 >= leaves){
            find_alpha(node, max_leaves, img, min, mid);
        }
        else {
            return mid;
        }
    }
    
}