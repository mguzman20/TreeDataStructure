#include "node.h"

Node* node_create(int value) {
	Node* node = (Node*)calloc(1, sizeof(Node));

	node->value = value;
	return node;
}

void node_insert(Node* root, Node* node) {
	Node* actual_node = root;
	bool is_inserted = false;

	while (!is_inserted) {
		if (actual_node->value > node->value) {
			if (actual_node->left_child) {
				actual_node = actual_node->left_child;
			}
			else {
				actual_node->left_child = node;
				is_inserted = true;
			}
		}
		else {
			if (actual_node->right_child) {
				actual_node = actual_node->right_child;
			}
			else {
				actual_node->right_child = node;
				is_inserted = true;
			}
		}
	}
}

void print_tree(Node* root) {
	printf("%d ", root->value);
	if (root->left_child) {
		print_tree(root->left_child);
	}
	if (root->right_child) {
		print_tree(root->right_child);
	}
}

void free_tree(Node* node){
	if (node != NULL) {
		free_tree(node->left_child);
		free_tree(node->right_child);
		free(node);
	}
}

void print_path(FILE* file, Node* root, int value){
	bool is_find = false;
	Node* actual_node = root;
	while (!is_find) {
		if (actual_node->value > value) {
			if (actual_node->left_child) {
				fprintf(file, "%i ", actual_node->value);
				actual_node = actual_node->left_child;
			}
			else {
				fprintf(file, "%i X\n", actual_node->value);
				is_find = true;
			}
		}
		else if (actual_node->value < value) {
			if (actual_node->right_child) {
				fprintf(file, "%i ", actual_node->value);
				actual_node = actual_node->right_child;
			}
			else {
				fprintf(file, "%i X\n", actual_node->value);
				is_find = true;
			}
		}
		else if (actual_node->value == value) {
			fprintf(file, "%i\n", actual_node->value);
			is_find = true;
		}
	}
}

int find_deep(Node* root, int value){
	bool is_find = false;
	Node* actual_node = root;
	int deep = 0;
	while (!is_find) {
		if (actual_node->value > value) {
			actual_node = actual_node->left_child;
			deep += 1;
		}
		else if (actual_node->value < value) {
			actual_node = actual_node->right_child;
			deep += 1;
		}
		else if (actual_node->value == value) {
			is_find = true;
		}
	}
	return deep;
}

void print_in_order(FILE* file, Node* node){
	if (node != NULL) {
		print_in_order(file, node->left_child);
		fprintf(file, "%i ", node->value);
		print_in_order(file, node->right_child);
	}
}

static bool are_identical(Node* node1, Node* node2){
	if (node1 == NULL && node2 == NULL){return true;}
	if (node1 == NULL || node2 == NULL){return false;}

	return (node1->value == node2->value && 
	are_identical(node1->left_child, node2->left_child) && 
	are_identical(node1->right_child, node2->right_child));
}

bool is_subtree(Node* node1, Node* node2){
	if (node1 == NULL){return false;}
	if (node2 == NULL){return true;}

	if (are_identical(node1, node2)){return true;}

	return is_subtree(node1->left_child, node2) || is_subtree(node1->right_child, node2);
}