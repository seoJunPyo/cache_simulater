#pragma once
template <typename T>
class CList
{
private:
	struct Node
	{
		T _data;
		Node* _Prev;
		Node* _Next;
	};
public:
	class iterator
	{
	private:
		Node* _node;

	public:
		iterator(Node* node = nullptr)
			: _node(node) {};

		iterator operator++()
		{
			_node = _node->_Next;

			return *this;
		}

		iterator& operator++(int)
		{
			iterator temp = { _node };

			_node = _node->_Next;

			return temp;
		}

		iterator operator--()
		{
			_node = _node->_Prev;

			return *this;
		}

		iterator& operator--(int)
		{
			iterator temp = { _node };

			_node = _node->_Prev;

			return temp;
		}

		T& operator*() { return _node->_data; }

		bool operator==(const iterator& other) { return _node == other._node; }
		bool operator!=(const iterator& other) { return _node != other._node; }

		Node* get_node() { return _node; }
	};

public:
	CList()
	{
		_head._Next = &_tail;
		_head._Prev = nullptr;

		_tail._Next = nullptr;
		_tail._Prev = &_head;
	}
	~CList()
	{
		clear();
	}

	iterator begin() { return iterator{ _head._Next }; }
	iterator end() { return iterator{ &_tail }; }

	void push_front(T data)
	{
		Node* new_node = new Node;

		new_node->_data = data;
		new_node->_Prev = &_head;
		new_node->_Next = _head._Next;

		_head._Next->_Prev = new_node;
		_head._Next = new_node;

		_size++;
	}

	void push_back(T data)
	{
		Node* new_node = new Node;

		new_node->_data = data;
		new_node->_Next = &_tail;
		new_node->_Prev = _tail._Prev;

		_tail._Prev->_Next = new_node;
		_tail._Prev = new_node;

		_size++;
	}

	void pop_front()
	{
		if (empty())
			return;

		Node* popped = _head._Next;

		_head._Next = _head._Next->_Next;
		_head._Next->_Prev = &_head;

		_size--;

		delete popped;
	}

	void pop_back()
	{
		if (empty())
			return;

		Node* popped = _tail._Prev;

		_tail._Prev = _tail._Prev->_Prev;
		_tail._Prev->_Next = &_tail;

		_size--;

		delete popped;
	}

	void clear()
	{
		while (!empty())
		{
			pop_back();
		}
	}

	int size() { return _size; }
	bool empty() { return _size == 0; }

	iterator erase(iterator iter)
	{
		Node* cur = iter.get_node();
		iterator next = { cur->_Next };

		cur->_Next->_Prev = cur->_Prev;
		cur->_Prev->_Next = cur->_Next;

		delete cur;

		_size--;

		return next;
	}

	void remove(T data)
	{
		for (iterator iter = begin(); iter != end(); ++iter)
		{
			if (*iter == data)
			{
				erase(iter);
				break;
			}
		}
	}

private:
	int _size = 0;
	Node _head;
	Node _tail;
};


