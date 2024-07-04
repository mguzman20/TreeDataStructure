#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "node.h"

/* Retorna true si ambos strings son iguales */
bool string_equals(char* string1, char* string2) {
	return !strcmp(string1, string2);
}

/* Revisa que los parametros del programa sean válidos */
bool check_arguments(int argc, char** argv) {
	if (argc != 3) {
		printf("Modo de uso: %s INPUT OUTPUT\n", argv[0]);
		printf("Donde:\n");
		printf("\tINPUT es la ruta del archivo de input\n");
		printf("\tOUTPUT es la ruta del archivo de output\n");
		exit(1);
	}

	return true;
}

int main(int argc, char** argv) {
	check_arguments(argc, argv);

	FILE* input_file = fopen(argv[1], "r");
	FILE* output_file = fopen(argv[2], "w");

	int node_count;
	int query_count;

	fscanf(input_file, "%d", &node_count);

	/* leemos Cada nodo */
	int k;

	Node* root;
	Node* node;

	fscanf(input_file, "%d", &k);
	root = node_create(k);
	for (int i = 1; i < node_count; i++) {
		fscanf(input_file, "%d", &k);
		node = node_create(k);
		node_insert(root, node);
		// crear arbol binario
	}

	fscanf(input_file, "%d", &query_count);
	/* leemos las consultas */
	char command[32];
	int value;
	for (int i = 0; i < query_count; i++) {
		fscanf(input_file, "%s %d", command, &value);
		printf("%s %d\n", command, value);
		/* completar la revision de comando y ejecucion de los mismos */
		if (string_equals(command, "PATH")) {
			print_path(output_file, root, value);
		}
		else if (string_equals(command, "DEEP")) {
			int deep = find_deep(root, value);
			fprintf(output_file, "%i\n", deep);
		}
		else if (string_equals(command, "ORDER")) {
			print_in_order(output_file, root);
			fprintf(output_file, "\n");
		}
		else if (string_equals(command, "SUBTREE")) {
			Node* root2;
			fscanf(input_file, "%d", &k);
			root2 = node_create(k);
			for (int i = 1; i < value; i++)
			{
				fscanf(input_file, "%d", &k);
				node_insert(root2, node_create(k));
			}
			fprintf(output_file,"%i\n", is_subtree(root, root2));
			free_tree(root2);
		}
	}

	fclose(input_file);
	fclose(output_file);
	// liberar memoria del arbol
  free_tree(root);
	return 0;
}
