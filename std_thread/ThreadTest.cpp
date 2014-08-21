// thread example
#include <iostream>       // std::cout
#include <thread>         // std::thread
 
void foo() 
{
  // do stuff...
  for(int i = 0 ; i < 50; i++)
  {
    std::cout << " foo " << std::endl;
  }
}

void bar(int x)
{
  // do stuff...
  for(int i = 0 ; i < 50; i++)
  {
    std::cout << " bar " << std::endl;
  }
}

int main() 
{
  std::thread first (foo);     // spawn new thread that calls foo()
  std::thread second (bar,0);  // spawn new thread that calls bar(0)

  std::cout << "main, foo and bar now execute concurrently...\n";

  // synchronize threads:
  first.join();                // pauses until first finishes
  second.join();               // pauses until second finishes

  std::cout << "foo and bar completed.\n";

  return 0;
}
