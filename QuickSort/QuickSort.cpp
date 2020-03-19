#include <iostream>
#include <stdio.h>

static int level = 0;

template <typename T>
void print_array(T (&array))
{
    for(auto x = std::begin(array); x != std::end(array); ++x ) {
      std::cout<< " " << *x << " ";
    }
    std::cout << std::endl;
}

void quickSort(int (&arr)[8], int left, int right) {
#ifdef DEBUG
    std::cout << "Recursion level:[" << level++ << "] left::" << left << " right::" << right;
#endif
    int i = left, j = right; 
    int tmp; 
    int pivot = arr[(left + right) / 2]; //arr[(left + right) / 2];
    int iteration = 1;
#ifdef DEBUG
    std::cout << " Pivot [" << (left + right) / 2 << "] : " << pivot << std::endl;
#endif
    /* partition */ 
    while (i <= j) { 
#ifdef DEBUG
        std::cout << "iter: [" << iteration++ << "] ::";
#endif
        while (arr[i] < pivot) 
              i++;

        while (arr[j] > pivot) 
              j--;

        if (i <= j) { 
#ifdef DEBUG
            std::cout << " swapping [i=" << i <<"]:" << arr[i] << " and [j=" << j <<"]:" << arr[j] << " ";
#endif
            tmp = arr[i];
            arr[i] = arr[j];
            arr[j] = tmp;
            i++;
            j--;
        }
#ifdef DEBUG
        else {
        	std::cout << " no swapping i:" << i << " > j:" << j << " ";
        }
#endif
        std::cout << "array[] = ";
        print_array(arr);
        std::cout << std::endl;
    }

    /* recursion */ 
    if (left < j)
        quickSort(arr, left, j); 
    if (i < right) 
        quickSort(arr, i, right);

}

template<typename T>
void QuickSort( T (&array), int start, int end ) 
{ 
	int i = start, j = end;
	int SIZE = end - start;
    if(SIZE <= 1) {
      return;
    } else {
      
    }

    if (level++ == 10) return;

    int pivot = SIZE/2;
    
    // sort
    // int smaller=0, larger=0;
    print_array(array);
    int iter = 1;
    while( start <= end ) {
    	while(array[start] < array[pivot]) {
    		start++;
    	}
    	std::cout << "iteration: [" << iter++ << "] " <<std::endl;
#ifdef DEBUG
            std::cout <<" S: " << array[start]
                      <<" P: " << array[pivot]
                      << std::endl;
#endif

        while(array[pivot] < array[end]) {
            end--;
        } 
#ifdef DEBUG
            std::cout << " E: " << array[end]
                      << " P: "  << array[pivot]
                      << std::endl;
#endif


 
        if(start <= end ) {
        	std::cout << "swapping ["<< start<< "] " << array[start] << " and ["<< end<< "] " << array[end]<< std::endl;
            std::swap(array[start++], array[end--]);
        }
        print_array(array);
    }


    //for(;array[smaller] < array[pivot] && smaller <= pivot) {
    //  std::cout <<" - " << array[smaller] <<std::endl;
    //}
    if(i<start)
       QuickSort(array, i, pivot);
    if(end < j)
       QuickSort(array, pivot+1, j);
}

int main() {
    std::cout << "Hello World!!" << std::endl;
    int unsorted[] = {-1,4, 17,3,9,10, 11,13};
    printf("The size of the array is:%lu\n\n", 
        sizeof(unsorted)/sizeof(unsorted[0]));
    std::cout << "array[] = ";
    
    print_array(unsorted);
    quickSort( unsorted, 0, sizeof(unsorted)/sizeof(unsorted[0])-1);
    //QuickSort( unsorted, 0, sizeof(unsorted)/sizeof(unsorted[0])-1);
    
    print_array(unsorted);
    return 0;
}
