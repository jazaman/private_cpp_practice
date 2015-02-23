#include <iostream>
#include <stdio.h>

template<typename T>
void QuickSort( T array[], int SIZE ) 
{

    if(SIZE == 1) {
      return;
    } else {
      
    }

    int pivot = SIZE/2;
    std::cout << pivot <<" - \n";
    
    QuickSort(array, pivot);
    //QuickSort(array+pivot, SIZE);

    
    //sort
    int smaller=0, larger=0;
    for(;array[smaller] < array[pivot] && smaller < pivot;smaller++) {
      std::cout <<" S: " << smaller 
                <<" P: " << pivot   
                <<" Z: " << SIZE
                <<" ["
                << array[smaller] 
                << "]"
                <<std::endl;
    }

   

    for(larger = SIZE-1; array[pivot] > array[larger] && larger > pivot; larger--)
    {
      std::cout <<" B: " << larger
                <<" P:" << pivot   
                <<" Z:" << SIZE
                <<" ["
                << array[larger] 
                << "]"
                <<std::endl;
        
    }

    std::swap(array[smaller], array[larger]);

    //for(;array[smaller] < array[pivot] && smaller <= pivot) {
    //  std::cout <<" - " << array[smaller] <<std::endl;
    //}    
}

template <typename T>
void print_array(T& array, unsigned size)
{
  for(auto x = std::begin(array); x != std::end(array); ++x ) {
    std::cout<< " " << *x << " ";
  }
  std::cout << std::endl;
}

int main()
{
    std::cout << "Hello World!!" << std::endl;
    int unsorted[] = {3,5,1,2,8,6};
    printf("The size of the array is:%lu\n\n", sizeof(unsorted)/sizeof(unsorted[0]));
    
    print_array(unsorted, sizeof(unsorted)/sizeof(unsorted[0]));
    QuickSort( unsorted, sizeof(unsorted)/sizeof(unsorted[0]));
    
    print_array(unsorted, sizeof(unsorted)/sizeof(unsorted[0]));
    return 0;
}
