#include <stdio.h>
#include <yamldom.h>
#include "setup.h"
#include "common.h"

void make_graph(struct serializabledata *data){
	yamldom_node_t *nodes,*tmp;
	char buffer[100];

	data->anchors=NULL;
	data->root=yamldom_make_map(NULL);

	nodes=NULL;

	/* Maps are treated like sequences
	 * The elements are taken in pairs
	 * [key][value][key][value]...
	 */
	tmp=yamldom_make_scalar(NULL,"myint",-1);
	nodes=yamldom_append_node(nodes,tmp);
	tmp=yamldom_make_scalar(NULL,inttostr(buffer,data->myint),-1);
	nodes=yamldom_append_node(nodes,tmp);

	/* Here we add a scalar as the key and a sequence as the value
	 * The sequence will contain a list of scalars
	 */
	tmp=yamldom_make_scalar(NULL,"manyints",-1);
	nodes=yamldom_append_node(nodes,tmp);
	tmp=intseq(data->manyints,data->num_ints,buffer);
	nodes=yamldom_append_node(nodes,tmp);

	YAMLDOM_MAP_NODES(data->root)=nodes;
}

void dump_graph(struct serializabledata *data){
	yamldom_dump(&data->ydd,data->root);
}

void write_data(struct serializabledata *data, FILE *fd){
	data->ydd.infd=NULL;
	data->ydd.outfd=fd;

	io_general_init(&data->ydd);

	make_graph(data);
	dump_graph(data);

	io_general_close(&data->ydd);

	yamldom_free_nodes(data->root);
	yamldom_free_anchors(data->anchors);
}

int main(){
	struct serializabledata mydata;
	int i;

	mydata.myint=123;
	mydata.num_ints=NUM_INTS;
	for(i=0;i<mydata.num_ints;i++)
		mydata.manyints[i]=i;

	write_data(&mydata,stdout);

	return 0;
}
