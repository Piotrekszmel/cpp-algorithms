#include "btree.h"

btree::btree()
{
    root = nullptr;
}

btree::~btree()
{
    destroy_tree();
}

void btree::destroy_tree(node *leaf)
{
    if (leaf != nullptr)
    {
        destroy_tree(leaf->right);
        destroy_tree(leaf->left);
        delete leaf;
    }
}

void btree::insert(int key, node *leaf)
{
    if (key < leaf->value)
    {
        if (leaf->left != nullptr)
        {
            insert(key, leaf->left);
        }
        else
        {
            leaf->left = new node;
            leaf->left->value = key;
            leaf->left->left = nullptr;
            leaf->left->right = nullptr;
        }
    }
    else if (key >= leaf->value)
    {
        if (leaf->right != nullptr)
        {
            insert(key, leaf->right);
        }
        else
        {
            leaf->right = new node;
            leaf->right->value = key;
            leaf->right->right = nullptr;
            leaf->right->left = nullptr;
        }
    }

}

void btree::insert(int key)
{
    if (root != nullptr)
    {
        insert(key, root);
    }
    else
    {
        root = new node;
        root->value = key;
        root->left = nullptr;
        root->right = nullptr;
    }
}

node * btree::search(int key, node *leaf)
{
    if (leaf != nullptr)
    {
        if (key == leaf->value)
        {
            return leaf;
        }
        if (key < leaf->value)
        {
            return search(key, leaf->left);
        }
        else
        {
            return search(key, leaf->right);
        }
    }
    else
    {
        return nullptr;
    }
}

node * btree::search(int key)
{
    return search(key, root);
}

void btree::destroy_tree()
{
    destroy_tree(root);
}

void btree::inorder_print()
{
    inorder_print(root);
    std::cout << '\n';
}

void btree::inorder_print(node *leaf)
{
    if (leaf != nullptr)
    {
        inorder_print(leaf->left);
        std::cout << leaf->value << ",";
        inorder_print(leaf->right);
    }
}

void btree::postorder_print()
{
    postorder_print(root);
    std::cout << '\n';
}

void btree::postorder_print(node *leaf)
{
    if (leaf != nullptr)
    {
        inorder_print(leaf->left);
        inorder_print(leaf->right);
        std::cout << leaf->value << ", ";
    }
}

void btree::preorder_print()
{
    preorder_print(root);
    std::cout << '\n';
}

void btree::preorder_print(node *leaf)
{
    if (leaf != nullptr)
    {
        std::cout << leaf->value << ",";
        inorder_print(leaf->left);
        inorder_print(leaf->right);
    }
}
