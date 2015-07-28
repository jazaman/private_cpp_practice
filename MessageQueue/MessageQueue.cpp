#include <iostream>

class Message {

public:

    void set_value(const uint32_t value) {

    }

    void set_key(const uint32_t value) {

    }
   
    const uint32_t get_value() const {
        return value_; 
    }
    
    const uint32_t get_key() const {
        return key_;
    }

private:
    uint32_t value_;
    uint32_t key_; 
};

int main() {
    std::cout << "Hello World!!" << std::endl;
    return 0;
}
