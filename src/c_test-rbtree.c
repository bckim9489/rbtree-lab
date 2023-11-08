#include <assert.h>
#include "rbtree.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

void test_init(void) {
	rbtree *t = new_rbtree();
	assert(t != NULL);
#ifdef SENTINEL
	assert(t->nil != NULL);
	assert(t->root == t->nil);
#else
	assert(t->root == NULL);
#endif
	delete_rbtree(t);

	printf("pass init!\n");
}

// root node should have proper values and pointers
void test_insert_single(const key_t key) {
	rbtree *t = new_rbtree();
	node_t *p = rbtree_insert(t, key);
	assert(p != NULL);
	assert(t->root == p);
	assert(p->key == key);
	// assert(p->color == RBTREE_BLACK);  // color of root node should be black
#ifdef SENTINEL
	assert(p->left == t->nil);
	assert(p->right == t->nil);
	assert(p->parent == t->nil);
#else
	assert(p->left == NULL);
	assert(p->right == NULL);
	assert(p->parent == NULL);
#endif
	delete_rbtree(t);

	printf("pass insert single!\n");
}
// find should return the node with the key or NULL if no such node exists
void test_find_single(const key_t key, const key_t wrong_key) {
	rbtree *t = new_rbtree();
	node_t *p = rbtree_insert(t, key);

	node_t *q = rbtree_find(t, key);
	assert(q != NULL);
	assert(q->key == key);

	assert(q == p);

	q = rbtree_find(t, wrong_key);
	assert(q == NULL);

	delete_rbtree(t);
	printf("pass insert single!\n");
}

int main(void){
	test_init();
	/*
	test_insert_single(1024);
	test_find_single(512,1024);
	rbtree *t = new_rbtree();
	node_t *a = rbtree_insert(t, 5);
	node_t *b = rbtree_insert(t, 4);
	node_t *c = rbtree_insert(t, 3);
	printf("node 5's left = %d\n", a->left->key);
	//printf("node 4's left = %d\n", b->left->key);
	//printf("node 3's left = %d\n", c->left->key);
	//print_tree(t);
	delete_rbtree(t);
	 */		
}
