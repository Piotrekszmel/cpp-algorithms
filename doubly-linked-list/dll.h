#ifndef _DLL_H
#define _DLL_H

void menu();

struct Node
{
    int data;
    Node *next;
    Node *prev;
    explicit Node(int value) : data{ value }, next{ nullptr }, prev{ nullptr } {}
};


class DoublyLinkedList
{
public:
    DoublyLinkedList();
    ~DoublyLinkedList();
    void insert_front(int value);
    void insert_back(int value);
    int delete_front();
    int delete_back();
    void display();
    int length() const;
    bool is_empty() const;
    void search(int value);

private:
    Node *head;
    Node *tail;
    int size;
};

#endif
