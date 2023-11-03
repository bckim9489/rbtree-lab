#include "rbtree.h"

#include <stdlib.h>
#include <stdio.h>
rbtree *new_rbtree(void) {
	rbtree *p = (rbtree *)calloc(1, sizeof(rbtree));
	// TODO: initialize struct if needed

	node_t *nil = (node_t *)calloc(1, sizeof(node_t));
	nil->color = RBTREE_BLACK;

	p->root = nil;
	p->nil = nil;
	return p;
}

void delete_rbtree(rbtree *t) {
	// TODO: find All node and do free()

	free(t->nil);
	free(t);
}

node_t *rbtree_insert(rbtree *t, const key_t key) {
	//create new node
	node_t *new_node = (node_t *)calloc(1, sizeof(node_t));
	new_node->key   = key;
	// new node set left, right -> nil
	new_node->left  = t->nil;
	new_node->right = t->nil;
	// node set color red
	new_node->color = RBTREE_RED;

	// set root if t->root is null
	if(t->root == t->nil){
		t->root = new_node;
		new_node->color = RBTREE_BLACK
		new_node->parent = t->nil

	}	else {
		// find insertion spot of new node
		node_t *next_node = t->root

		while(true){
			if(key <= next_node->key){
				if(next_node->left == t->nil){
					next_node->left = new_node;
					break;
				}
				next_node = next_node->left;
				continue;
			}
			if(key > next_node->key){
				if (next_node->right == t->nil){
					next_node->right = new_node;
					break;
				}
				next_node = next_node->right;
				continue
			}
		}
		new_node->parent = t->root
	}

	return new_node;
}

node_t *rbtree_find(const rbtree *t, const key_t key) {
	// TODO: implement find
	return t->root;
}

node_t *rbtree_min(const rbtree *t) {
	// TODO: implement find
	return t->root;
}

node_t *rbtree_max(const rbtree *t) {
	// TODO: implement find
	return t->root;
}

int rbtree_erase(rbtree *t, node_t *p) {
	// TODO: implement erase
	return 0;
}

int rbtree_to_array(const rbtree *t, key_t *arr, const size_t n) {
	// TODO: implement to_array
	return 0;
}

void left_lotate(){
	// XXX: case 1, 2, 3, 4
}

void right_lotate(){
	// XXX: case 1, 2, 3, 4
}

void change_color(){
	// XXX: case 1, 2
}

