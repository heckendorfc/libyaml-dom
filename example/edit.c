#include <stdio.h>
#include <yamldom.h>
#include "setup.h"
#include "common.h"

void edit_graph(struct serializabledata *data){
	yamldom_node_t *nodes,*tmp;
	char buffer[100];

	nodes=NULL;

	// Return the value node for the given map key under root
	tmp=yamldom_find_map_val(data->root,"myint");
	// We are expecting myint to be paired with a scalar (integer as a string)
	if(tmp->type==Y_SCALAR){
		// Replace the value in the YAML file with our new value
		free(YAMLDOM_SCALAR_DATA(tmp)->val);
		YAMLDOM_SCALAR_DATA(tmp)->val=strdup(inttostr(buffer,data->myint));
	}

	// Append a new list of integers to the main map
	tmp=yamldom_make_scalar(NULL,"manymoreints",-1);
	nodes=yamldom_append_node(nodes,tmp);
	tmp=intseq(data->manyints,data->num_ints,buffer);
	nodes=yamldom_append_node(nodes,tmp);

	YAMLDOM_MAP_NODES(data->root)=yamldom_append_node(YAMLDOM_MAP_NODES(data->root),nodes);
}

void dump_graph(struct serializabledata *data){
	yamldom_dump(&data->ydd,data->root);
}

void write_data(struct serializabledata *data, FILE *infd, FILE *outfd){
	data->ydd.infd=infd;
	data->ydd.outfd=outfd;

	io_general_init(&data->ydd);

	data->anchors=NULL;
	data->root=yamldom_gen(&data->ydd,NULL); // Second argument is optional (anchors).

	edit_graph(data);
	dump_graph(data);

	io_general_close(&data->ydd);

	yamldom_free_nodes(data->root);
	yamldom_free_anchors(data->anchors);
}

int main(){
	struct serializabledata mydata;
	int i;

	mydata.myint=456;
	mydata.num_ints=NUM_INTS;
	for(i=0;i<mydata.num_ints;i++)
		mydata.manyints[i]=mydata.num_ints+i;

	write_data(&mydata,stdin,stdout);

	return 0;
}
