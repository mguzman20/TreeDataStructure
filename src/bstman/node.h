#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

typedef struct node_t {
    int value;
    struct node_t *left_child;
    struct node_t *right_child;
} Node;

Node* node_create(int value); // Sacado de la capsula del enunciado
void node_insert(Node* root, Node* node);
void print_tree(Node* root);
void free_tree(Node* node); // https://stackoverflow.com/questions/9181146/freeing-memory-of-a-binary-tree-c
void print_path(FILE* file, Node* root, int value);
int find_deep(Node* root, int value);
void print_in_order(FILE* file, Node* node); //https://medium.com/javarevisited/how-to-print-nodes-of-a-binary-search-tree-in-sorted-order-8a4e52eb8856
bool is_subtree(Node* node1, Node* node2); //https://www.geeksforgeeks.org/check-if-a-binary-tree-is-subtree-of-another-binary-tree/