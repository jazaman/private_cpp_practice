#include <iostream>
#include <stdio.h>

static int level = 0;

template <typename T>
void print_array(T (&array), unsigned size)
{
    for(auto x = std::begin(array); x != std::end(array); ++x ) {
      std::cout<< " " << *x << " ";
    }
    std::cout << std::endl;
}

void quickSort(int (&arr)[6], int left, int right) {
      
<<<<<<< HEAD
    std::cout << "level: " << level << " " ;
=======
    std::cout << "level: " << level++ << " " ;
>>>>>>> origin/master

    int i = left, j = right; 
    int tmp; 
    int pivot = arr[(left + right) / 2]; 

    /* partition */ 
    while (i <= j) { 
        while (arr[i] < pivot) 
              i++;

        while (arr[j] > pivot) 
              j--;

        if (i <= j) { 
              tmp = arr[i]; 
              arr[i] = arr[j]; 
              arr[j] = tmp; 
              i++; 
              j--; 
        } 
    }; 
   
    print_array(arr, right+1);

    /* recursion */ 
    if (left < j)
        quickSort(arr, left, j); 
    if (i < right) 
        quickSort(arr, i, right);

}

template<typename T>
void QuickSort( T (&array), int start, int end ) 
{ 
    int SIZE = end - start;
    if(SIZE <= 1) {
      return;
    } else {
      
    }

    int pivot = SIZE/2;
    
    // sort
    // int smaller=0, larger=0;
    
    while( start <= end ) {
    
        for(;array[start] < array[pivot]; start++)
        {
        /*    std::cout <<" S: " << start 
                      <<" P: " << pivot   
                      <<" Z: " << SIZE
                      <<" ["
                      << array[start] 
                      << "]"
                      << std::endl;
        */
        } 
 
        for(; array[pivot] < array[end]; end--)
        {
           /* std::cout << " E: " << end
                      << " P:"  << pivot   
                      << " Z:"  << SIZE
                      << " ["   << array[end] 
                      << "]"    << std::endl; 
           */
        }
 
        if(start <= end ) {
            std::swap(array[start++], array[end--]);
        }
        print_array(array, end + 1);
    }


    //for(;array[smaller] < array[pivot] && smaller <= pivot) {
    //  std::cout <<" - " << array[smaller] <<std::endl;
    //}    
    QuickSort(array, start, pivot);
    QuickSort(array, pivot+1, end);
}

int main()
{
    std::cout << "Hello World!!" << std::endl;
    int unsorted[] = {3,5,2,1,8,6};
    printf("The size of the array is:%lu\n\n", 
        sizeof(unsorted)/sizeof(unsorted[0]));
    
    print_array(unsorted, sizeof(unsorted)/sizeof(unsorted[0]));
    quickSort( unsorted, 0, sizeof(unsorted)/sizeof(unsorted[0])-1);
    
    print_array(unsorted, sizeof(unsorted)/sizeof(unsorted[0]));
    return 0;
}
