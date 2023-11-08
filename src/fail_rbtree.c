#include "rbtree.h"
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
//------------------------------
#include <time.h>
#include <string.h>
time_t t;
struct tm tm;
void log_d(char *message){
	FILE *log_file = fopen("log.txt", "a");
	time_t uctTime;
	struct tm *loctime;
	(void)time(&uctTime);
	loctime = localtime(&uctTime);

	char logMessage[1024];
	memset(logMessage, 0x00, sizeof(logMessage));
	strftime(logMessage, 255, "[%F %H:%M:%S]: ", loctime);

	if(log_file !=NULL){
		fprintf(log_file, logMessage);
		fprintf(log_file, "%s\n", message);
		fflush(log_file);
		fclose(log_file);
		log_file=NULL;
	}
}

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
bool is_red_parent(node_t *insert_node){
	if(!insert_node){
		return false;
	}

	//insert_node is root if not exist parent node
	if(insert_node->parent){
		node_t *p_node = insert_node->parent;

		if((p_node->color == RBTREE_RED) && (insert_node->color == RBTREE_RED)){
			return true;
		}
	}
	return false;
}

void postorder(rbtree *t, node_t *node){
	if(node != t->nil){
		postorder(t, node->left);
		postorder(t, node->right);
		free(node);
	}
}

void right_rotate(rbtree *t, node_t *node){	
	node_t *p_node   = node->parent; //parent node
	if(t->root == p_node){
		t->root = node;
	}
	node_t *p_p_node = p_node->parent; //grand parent

	p_node->left     = node->right; //node's right child node -> parent node's left node
	node->right->parent = p_node;
	node->right			 = p_node;
	
	node->parent     = p_p_node; //parent node's parent node -> node's parent node
	p_node->parent   = node; //Now, parent node's parent is node
	
	if(p_p_node->key < p_node->key){ //parent node was grand parent's right node
		p_p_node->right = node;
	} else { //parent node was grand parent's left node
		p_p_node->left = node;
	}
}

void left_rotate(rbtree *t, node_t *node){	
	node_t *p_node   = node->parent; //parent node
	if(t->root == p_node){
		t->root = node;
	}
	node_t *p_p_node = p_node->parent; //grand parent

	p_node->right     = node->left; //node's right child node -> parent node's left node
	node->left->parent = p_node;
	node->left			 = p_node;

	node->parent     = p_p_node; //parent node's parent node -> node's parent node
	p_node->parent   = node; //Now, parent node's parent is node

	if(p_p_node->key < p_node->key){ //parent node was grand parent's right node
		p_p_node->right = node;
	} else { //parent node was grand parent's left node
		p_p_node->left = node;
	}
}

node_t *find_uncle(rbtree *t, node_t *now_node){
	if(now_node->parent->parent == t->nil){
		return t->nil;
	}

	if(now_node->parent->key > now_node->parent->parent->key){
		return now_node->parent->parent->left;
	}

	return now_node->parent->parent->right;
}

void recolor(rbtree *t, node_t *node){
	if(node == t->root){
		node->color = RBTREE_BLACK;
		return ;
	}

	if(node->color != node->parent->color){
		return;
	}

	node_t *uncle_node  = find_uncle(t, node);
	node->parent->color = RBTREE_BLACK;

	if(uncle_node != t->nil){
		uncle_node->color   = RBTREE_BLACK;
	}

	if(node->parent->parent != t->nil){
		node->parent->parent->color = RBTREE_RED;
	}

}

void rotate_case(rbtree *t, node_t *new_node){
	if(new_node->parent->key > new_node->parent->parent->key){
		//new_node's parent is grand parent's right node
		if(new_node->key > new_node->parent->key){
			//new_node is parent_node's right node, like \ shape
			new_node->parent->color          = RBTREE_BLACK;
			new_node->parent->parent->color  = RBTREE_RED;

			left_rotate(t, new_node->parent);
		} else {
			//new_node is parent_node's left node, like > shape
			right_rotate(t, new_node);

			new_node->color          = RBTREE_BLACK;
			new_node->parent->color  = RBTREE_RED;

			left_rotate(t, new_node);	
		}
	} else {
		//new_node's parent is grand parent's left node
		if(new_node->key > new_node->parent->key){
			//new_node is parent_node's right node, like < shape
			left_rotate(t, new_node);

			new_node->color          = RBTREE_BLACK;
			new_node->parent->color  = RBTREE_RED;

			right_rotate(t, new_node);
		} else {
			//new_node is parent_node's left node, like / shape
			new_node->parent->color  				 = RBTREE_BLACK;
			new_node->parent->parent->color  = RBTREE_RED;

			right_rotate(t, new_node->parent);
		}
	}
}

