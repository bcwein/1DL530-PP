#ifndef lacpp_sorted_list_hpp
#define lacpp_sorted_list_hpp lacpp_sorted_list_hpp

#include <thread>
#include <mutex>

const int MAX = 256;

/* a sorted list implementation by David Klaftenegger, 2015
 * please report bugs or suggest improvements to david.klaftenegger@it.uu.se
 */

/* struct for list nodes */
template<typename T>
struct node {
	T value;
	node<T>* next;
	std::mutex tl;
};

/* non-concurrent sorted singly-linked list */
template<typename T>
class sorted_list {
	node<T>* first = nullptr;

	public:
		/* default implementations:
		 * default constructor
		 * copy constructor (note: shallow copy)
		 * move constructor
		 * copy assignment operator (note: shallow copy)
		 * move assignment operator
		 *
		 * The first is required due to the others,
		 * which are explicitly listed due to the rule of five.
		 */
		sorted_list() = default;
		sorted_list(const sorted_list<T>& other) = default;
		sorted_list(sorted_list<T>&& other) = default;
		sorted_list<T>& operator=(const sorted_list<T>& other) = default;
		sorted_list<T>& operator=(sorted_list<T>&& other) = default;
		~sorted_list() {
			while(first != nullptr) {
				remove(first->value);
			}
		}
		/* insert v into the list */
		void insert(T v) {
			/* first find position */
			node<T>* pred = nullptr;
			node<T>* succ = first;
			succ->tl.lock();
			while(succ != nullptr && succ->value < v) {
				pred = succ;
				pred->tl.unlock();
				succ->next->tl.lock();
				succ = succ->next;
			}
			
			/* construct new node */
			node<T>* current = new node<T>();
			current->tl.lock();
			current->value = v;
			/* insert new node between pred and succ */
			current->next = succ;
			pred->tl.lock();
			if(pred == nullptr) {
				first = current;
				current->tl.unlock();
			} else {
				pred->next = current;
				current->tl.unlock();
			}
			pred->tl.unlock();
		}

		void remove(T v) {
			/* first find position */
			node<T>* pred = nullptr;
			node<T>* current = first;
			current->tl.lock();
			while(current != nullptr && current->value < v) {
				pred = current;
				pred->tl.unlock();
				current->next->tl.lock();
				current = current->next;
			}
			if(current == nullptr || current->value != v) {
				/* v not found */
				current->tl.unlock();
				return;
			}
			/* remove current */
			pred->tl.lock();
			if(pred == nullptr) {
				first = current->next;
				current->tl.unlock();
			} else {
				pred->next = current->next;
				current->tl.unlock();
			}
			pred->tl.unlock();
			delete current;
		}

		/* count elements with value v in the list */
		std::size_t count(T v) {
			std::size_t cnt = 0;
			/* first go to value v */
			node<T>* pred = first;
			node<T>* current = first;
			current->tl.lock();
			while(current != nullptr && current->value < v) {
				pred = current;
				current = current->next;
				current->tl.lock();
				pred->tl.unlock();
			}
			while(current != nullptr && current->value == v) {
				pred = current;
				cnt++;
				current = current->next;
				current->tl.lock();
				pred->tl.unlock();
			}
			return cnt;
		}
};

#endif // lacpp_sorted_list_hpp
