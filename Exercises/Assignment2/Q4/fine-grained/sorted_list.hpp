#ifndef lacpp_sorted_list_hpp
#define lacpp_sorted_list_hpp lacpp_sorted_list_hpp

#include <thread>
#include <mutex>

/* a sorted list implementation by David Klaftenegger, 2015
 * please report bugs or suggest improvements to david.klaftenegger@it.uu.se
 */

/* struct for list nodes */
template <typename T>
struct node
{
	T value;
	node<T> *next;
	std::mutex l;
};

/* non-concurrent sorted singly-linked list */
template <typename T>
class sorted_list
{
	node<T> *first = nullptr;
	std::mutex l;

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
	sorted_list(const sorted_list<T> &other) = default;
	sorted_list(sorted_list<T> &&other) = default;
	sorted_list<T> &operator=(const sorted_list<T> &other) = default;
	sorted_list<T> &operator=(sorted_list<T> &&other) = default;
	~sorted_list()
	{
		while (first != nullptr)
		{
			remove(first->value);
		}
	}
	/* insert v into the list */
	void insert(T v)
	{
		/* first find position */
		node<T> *pred = nullptr;
		l.lock();
		node<T> *succ = first;
		if(succ) succ->l.lock();
		l.unlock();
		/* Check that first is not nullptr*/
		while (succ != nullptr && succ->value < v)
		{
			pred = succ;
			succ->l.unlock();
			succ = succ->next;
			if (succ != nullptr)
			{
				succ->l.lock();
			}
		}

		/* construct new node */
		node<T> *current = new node<T>();
		current->value = v;

		/* insert new node between pred and succ */
		current->next = succ;
		if (pred == nullptr)
		{
			first = current;
		}
		else
		{
			pred->next = current;
		}
		if (succ) succ->l.unlock();
	}

	void remove(T v)
	{
		/* first find position */
		node<T> *pred = nullptr;
		l.lock();
		node<T> *current = first;
		if(current) current->l.lock();
		l.unlock();
		while (current != nullptr && current->value < v)
		{
			pred = current;
			current->l.unlock();
			current = current->next;
			if(current != nullptr)
			{
				current->l.lock();
			}
		}

		if (current == nullptr || current->value != v)
		{
			/* v not found */
			if (current) current->l.unlock();
			return;
		}

		if(pred) pred->l.lock();
		/* remove current */
		if (pred == nullptr)
		{
			first = current->next;
		}
		else
		{
			pred->next = current->next;
		}
		if(pred) pred->l.unlock();
		if(current) current->l.unlock();
		delete current;
	}

	/* count elements with value v in the list */
	std::size_t count(T v)
	{
		std::size_t cnt = 0;
		/* first go to value v */
		l.lock();
		node<T> *current = first;
		if(current) current->l.lock();
		l.unlock();
		while (current != nullptr && current->value < v)
		{
			current->l.unlock();
			current = current->next;
			if(current) current->l.lock();
		}
		/* count elements */
		while (current != nullptr && current->value == v)
		{
			cnt++;
			current->l.unlock();
			current = current->next;
			if(current) current->l.lock();
		}
		if(current) current->l.unlock();
		return cnt;
	}
};

#endif // lacpp_sorted_list_hpp
