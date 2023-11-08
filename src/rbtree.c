#include "rbtree.h"
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>

void rb_insert_fixup(rbtree *, node_t *);
void left_rotate(rbtree *, node_t *);
void right_rotate(rbtree *, node_t *);
void rb_transplant(rbtree *, node_t *, node_t *);
void rb_delete_fixup(rbtree *, node_t *);
void postorder(rbtree *, node_t *);
void inorder(const rbtree *, key_t *, const size_t, node_t *, size_t *);

//------------------------------
void print_tree(rbtree *t);
void preorder_print(rbtree *t, node_t *node);

void print_tree(rbtree *t){
	preorder_print(t, t->root);
}

void preorder_print(rbtree *t, node_t *node){
	if(node != t->nil){
		printf("key :%d, parent: %d, left: %d, right: %d, color: %s\n", node->key, node->parent->key, node->left->key, node->right->key, node->color == 0?"RED":"BLACK");
		preorder_print(t, node->left);
		preorder_print(t, node->right);
	}
}
//-------------------------------

void postorder(rbtree *t, node_t *node){
	if(node != t->nil){
		postorder(t, node->left);
		postorder(t, node->right);
		free(node);
	}
}


rbtree *new_rbtree(void) {
	rbtree *p = (rbtree *)calloc(1, sizeof(rbtree));
	// TODO: initialize struct if needed
	node_t *nil = (node_t *)calloc(1, sizeof(node_t));
	nil->color  = RBTREE_BLACK;

	p->root 		= nil;
	p->nil  		= nil;
	return p;
}

void delete_rbtree(rbtree *t) {
	// TODO: find All node and do free()
	if(t->root != t->nil){
		postorder(t, t->root);
	}
	free(t->nil);
	free(t);
}

node_t *rbtree_insert(rbtree *T, const key_t key){
	node_t *z = (node_t *)calloc(1, sizeof(node_t));
	z->key    = key;
	node_t *y = T->nil;
	node_t *x = T->root;
	while (x!= T->nil){
		y = x;
		if(z->key <x->key){
			x = x->left;
		} else {
			x = x->right;
		}
	}
	z->parent = y;
	if (y== T->nil){
		T->root = z;
	} else if(z->key < y->key){
		y->left = z;
	} else {
		y->right = z;
	}
	z->left   = T->nil;
	z->right  = T->nil;
	z->color  = RBTREE_RED;
	rb_insert_fixup(T,z);
	return z;
}

void rb_insert_fixup(rbtree *T, node_t *z){
	while(z->parent->color == RBTREE_RED){
		if(z->parent == z->parent->parent->left){
			node_t *y = z->parent->parent->right;
			if(y->color == RBTREE_RED){
				z->parent->color = RBTREE_BLACK;
				y->color = RBTREE_BLACK;
				z->parent->parent->color = RBTREE_RED;
				z = z->parent->parent;
			} else {
				if(z == z->parent->right){
					z = z->parent;
					left_rotate(T, z);
				}
				z->parent->color = RBTREE_BLACK;
				z->parent->parent->color = RBTREE_RED;
				right_rotate(T, z->parent->parent);
			}
		}else {//right
			node_t *y = z->parent->parent->left;
			if(y->color == RBTREE_RED){
				z->parent->color = RBTREE_BLACK;
				y->color = RBTREE_BLACK;
				z->parent->parent->color = RBTREE_RED;
				z = z->parent->parent;
			} else{ 
				if (z == z->parent->left){
					z = z->parent;
					right_rotate(T, z);
				}
				z->parent->color = RBTREE_BLACK;
				z->parent->parent->color = RBTREE_RED;
				left_rotate(T, z->parent->parent);
			}
		}
	}
	T->root->color = RBTREE_BLACK;
}

void left_rotate(rbtree *T, node_t *x){
	node_t *y = x->right;
	x->right = y->left;
	if(y->left != T->nil){
		y->left->parent = x;
	}
	y->parent = x->parent;
	if(x->parent == T->nil){
		T->root = y;
	} else if(x == x->parent->left){
		x->parent->left = y;
	} else {
		x->parent->right = y;
	}
	y->left = x;
	x->parent = y;
}

void right_rotate(rbtree *T, node_t *x){
	node_t *y = x->left;
	x->left = y->right;
	if(y->right != T->nil){
		y->right->parent = x;
	}
	y->parent = x->parent;
	if(x->parent == T->nil){
		T->root = y;
	} else if(x == x->parent->left){
		x->parent->left = y;
	} else {
		x->parent->right = y;
	}
	y->right = x;
	x->parent = y;
}

