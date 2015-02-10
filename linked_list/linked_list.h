#ifndef LINKED_LIST
#define LINKED_LIST

template<class T>
class Node {
public:
    Node(T data): data_(data), next_(0), prev_(0) {}
    void set_next(Node* next) { next_ = next; }
    Node* get_next() const { return next_; }
    const T& data() const { return data_; }

private:
    T        data_;
    Node     *next_;
    Node     *prev_;
};

template<class T>
class LinkedList {

public:
    LinkedList() : head(0) {}
    Node<T>* get_head() const { return head; }
    void add(T data) {
        Node<T>* node = new Node<T>(data);
        node->set_next(head);
        head = node;
    };
    void remove(T data) {
        Node<T> *prev = 0, *curr = head;
        while( curr  ) {
	    if( curr->data() == data) {
	        if(prev == 0) { //head
		    head = head->get_next();
	        } else {
                    prev->set_next(curr->get_next());
		}
                delete curr;
                curr = 0;
            } else {
                prev = curr;
                curr = curr->get_next();
            }
        }
    }

    void delete_list() {
        while (head) {
	    Node<T>* temp = head;
            head->set_next(head->get_next());
            delete temp;
        }
    }
private:
    Node<T>* head;    

};

#endif //LINKED_LIST
