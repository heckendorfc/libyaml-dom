#ifndef LIST_H
#define LIST_H

#define NEXT_NODE(node) node->next
#define FINDTAILNODE(x) if(x)while(x->next)x=x->next;
#define ADDNODE(x,t) if(x){t=x; x=malloc(sizeof(*x)); x->next=t;}
#define ADDNODEAFTER(x,t) if(x){t=x->next; x->next=malloc(sizeof(*t)); x->next->next=t;}

#endif
