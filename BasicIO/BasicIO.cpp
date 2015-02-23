#include <iostream>
#include <sstream>
#include <string>
#include "BinaryTree.h"

void QuickSort(int array[], int end_index) {
    int pivot_index = end_index /2;
    if(pivot_index == 0) {
        if(array[end_index] > array[pivot_index]) {
            std::swap(array[end_index], array[pivot_index]);
        }
        return;
    }
    
    int start = 0, left = 0, right = end_index;
    
    while(left < pivot_index && right > pivot_index) {
        while(array[left++] < array[pivot_index]); //left
        --left; //pointing at the item bigger than the one at pivot index
        
        while(array[right--] > array[pivot_index]); //right
        ++right; //pointing at the item less than the one at pivot index
    
        std::swap(array[left], array[right]);
        
        
    }
    if(array[start] > array[pivot_index]) {
        std::swap(array[start], array[pivot_index]);
    }
    
    
}

int main(int argc, char *argv[]) {
    
    std::string input;
    std::istringstream input_stream;
    int number;
    bool quit = false;
    BinaryTree<int> tree;
    do {
        std::cout << "Please Enter Number[s] or 'q' to quit: " << std::endl;
        std::getline(std::cin, input);
        input_stream.str(input);
        while( input_stream >> number) {
            std::cout << "Adding Number: " << number << std::endl;
            tree.add(number);
        }
        
        if(input_stream.eof()) {
            std::cout << "Current storage:" << std::endl;
            tree.display();
        } else if ( (input_stream.rdstate() & std::istringstream::failbit ) != 0 ) {
            //handle 
            std::cout <<"Quit" << std::endl;
            quit = true;
        }
        input_stream.clear();
    } while (!quit);
    
    tree.display();
    return 0;
}
