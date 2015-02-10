template <class T> 
struct Node {
	Node(T qData) : data(qData), left(0), right(0), prev(0) {}
	T data;
	Node<T> *left;
	Node<T> *right;
	Node<T> *prev;	
};

template <class T>
class BinaryTree {
public:
    typedef void (*action)(Node<T>*);
	void add(T data) {
		Node<T> *curr = head;
		Node<T> *prev = 0;
		while(curr) {
			prev = curr;
			if(data > curr->data) {
#ifdef DEBUG
				std::cout << curr->data << "\n\\\n";
#endif
				curr = curr->right;				
			} else {
#ifdef DEBUG
				std::cout << curr->data << "\n/\n";
#endif
				curr = curr->left;				
			}			
		}
        Node<T> *tmp = new Node<T>(data);
        tmp->prev = prev; 
		if(!prev) { //head
			head = tmp;
		} else {
            (data > prev->data ? prev->right : prev->left) = tmp;		
		}
#ifdef DEBUG		
		std::cout << data << std::endl;
#endif
	}
	
	void browse(Node<T> *curr, action act) {
		if(!curr) {
			return;
		}
		browse(curr->left, act);
		browse(curr->right, act);
		act(curr);		
	}
	
	void display() {
		auto lamda = [](Node<T>* curr) { std::cout << "[" << curr->data << "] ";};
		browse(head, lamda);
		std::cout << std::endl;
	}
	
	void destroy() {
		auto lamda = [](Node<T>* curr) { delete curr;};		
		browse(head, lamda);
	}
	
	BinaryTree() : head(0){}
	Node<T>* get_head() const {return head;}
	~BinaryTree() { destroy();}
private:
    Node<T> *head;	
};
