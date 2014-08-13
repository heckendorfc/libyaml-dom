/* Copyright 2014, Heckendorf */

#include <yaml.h>
#include "yamldom.h"

static void io_dump_rec(yamldom_data_t *ydd, yamldom_node_t *root);

static void printerr(yamldom_data_t *ydd){
	switch (ydd->emitter.error) {
		case YAML_MEMORY_ERROR:
			fprintf(stderr,"Memory error: Not enough memory for emitting");
			break;
		case YAML_WRITER_ERROR:
			fprintf(stderr,"Writer error: %s", ydd->emitter.problem);
			break;
		case YAML_EMITTER_ERROR:
			fprintf(stderr,"Emitter error: %s", ydd->emitter.problem);
			break;
		default:
			fprintf(stderr,"Internal error");
	}
}

static void dump_seq(yamldom_data_t *ydd, yamldom_node_t *root){
	yamldom_seq_t *children=root->data;

	yaml_sequence_start_event_initialize(&ydd->event,root->anchor,(unsigned char*)"tag:yaml.org,2002:seq",1,YAML_BLOCK_SEQUENCE_STYLE);
	if (!yaml_emitter_emit(&ydd->emitter, &ydd->event))
	{printerr(ydd);return;}

	io_dump_rec(ydd,children->nodes);

	yaml_sequence_end_event_initialize(&ydd->event);
	if (!yaml_emitter_emit(&ydd->emitter, &ydd->event))
	{printerr(ydd);return;}
}

static void dump_map(yamldom_data_t *ydd, yamldom_node_t *root){
	yamldom_map_t *children=root->data;

	yaml_mapping_start_event_initialize(&ydd->event,root->anchor,(unsigned char*)"tag:yaml.org,2002:map",1,YAML_BLOCK_MAPPING_STYLE);
	if (!yaml_emitter_emit(&ydd->emitter, &ydd->event))
	{printerr(ydd);return;}

	io_dump_rec(ydd,children->nodes);

	yaml_mapping_end_event_initialize(&ydd->event);
	if (!yaml_emitter_emit(&ydd->emitter, &ydd->event))
	{printerr(ydd);return;}
}

static void dump_scalar(yamldom_data_t *ydd, yamldom_node_t *root){
	yamldom_scalar_t *data=root->data;

	yaml_scalar_event_initialize(&ydd->event,root->anchor,data->tag,data->val,-1,1,1,YAML_PLAIN_SCALAR_STYLE);
	if (!yaml_emitter_emit(&ydd->emitter, &ydd->event))
	{printerr(ydd);return;}
}

static void dump_alias(yamldom_data_t *ydd, yamldom_node_t *root){
	yamldom_alias_t *data=root->data;

	yaml_alias_event_initialize(&ydd->event,data->val);
	if (!yaml_emitter_emit(&ydd->emitter, &ydd->event))
	{printerr(ydd);return;}
}

static void io_dump_rec(yamldom_data_t *ydd, yamldom_node_t *root){
	while(root){
		switch(root->type){
			case Y_SEQ:
				dump_seq(ydd,root);
				break;
			case Y_MAP:
				dump_map(ydd,root);
				break;
			case Y_SCALAR:
				dump_scalar(ydd,root);
				break;
			case Y_ALIAS:
				dump_alias(ydd,root);
				break;
		}
		root=root->next;
	}
}


void yamldom_dump(yamldom_data_t *ydd, yamldom_node_t *root){
	yaml_stream_start_event_initialize(&ydd->event, YAML_UTF8_ENCODING);
	if (!yaml_emitter_emit(&ydd->emitter, &ydd->event))
		return;

	if (!yaml_document_start_event_initialize(&ydd->event,NULL,NULL,NULL,0))
		return;
	if (!yaml_emitter_emit(&ydd->emitter, &ydd->event))
		return;

	io_dump_rec(ydd,root);

	yaml_document_end_event_initialize(&ydd->event,1);
	if (!yaml_emitter_emit(&ydd->emitter, &ydd->event)){
		printerr(ydd);
		return;
	}

	yaml_stream_end_event_initialize(&ydd->event);
	if (!yaml_emitter_emit(&ydd->emitter, &ydd->event))
		return;

	fflush(ydd->outfd);
}
