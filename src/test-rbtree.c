#include <assert.h>
#include <rbtree.h>
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

int main(void){
	//test_init();
	test_insert_single(1024);
	//rbtree *t = new_rbtree();
	//node_t *p = rbtree_insert(t, 1024);

	//assert(p != NULL);
}