node_t *rbtree_find(const rbtree *t, const key_t key) {
	// TODO: implement find
	node_t *found_node = t->root;
	while(found_node->key != key){
		if(found_node == t->nil){
			return NULL;
		}

		if(found_node->key > key){
			found_node = found_node->left;
			continue;
		}

		if(found_node->key < key){
			found_node = found_node->right;
			continue;
		}
	}
	return found_node;
}

node_t *rbtree_min(const rbtree *t) {
	// TODO: implement find
	node_t *min_node = t->root;
	while(min_node->left != t->nil){
		min_node = min_node->left;
	}

	return min_node;
}

node_t *rbtree_max(const rbtree *t) {
	// TODO: implement find

	node_t *max_node = t->root;
	while(max_node->right != t->nil){
		max_node = max_node->right;
	}

	return max_node;
}

void rb_transplant(rbtree *T, node_t *u, node_t *v){
	if(u->parent == T->nil){
		T->root = v;
	} else if(u == u->parent->left){
		u->parent->left = v;
	} else {
		u->parent->right = v;
	}
	v->parent = u->parent;
}

int rbtree_erase(rbtree *T, node_t *z) { //return 1 :success, 0 : not exist delete node, -1: error
	// TODO: implement erase
	node_t *y = z;
	node_t *x;
	color_t y_original_color = y->color;
	if(z->left == T->nil){
		x = z->right;
		rb_transplant(T, z, z->right);
	} else if(z->right == T->nil){
		x = z->left;
		rb_transplant(T, z, z->left);
	} else {
		y = z->right;
		while(y->left != T->nil){ //minimum
			y = y->left;
		}
		y_original_color = y->color;
		x = y->right;
		if(y->parent == z){
			x->parent = y;
		} else {
			rb_transplant(T, y, y->right);
			y->right = z->right;
			y->right->parent = y;
		}
		rb_transplant(T, z, y);
		y->left = z->left;
		y->left->parent = y;
		y->color = z->color;
	}
	if(y_original_color == RBTREE_BLACK){
		rb_delete_fixup(T,x);
	}
	free(z);

	return 1;
}

void rb_delete_fixup(rbtree *T,node_t *x){
	while((x != T->root) && (x->color == RBTREE_BLACK)){
		if(x==(x->parent->left)){
			node_t *w = x->parent->right;

			if(w->color == RBTREE_RED){
				w->color = RBTREE_BLACK;
				x->parent->color = RBTREE_RED;
				left_rotate(T, x->parent);
				w = x->parent->right;
			}

			if((w->left->color == RBTREE_BLACK) && (w->right->color == RBTREE_BLACK)){
				w->color = RBTREE_RED;
				x = x->parent;
			} else {
				if(w->right->color == RBTREE_BLACK){
					w->left->color = RBTREE_BLACK;
					w->color = RBTREE_RED;
					right_rotate(T, w);
					w = x->parent->right;
				}
				w->color = x->parent->color;
				x->parent->color = RBTREE_BLACK;
				w->right->color = RBTREE_BLACK;
				left_rotate(T, x->parent);
				x = T->root;
			}
		} else {
			node_t *w = x->parent->left;
			if(w->color == RBTREE_RED){
				w->color = RBTREE_BLACK;
				x->parent->color = RBTREE_RED;
				right_rotate(T, x->parent);
				x = x->parent->left;
			}
			if((w->right->color == RBTREE_BLACK) && (w->left->color == RBTREE_BLACK)){
				w->color = RBTREE_RED;
				x = x->parent;
			} else { 
					if(w->left->color == RBTREE_BLACK){
					w->right->color = RBTREE_BLACK;
					w->color = RBTREE_RED;
					left_rotate(T, w);
					w = x->parent->left;
				}
				w->color = x->parent->color;
				x->parent->color = RBTREE_BLACK;
				w->left->color = RBTREE_BLACK;
				right_rotate(T, x->parent);
				x = T->root;
			}
		}
	}

	x->color = RBTREE_BLACK;
}

void inorder(const rbtree *t, key_t *arr, const size_t n, node_t *node, size_t *cnt){
	if(node == t->nil){
		return ;
	}

	inorder(t, arr, n, node->left, cnt);
	if(*cnt >= n){ return ;}
	arr[(*cnt)++] = node->key;
	inorder(t, arr, n, node->right, cnt);
}

int rbtree_to_array(const rbtree *t, key_t *arr, const size_t n) {
	// TODO: implement to_array
	if(t->root == t->nil){
		return 0;
	}
	size_t cnt = 0;

	inorder(t, arr, n, t->root, &cnt);

	return 0;
}
