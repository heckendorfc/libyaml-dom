#ifndef COMMON_H
#define COMMON_H

#define NUM_INTS 5
struct serializabledata{
	yamldom_data_t ydd;
	yamldom_node_t *root;
	yamldom_anchor_list_t *anchors;
	int myint;
	int manyints[NUM_INTS];
	int num_ints;
};

char* inttostr(char *buf, int x);
char* dbltostr(char *buf,double x);
yamldom_node_t* intseq(int *arr, int num, char *buffer);

#endif
