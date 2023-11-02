#include <assert.h>
#include <rbtree.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

void test_init(void){
	rbtree *t = new_rbtree();
	delete_rbtree(t);
	printf("init pass!\n");
}

int main(void){
	test_init();
}