void detect_red(rbtree *t, node_t *new_node){
	//XXX: Balance
	if(new_node == t->root){
		return;
	}
	if(new_node->parent->parent == t->nil){
		return;
	}	
	if((new_node->parent->color == RBTREE_RED)&&(new_node->color == RBTREE_RED)){
		node_t *uncle_node = find_uncle(t, new_node);
		if(uncle_node->color == RBTREE_BLACK){ //Restructuring

			rotate_case(t, new_node);
		} else { //Recoloring
			recolor(t, new_node);	
		}
		if(t->root->color == RBTREE_RED){
			t->root->color = RBTREE_BLACK;
		}

	}
	detect_red(t, new_node->parent);
}

void double_black_case(rbtree *t, node_t *double_black_node, node_t *parent){
	printf("#case : double parent : %d --------------\n",double_black_node->parent->key);

	if(double_black_node == t->root){
		return;
	}
	//double black node's parent has left double black node - default
	bool is_right = false;
	if(parent->right == double_black_node){
		is_right = true;
	}
	
	node_t *double_black_node_sibling = double_black_node->parent->right;

	if (is_right){
		double_black_node_sibling = double_black_node->parent->left;
	}

	if(double_black_node_sibling->color == RBTREE_RED){ //double black has red sibling node
		printf("#case RED : double_black_node_sibling = %d\n", double_black_node_sibling->key);
		double_black_node_sibling->color = RBTREE_BLACK;
		double_black_node->parent->color = RBTREE_RED;

		right_rotate(t, double_black_node_sibling);
		double_black_case(t, double_black_node, double_black_node->parent);
	} else {
		if(double_black_node_sibling->left->color == RBTREE_BLACK 
				&& double_black_node_sibling->right->color == RBTREE_BLACK){ //sibling black, left, right nephew also black
			printf("#case B_B_B : double_black_node_sibling = %d\n", double_black_node_sibling->key);
			
			double_black_node_sibling->color = RBTREE_RED;
			if(double_black_node->parent->color == RBTREE_RED){
				double_black_node->parent->color = RBTREE_BLACK;
				return ;
			}
			printf("---------------input : %d\n", double_black_node->parent->parent->key);
			double_black_case(t, double_black_node->parent, double_black_node->parent->parent); //parent is double black
		} else if(double_black_node_sibling->left->color == RBTREE_RED 
				&& double_black_node_sibling->right->color == RBTREE_BLACK){
			//left nephew is red, right nephew is black
			printf("#case B_R_B : double_black_node_sibling = %d\n", double_black_node_sibling->key);
			double_black_node_sibling->color = RBTREE_RED;
			double_black_node_sibling->left->color = RBTREE_BLACK;

			right_rotate(t, double_black_node_sibling);
		} else if(double_black_node_sibling->right->color == RBTREE_RED){
			printf("#case B_*_R : double_black_node_sibling = %d\n", double_black_node_sibling->key);
			//right nephew is red, right nephew is black or red
			double_black_node_sibling->color = double_black_node->parent->color;
			double_black_node->parent->color = RBTREE_BLACK;
			double_black_node_sibling->right->color = RBTREE_BLACK;

			left_rotate(t, double_black_node_sibling);
		}
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

node_t *rbtree_insert(rbtree *t, const key_t key){
	//create new node
	node_t *new_node = (node_t *)calloc(1, sizeof(node_t));

	new_node->key    = key;
	// new node set left, right -> nil
	new_node->left   = t->nil;
	new_node->right  = t->nil;
	// node set color red
	new_node->color  = RBTREE_RED;

	// set root if t->root is nil
	if(t->root == t->nil){
		t->root          = new_node;
		new_node->color  = RBTREE_BLACK;
		new_node->parent = t->nil;
	}	else {
		// find insertion spot of new node
		node_t *next_node = t->root;
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
				continue;
			}
		}

		new_node->parent = next_node;
		detect_red(t, new_node);
	}

	return new_node;
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
	while(min_node->left){
		min_node = min_node->left;
	}

	return min_node;
}

node_t *rbtree_max(const rbtree *t) {
	// TODO: implement find

	node_t *max_node = t->root;
	while(max_node->right){
		max_node = max_node->right;
	}

	return max_node;
}

