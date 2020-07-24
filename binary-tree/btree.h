#ifndef _BTREE_H
#define _BTREE_H

#include <iostream>

struct node
{
    int value;
    node *left;
    node *right;
};

class btree
{
public:
    btree();
    ~btree();

    void insert(int key);
    node * search(int key);
    void destroy_tree();
    void inorder_print();
    void postorder_print();
    void preorder_print();

private:
    void destroy_tree(node *leaf);
    void insert(int key, node *leaf);
    node * search(int key, node *leaf);
    void inorder_print(node *leaf);
    void postorder_print(node *leaf);
    void preorder_print(node *leaf);

    node *root;

};

#endif
