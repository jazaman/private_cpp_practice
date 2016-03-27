#include <iostream>
#include <vector>
#include <map>

class Storage {
public:
   Storage(char c) {
      std::cout << "Default" << std::endl;
      aChar = c;
   }
   
   Storage(const Storage &storage) {
      aChar = storage.aChar;
      std::cout << "Copy" << std::endl;
   }
   
   Storage& operator=(const Storage& storage) {
      aChar = storage.aChar;
      std::cout << "Assignment" << std::endl;
      return *this;
   }
   char aChar;

   typedef std::vector<Storage> StorageQueueT;
   typedef std::map<unsigned int, StorageQueueT> StorageMapT;
};

int main()
{
    Storage a(a), b(b);
    Storage c(b);
    
    Storage::StorageQueueT storageQueue;
    
    storageQueue.push_back(a);
    storageQueue.push_back(b);
    storageQueue.push_back(c);

  

    Storage::StorageMapT storageMap1, storageMap2;


    storageMap1.insert(std::make_pair(1, storageQueue)); 
    storageMap2.insert(std::make_pair(1, std::move(storageQueue))); 


    
    std::cout << "Hello World!!" << std::endl;
    return 0;
}