int rbtree_erase(rbtree *t, node_t *p) { //return 1 :success, 0 : not exist delete node, -1: error
	// TODO: implement erase
	bool is_root = false;

	node_t *p_node = p->parent;
	bool is_left = p_node->key < p->key? false:true; //is p parent_node's left

	if(t->root == p){// root is always black, t->root change
		is_root = true;
	}

	if(p->color == RBTREE_RED){ //p is RED, heir is BLACK or RED;
		if((p->left == t->nil) && (p->right == t->nil)){ //only target node(p) exist
			if(is_left){ //parent_node's left
				p_node->left = t->nil;
			} else {
				p_node->right = t->nil;
			}
			free(p);
			return 1;
		}

		//p have left and right node;
		node_t *heir_node = p->left;
		while(heir_node->right != t->nil){
			heir_node = heir_node->right;
		}
		
		if(is_left){
			p_node->left = heir_node;
		} else {
			p_node->right = heir_node;
		}

		if(heir_node->color == RBTREE_RED){	//heir node is red

			p->right->parent = heir_node;
			p->left->parent = heir_node;

			heir_node->parent->right = heir_node->left;
			
			heir_node->left = p->left;
			heir_node->right = p->right;

			heir_node->parent = p_node;

			free(p);
			return 1;
		} else {//heir node is black,
				p->right->parent = heir_node;

				heir_node->right = p->right;

				heir_node->parent = p_node;
				heir_node->color = RBTREE_RED;
				heir_node->left->color = RBTREE_BLACK;
				free(p);
			if(heir_node->left != t->nil){
				return 1;
			} else {	//if heir node have not child
				left_rotate(t, heir_node->right);
				detect_red(t, heir_node->right);
			}
		}

	} else { //p is BLACK, heir is RED or BLACK
		if((p->left == t->nil)&&(p->right == t->nil)){
			if(is_left){
				p_node->left = t->nil;
			} else {
				p_node->right = t->nil;
			}
			if(is_root){
				t->root = t->nil;
			}
			free(p);
			return 1;
		}
		
		if((p->left == t->nil)&&(p->right != t->nil)){
			node_t *heir_node = p->right;

			heir_node->parent = p_node;
			if(is_left){
				p_node->left = heir_node;
			} else {
				p_node->right = heir_node;
			}
			heir_node->color = RBTREE_BLACK;
			free(p);
			if(is_root){
				t->root = heir_node;
			}
			return 1;	
		}

		if((p->left != t->nil)&&(p->right == t->nil)){
			node_t *heir_node = p->left;
			
			heir_node->parent = p_node;
			if(is_left){
				p_node->left = heir_node;
			} else {
				p_node->right = heir_node;
			}
			heir_node->color = RBTREE_BLACK;
			free(p);
			if(is_root){
				t->root = heir_node;
				heir_node->parent = t->nil;
			}
			return 1;
		}

		if((p->left != t->nil)&&(p->right != t->nil)){

			node_t *heir_node = p->left;

			while(heir_node->right != t->nil){
				heir_node = heir_node->right;
			}
			
			node_t *double_black_parent_node = heir_node->parent;
			if(double_black_parent_node == p){
				double_black_parent_node = heir_node;
			}

			if(is_left){
				p_node->left = heir_node;
			} else {
				p_node->right = heir_node;
			}

			p->right->parent = heir_node;
			p->left->parent = heir_node;

			heir_node->parent->right = heir_node->left;
			
			heir_node->left = p->left;
			heir_node->right = p->right;

			heir_node->parent = p_node;
			
			if(is_root){
				t->root = heir_node;
				heir_node->parent = t->nil;
			}

			free(p);
			if(heir_node->color == RBTREE_RED){	//heir node is red
				heir_node->color = RBTREE_BLACK;
				return 1;
			} else {//heir node is black, almost double black
				if(heir_node->left->right->color != RBTREE_BLACK){
					heir_node->left->right->color = RBTREE_BLACK;
				} else {
					printf("------DOUBLE BLACK-----------\n");
					node_t *double_black_node = double_black_parent_node->right;
					double_black_node->parent = double_black_parent_node;
					if(double_black_parent_node == heir_node){
						double_black_node = double_black_parent_node->left;
						double_black_node->parent = heir_node->parent;
					}
					double_black_case(t, double_black_node, double_black_node->parent);
				}
			}
		}
	}
	return 0;
}

int rbtree_to_array(const rbtree *t, key_t *arr, const size_t n) {
	// TODO: implement to_array
	return 0;
}
