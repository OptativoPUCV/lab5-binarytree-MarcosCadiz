#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "treemap.h"

typedef struct TreeNode TreeNode;


struct TreeNode {
    Pair* pair;
    TreeNode * left;
    TreeNode * right;
    TreeNode * parent;
};

struct TreeMap {
    TreeNode * root;
    TreeNode * current;
    int (*lower_than) (void* key1, void* key2);
};

int is_equal(TreeMap* tree, void* key1, void* key2){
    if(tree->lower_than(key1,key2)==0 &&  
        tree->lower_than(key2,key1)==0) return 1;
    else return 0;
}


TreeNode * createTreeNode(void* key, void * value) {
    TreeNode * new = (TreeNode *)malloc(sizeof(TreeNode));
    if (new == NULL) return NULL;
    new->pair = (Pair *)malloc(sizeof(Pair));
    new->pair->key = key;
    new->pair->value = value;
    new->parent = new->left = new->right = NULL;
    return new;
}

TreeMap * createTreeMap(int (*lower_than) (void* key1, void* key2)) {
    TreeMap * new = (TreeMap *)malloc(sizeof(TreeMap));
    new->lower_than = lower_than;
    new->root = NULL;
    new->current = NULL;
    return new;
    
}


void insertTreeMap(TreeMap * tree, void* key, void * value) {
    TreeNode * new = createTreeNode(key, value);
    if (tree->root == NULL){
        tree->root = new;
        tree->current = new;
        return;
    }
    TreeNode * aux = tree->root;
    TreeNode * parent = NULL;
    while (aux != NULL){
        parent = aux;
        if (is_equal(tree,aux->pair->key,key)){
            aux->pair->value = value;
            free(new->pair);
            free(new);
            return;
        }
        if (tree->lower_than(aux->pair->key,key)){
            aux = aux->right; 
        }
        else{
            aux = aux->left;
        }
        
    }
    new->parent = parent;
    if (tree->lower_than(parent->pair->key,key)){
        parent->right = new;
    }
    else{
        parent->left = new;
    }
    tree->current = new;
    
}

TreeNode * minimum(TreeNode * x){
    while (x->left != NULL){
        x = x->left;
    }
    return x;
}


void removeNode(TreeMap * tree, TreeNode* node) {
    if (node->left == NULL && node->right == NULL){
        if (node->parent == NULL){
            tree->root = NULL;
        }
        else{
            if (node->parent->left == node){
                node->parent->left = NULL;
            }
            else{
                node->parent->right = NULL;
            }
        }
        free(node->pair);
        free(node);
        return;
    }
    if (node->left == NULL || node->right == NULL){
        TreeNode * child;
        if (node->left != NULL){
            child = node->left;
            
        }
        else{
            child = node->right;
        }
        child->parent = node->parent;
        if (node->parent == NULL){
            tree->root = child;
            
        }
        else{
            if (node->parent->left == node){
                node->parent->left = child;
            }
            else{
                node->parent->right = child;
            }
            
        }
        free(node->pair);
        free(node);
        return;
    }
    TreeNode * succesor = minimum(node->right);{
        node->pair->key = succesor->pair->key;
        node->pair->value = succesor->pair->value;
        removeNode(tree, succesor);
    }
}

void eraseTreeMap(TreeMap * tree, void* key){
    if (tree == NULL || tree->root == NULL) return;

    if (searchTreeMap(tree, key) == NULL) return;
    TreeNode* node = tree->current;
    removeNode(tree, node);

}

Pair * searchTreeMap(TreeMap * tree, void* key) {
    TreeNode * current = tree->root;
    while (current != NULL){
        if (is_equal(tree,current->pair->key,key)){
            tree->current = current;
            return current->pair;
        }
        if (tree->lower_than(key,current->pair->key) == 1){
            current = current->left;
            
        }
        else{
            current = current->right;
        }
    }
    tree->current = NULL;
    return NULL;
}


Pair * upperBound(TreeMap * tree, void* key) {
    return NULL;
}

Pair * firstTreeMap(TreeMap * tree) {
    TreeNode * current = tree->root;
    if (current == NULL) return NULL;
    while (current->left != NULL){
        current = current->left;
    }
    tree->current = current;
    return current->pair;
}

Pair * nextTreeMap(TreeMap * tree) {
    TreeNode * current = tree->current;
    if (current == NULL) return NULL;
    if (current->right != NULL){
        current = current->right;
        while (current->left != NULL){
            current = current->left;
        }
        tree->current = current;
        return current->pair;
    }
    TreeNode * parent = current->parent;
    while (parent != NULL && current == parent->right){
        current = parent;
        parent = parent->parent;
    }
    tree->current = parent;
    if (parent != NULL){
        return parent->pair;
    }
    return NULL;
}
