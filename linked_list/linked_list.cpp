#include <iostream>
#include "linked_list.h"
#include <stdio.h>
template<class T>
void print_list(LinkedList<T>& list)
{
    Node<T>* head = list.get_head();
    printf("HEAD");
    while(head) {
        printf("->%d", head->data());
        head = head->get_next();
    }
    printf("->EOL\n");
}
int main()
{

    LinkedList<int> list;
    list.add(4);
    list.add(6);
    list.add(1);
    list.add(3);
    list.add(7);
    print_list(list);

    list.remove(1);
    print_list(list);   
    
    auto lamda = [] (Node<int>* node) { printf("%d ", node->data());   
   
 
    std::cout << "Hello World!!" << std::endl;
    return 0;
}
