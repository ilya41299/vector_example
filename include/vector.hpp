#include <iostream>
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
	void print(std::ostream& stream, int level, node_t* node);
	void destroy(node_t* node);
	~tree_t();
};
	
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

	for (unsigned int i = 0; i < level; i++)
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
		destroy(node->left);
		destroy(node->right);
		delete node;
	}
}
