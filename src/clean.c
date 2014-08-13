/* Copyright 2014, Heckendorf */

#include "yamldom.h"

void freestr(char *str){
	if(str)
		free(str);
}

void yamldom_free_nodes(yamldom_node_t *root){
	yamldom_node_t *tmp;
	char *str;

	while(root){
		switch(root->type){
			case Y_SEQ:
				yamldom_free_nodes(YAMLDOM_SEQ_NODES(root));
				break;
			case Y_MAP:
				yamldom_free_nodes(YAMLDOM_MAP_NODES(root));
				break;
			case Y_SCALAR:
				freestr(YAMLDOM_SCALAR_DATA(root)->val);
				freestr(YAMLDOM_SCALAR_DATA(root)->tag);
				break;
			case Y_ALIAS:
				freestr(YAMLDOM_ALIAS_DATA(root)->val);
				break;
		}

		tmp=root->next;

		if(root->anchor)
			free(root->anchor);

		free(root);

		root=tmp;
	}
}

void yamldom_free_anchors(yamldom_anchor_list_t *root){
	yamldom_anchor_list_t *tmp;

	while(root){
		freestr(root->val);
		tmp=root->next;
		free(root);
		root=tmp;
	}
}
