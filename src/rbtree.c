#include "rbtree.h"
#include <stdlib.h>
void left_rotate(rbtree *t, node_t *x){
  node_t *y = x -> right;
  x -> right = y -> left;
  if(y -> left != t -> nil){
    y -> left -> parent = x;
  }
  y -> parent = x -> parent;
  if(x -> parent == t -> nil){
    t -> root = y;
  }
  else if(x == x -> parent -> left){
    x -> parent -> left = y;
  }
  else{
    x -> parent -> right = y;
  }
  y -> left = x;
  x -> parent = y;
}
void right_rotate(rbtree *t, node_t *x){
  node_t *y = x -> left;
  x -> left = y -> right;
  if(y -> right != t -> nil){
    y -> right -> parent = x;
  }
  y -> parent = x -> parent;
  if(x -> parent == t -> nil){
    t -> root = y;
  }
  else if(x == x -> parent -> left){
    x -> parent -> left = y;
  }
  else{
    x -> parent -> right = y;
  }
  y -> right = x;
  x -> parent = y;
}
rbtree *new_rbtree(void) {
  rbtree *p = (rbtree *)calloc(1, sizeof(rbtree));
  p->nil = (node_t *)malloc(sizeof(node_t));
  p->nil -> color = RBTREE_BLACK;
  p->nil->left = p -> nil;
  p->nil->right = p -> nil;
  p->nil->parent = p -> nil;
  p -> root = p -> nil;
  return p;
}
void delete_nodes(rbtree *t, node_t *n){
  if(n == t-> nil) return;
  delete_nodes(t, n->left);
  delete_nodes(t, n->right);
  free(n);
}
void delete_rbtree(rbtree *t) {
  delete_nodes(t, t->root);
  free(t->nil);
  free(t);
}
void insert_fixup(rbtree *t, node_t *z){
  while(z != t->root && z -> parent -> color == RBTREE_RED){
    if(z -> parent == z -> parent -> parent -> left){
      node_t *w = z -> parent -> parent -> right;
      if (w -> color == RBTREE_RED){
        z -> parent -> color = RBTREE_BLACK;
        w -> color = RBTREE_BLACK;
        z -> parent -> parent -> color = RBTREE_RED;
        z = w -> parent;
      }
      else{
        if(z == z -> parent -> right){
          z = z -> parent;
          left_rotate(t, z);
        }
        z -> parent  -> color = RBTREE_BLACK;
        z -> parent -> parent  -> color = RBTREE_RED;
        right_rotate(t,z -> parent -> parent);
      }
    }
    // 반대 경우
    else{
      node_t *w = z -> parent -> parent -> left;
      if (w -> color == RBTREE_RED){
        z -> parent -> color = RBTREE_BLACK;
        w -> color = RBTREE_BLACK;
        z -> parent -> parent -> color = RBTREE_RED;
        z = w -> parent;
      }
      else{
        if(z == z -> parent -> left){
          z = z -> parent;
          right_rotate(t, z);
        }
        z -> parent -> color = RBTREE_BLACK;
        z -> parent -> parent  -> color = RBTREE_RED;
        left_rotate(t,z -> parent -> parent);
      }
    }
  }
  t-> root -> color = RBTREE_BLACK;
}
node_t *rbtree_insert(rbtree *t, const key_t key) {
  node_t *z = (node_t *)malloc(sizeof(node_t));
  z -> color = RBTREE_RED;
  z -> key = key;
  z -> left = t -> nil;
  z -> right = t -> nil;
  z -> parent = t -> nil;
  node_t *x = t -> root;
  node_t *y = t -> nil;
  if(x == t -> nil){
    z -> color = RBTREE_BLACK;
    t->root = z;
    return t-> root;
  }
  while(x != t -> nil){
    y = x;
    if(z -> key < x -> key)
      x = x -> left;
    else
      x = x -> right;
  }
  z -> parent = y;
  if(y == t -> nil)
    t -> root = z;
  else if(z -> key < y -> key){
    y -> left = z;
  }
  else{
    y -> right = z;
  }
  z -> left = t -> nil;
  z -> right = t -> nil;
  insert_fixup(t, z);
  return t->root;
}
node_t *rbtree_find(const rbtree *t, const key_t key) {
  node_t *x = t-> root;
  if(t -> root == t -> nil){
    return NULL;
  }
  while(x != t -> nil ){
    if(key < x-> key )
      x = x-> left;
    else if(key > x -> key)
      x = x -> right;
    else
      return x;
  }
  return NULL;
}
node_t *rbtree_min(const rbtree *t) {
  node_t *minNode = t -> root;
  while(minNode -> left != t -> nil){
    minNode = minNode -> left;
  }
  return minNode;
}
node_t *rbtree_max(const rbtree *t) {
  node_t *maxNode = t -> root;
  while(maxNode -> right != t -> nil){
    maxNode = maxNode -> right;
  }
  return maxNode;
}
void transplant(rbtree *t, node_t *u, node_t *v){
  if(u -> parent == t -> nil)
    t-> root = v;
  else if(u == u -> parent -> left){
    u -> parent -> left = v;
  }
  else
    u -> parent -> right = v;
  v -> parent = u -> parent;
}
node_t *tree_minimum(rbtree *t, node_t *z){
  while(z -> left != t -> nil){
    z = z -> left;
  }
  return z;
}

