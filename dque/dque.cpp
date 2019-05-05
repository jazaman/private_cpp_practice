#include <iostream>

class dque {
private:
    uint32_t mValue;
    uint32_t mKey;
public:
    dque* next;
    dque(uint32_t value, uint32_t key) : mValue(value), mKey(key),next(0) {}
    uint32_t key() { return mKey;};
    uint32_t value( ){ return mValue;};
};

int main()
{
    dque* head = 0;
    dque* temp = 0;
    

    for(int i = 0; i < 10; i++){
        temp = new dque(i, ~i);
        temp->next = head;
        head = temp; 
    }

    while (head) {
        std::cout << "Key: 0x" << std::hex << head->key()<< " Value: " << std::dec << head->value()<< std::endl;
        head = head->next;
    }
    std::cout << "Hello World!!" << std::endl;
    return 0;
}
