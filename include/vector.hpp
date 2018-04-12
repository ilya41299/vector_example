#include <iostream>
#include <initializer_list> 

template <typename T>
class tree_t
{
private:
	struct node_t
	{
		node_t* left = nullptr;
		node_t* right = nullptr;
		T value = 0;
	};

private:
	node_t * root_;
public:
	tree_t();
	node_t* root()
	{
		return root_;
	}
	void check_operator(std::ostream& stream, char op, T value);
	void insert(T value);
	bool find(T value) const;
	bool equal(node_t* a, node_t* b) const;
	void print(std::ostream& stream, int level, node_t* node);
	void destroy(node_t* node);
	~tree_t();
	tree_t(std::initializer_list<T> keys);
	auto operator==(tree_t const & other) const;
	bool remove(T key);
};
	
template <typename T>
tree_t<T>::tree_t(std::initializer_list<T> keys)
{
	int n = keys.size();
	const int* _ptr = keys.begin();
	for (int i=0; i < n; i++)
	{
		insert(_ptr[i]);
	}
}

template <typename T>
tree_t<T>::tree_t()
{
	root_ = nullptr;
}

template <typename T>
tree_t<T>::~tree_t()
{
	destroy(root_);
}

template <typename T>
void tree_t<T>::check_operator(std::ostream& stream, char op, T value)
{
	switch (op)
	{
	case '+':
	{
		insert(value);
		break;
	}
	case '?':
	{
		if (find(value))
		{
			stream << "true";
		}
		else stream << "false";
		break;
	}
	case '=':
	{
		print(stream, 0, root_);
		break;
	}
	case 'q':
	{
		exit(0);
		break;
	}
	default:
	{
		stream << "Error of use operator";
	}
	}
}

template <typename T>
void tree_t<T>::insert(T value)
{
	node_t* node = new node_t;
	node->value = value;
	node->right = nullptr;
	node->left = nullptr;
	if (root_ == nullptr)
	{
		root_ = node;
		return;
	}

	node_t* vetka = root_;
	while (vetka != nullptr)
	{
		if (vetka->value < value)
		{
			if (vetka->right != nullptr)
			{
				vetka = vetka->right;
			}
			else
			{
				vetka->right = node;
				return;
			}
		}
		else if (vetka->value > value)
		{
			if (vetka->left != nullptr)
			{
				vetka = vetka->left;
			}
			else
			{
				vetka->left = node;
				return;
			}
		}
		else
			return;
	}
}

template <typename T>
void tree_t<T>::print(std::ostream& stream, int level, node_t* node)
{
	if (node == nullptr)
		return;

	print(stream, level + 1, node->right);

	for (size_t i = 0; i < level; i++)
	{
		stream << "---";
	}
	stream << node->value << std::endl;

	print(stream, level + 1, node->left);
}

template <typename T>
bool tree_t<T>::find(T value) const
{
	node_t* node = root_;
	while (node != nullptr)
	{
		if (node->value == value)
		{
			return true;
		}
		else
		{
			if (value <= node->value)
			{
				node = node->left;
			}
			else
				node = node->right;
		}
	}
	return false;
}

template <typename T>
void tree_t<T>::destroy(node_t* node)
{
	if (node != nullptr)
	{
		if (node->left) 
		{
			destroy(node->left);
		}
		if (node->right) 
		{
			destroy(node->right);
		}
		delete node;
	}
}


template <typename T>
bool tree_t<T>::remove(T key) 
{
if (root_ == nullptr)
{
	return false;
}
else
{
	node_t* param1 = root_;
	node_t* param2 = root_;
	while (1)
	{
		if (param2->value == key)
		{
			break;
		}
		else if (param2->value < key)
		{
			param1 = param2;
			param2 = param2->right;
		}
		else if (param2->value > key)
		{
			param1 = param2;
			param2 = param2->left;
		}
		else if (param2 == nullptr) {
			break;
		}
	}
	if (param2 == nullptr) {
		return false;
	}
	else {
		if (param2->left == nullptr && param2->right == nullptr) {
			if (param2 == param1->right) {
				param1->right = nullptr;
			}
			if (param2 == param1->left) {
				param1->left = nullptr;
			}
			delete param2;
			return true;
		}
		else {
			if (param2->left == nullptr && param2->right != nullptr) {
				if (param2 == param1->right) {
					param1->right = nullptr;
				}
				if (param2 == param1->left) {
					param1->left = nullptr;
				}
				delete param2;
			}
			else if (param2->left != nullptr && param2->right == nullptr) {
				if (param2 == param1->right) {
					param1->right = nullptr;
				}
				if (param2 == param1->left) {
					param1->left = nullptr;
				}
				delete param2;
			}
			else if (param2->left != nullptr && param2->right != nullptr) {
				node_t* param = param2;
				param1 = param2;
				param2 = param2->right;
				while (param2->left != nullptr) {
					param1 = param2;
					param2 = param2->left;
				}
				param->value = param2->value;
				param1->left = param2->right;
				delete param2;
			}
		}
	}
}
return true;
} 

template <typename T>
bool tree_t<T>::equal (node_t* first, node_t* second) const {
	if (first == nullptr && second == nullptr) return(true);
	else if (first != nullptr && second != nullptr)
	{
		return(
			first->value == second->value &&
			equal(first->left, second->left) &&
			equal(first->right, second->right)
			);
	}
	else return false;
}

template <typename T>
auto tree_t<T>::operator==(tree_t const & other) const {
	node_t* first = root_; node_t* second = other.root_;
	return (equal(first, second));
}
	