void rbtree_erase_fixup(rbtree *t, node_t *p){
  while(p != t -> root && p -> color == RBTREE_BLACK){
    if(p == p -> parent -> left){
      node_t *w = p -> parent -> right;
      if(w -> color == RBTREE_RED){
        w -> color = RBTREE_BLACK;
        p -> parent -> color = RBTREE_RED;
        left_rotate(t, p -> parent);
        w = p -> parent -> right;
      }
      if(w -> left -> color == RBTREE_BLACK && w -> right -> color == RBTREE_BLACK){
        w -> color = RBTREE_RED;
        p = p -> parent;
      }
      else{
        if(w -> right -> color == RBTREE_BLACK){
          w -> color = RBTREE_RED;
          w -> left -> color = RBTREE_BLACK;
          right_rotate(t, w);
          w = p->parent->right;
        }
          w -> color = p->parent->color;
          w -> right -> color = RBTREE_BLACK;
          p -> parent -> color = RBTREE_BLACK;
          left_rotate(t, p -> parent);
          p = t -> root;
      }
    }
    else{
      node_t *w = p -> parent -> left;
      if(w -> color == RBTREE_RED){
        w -> color = RBTREE_BLACK;
        w -> parent -> color = RBTREE_RED;
        right_rotate(t, p -> parent);
        w = p -> parent -> left;
      }
      if(w -> left -> color == RBTREE_BLACK && w -> right -> color == RBTREE_BLACK){
        w -> color = RBTREE_RED;
        p = p -> parent;
      }
      else {
        if(w -> left -> color == RBTREE_BLACK){
        w -> right -> color = RBTREE_BLACK;
        w -> color = RBTREE_RED;
        left_rotate(t, w);
        w = p->parent->left;
      }
        w -> color = p->parent->color;
        w -> left -> color = RBTREE_BLACK;
        p -> parent -> color = RBTREE_BLACK;
        right_rotate(t, w -> parent);
        p = t -> root;
      }
    }
  }
  p -> color = RBTREE_BLACK;
}

int rbtree_erase(rbtree *t, node_t *p) {
  node_t *y = p;
  color_t yColor = y -> color;
  node_t *x;
  if(p -> left == t -> nil){
    x = p -> right;
    transplant(t, p, p -> right);
  }
  else if(p -> right == t -> nil){
    x = p -> left;
    transplant(t, p, p -> left);
  }
  else{
    y = tree_minimum(t, p -> right);
    yColor = y -> color;
    x = y -> right;
    if(y != p -> right){
      transplant(t, y, y-> right);
      y -> right = p -> right;
      y -> right -> parent = y;
    }
    else x -> parent = y;
    transplant(t, p, y);
    y -> left = p -> left;
    y -> left -> parent = y;
    y -> color = p -> color;
  }
  if(yColor == RBTREE_BLACK){
    rbtree_erase_fixup(t, x);
  }
  free(p);
  return 0;
}
void inorder(const rbtree *t, node_t *node, key_t *arr, size_t n, int *i){
  if(node == t -> nil || *i >= n)
    return;
  // printf("현재 노드: %d\n", node->key);
  if(node -> left != t -> nil)
    inorder(t,node -> left, arr, n, i);
  arr[*i] = node -> key;
  (*i)++;
  if(node -> right != t -> nil)
    inorder(t,node -> right, arr, n, i);
}
int rbtree_to_array(const rbtree *t, key_t *arr, const size_t n) {
  int idx = 0;
  inorder(t, t-> root, arr, n, &idx);
  return 0;
}







