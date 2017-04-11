#include <iostream>        //std::cout
#include <thread>          //std::thread
 
void foo() 
{
   for (int n = 0; n < 2000; n++) {
      std::cout << "foo ..." << std::endl; 
   }

}
  
void bar(int x)
{
   for (int n = 0; n < 2000; n++) {
      std::cout << "bar ..." << std::endl; 
   }
}
  
int main() 
{
    auto start = std::chrono::high_resolution_clock::now();

    std::thread first (foo);      //spawn new thread that calls foo()
    std::thread second (bar,0);   //spawn new thread that calls bar(0)
  
    std::cout << "main, foo and bar now execute concurrently...\n";
  
    //synchronize threads:
    first.join();                 //pauses until first finishes
    second.join();                //pauses until second finishes
  
   auto elapsed = std::chrono::high_resolution_clock::now() - start;
   long long microseconds = std::chrono::duration_cast<std::chrono::microseconds>(elapsed).count();
    std::cout << "foo and bar completed in "<< microseconds/1000 <<" mil seconds.\n";
  
    return 0;
}
