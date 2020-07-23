#include <iostream>
#include "dll.h"

DoublyLinkedList::DoublyLinkedList()
{
    head = tail = nullptr;
    size = 0;
}

DoublyLinkedList::~DoublyLinkedList()
{
}

void DoublyLinkedList::insert_front(int value)
{
    Node *temp = new Node(value);

    if (tail == nullptr)
    {
        head = tail = temp;
    }
    else
    {
        head->prev = temp;
        temp->next = head;
        head = temp;
    }
    size++;
}

void DoublyLinkedList::insert_back(int value)
{
    Node *temp = new Node(value);

    if (tail == nullptr)
    {
        head = tail = temp;
    }
    else
    {
        tail->next = temp;
        temp->prev = tail;
        tail = temp;
    }
    size++;

}

int DoublyLinkedList::delete_front()
{
    if (!is_empty()) {
        Node *temp = head;

        if (head == tail)
        {
            tail = nullptr;
        }
        int delValue = temp->data;
        head = head->next;

        delete temp;

        size--;

        return delValue;
    }

    return 0;
}

int DoublyLinkedList::delete_back()
{
    if (!is_empty())
    {
        Node *temp = tail;

        if (head == tail)
        {
            head = nullptr;
        }
        int delValue = temp->data;
        tail->next = nullptr;
        tail = tail->prev;

        delete temp;

        size--;

        return delValue;
    }

    return 0;
}

bool DoublyLinkedList::is_empty() const
{
    return size <= 0;
}

void DoublyLinkedList::display()
{
    Node *temp = head;

    std::cout << "\n\nDisplay in forward order\n";
    while (temp != nullptr)
    {
        std::cout << temp->data << " -> ";
        temp = temp->next;
    }
    std::cout << "nullptr";
}

int DoublyLinkedList::length() const
{
    return size;
}

void DoublyLinkedList::search(int value)
{
    if (!is_empty())
    {
        Node *temp = head;
        while(temp != nullptr)
        {
            if (temp->data == value)
            {
                std::cout << "Got it." << '\n';
                break;
            }
            temp = temp->next;
        }
    }
    else
    {
        std::cout << "List is empty" << '\n';
    }
}


int main()
{
    DoublyLinkedList *dll = new DoublyLinkedList();

    menu();

    int choice, nn;

    do
    {
        std::cout << "\n\nEnter your choice: ";
        std::cin >> choice;

        switch (choice)
        {
            case 1:
                std::cout << "Enter data: ";
                std::cin >> nn;
                dll->insert_front(nn);
                break;

            case 2:
                std::cout << "Enter data: ";
                std::cin >> nn;
                dll->insert_back(nn);
                break;

            case 3:
                std::cout << "Deleted value is: " << dll->delete_front();
                break;

            case 4:
                std::cout << "Deleted value is: " << dll->delete_back();
                break;

            case 5:
                dll->display();
                break;

            case 6:
                std::cout << "Number of nodes: " << dll->length();
                break;

            case 7:
                int searchVal;
                std::cout << "Enter search data: ";
                std::cin >> searchVal;
                dll->search(searchVal);
                break;

            default:
                std::cout << "Wrong choice" << '\n';
                break;
        }
    } while (choice != 8);


    return 0;
}

void menu()
{
    std::cout << "-------- MENU --------" << '\n'
    << "1. Insert at Front" << '\n'
    << "2. Insert at Back" << '\n'
    << "3. Delete from front" << '\n'
    << "4. Delete from Back" << '\n'
    << "5. Display" << '\n'
    << "6. Total Nodes" << '\n'
    << "7. Search" << '\n'
    << "8. Exit" << '\n';
}
