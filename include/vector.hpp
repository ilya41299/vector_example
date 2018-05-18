#include <iostream>
#include <algorithm>
#include <cassert>

template <typename T>
class rb_tree {
	struct node_t {
		node_t * left;
		node_t * right;
		node_t * parent;
		bool color; // 0 - black, 1 - red
		T value;
	};
	node_t *root_;
public:
	rb_tree();
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
	~rb_tree();
	rb_tree(std::initializer_list<T> keys);
	auto operator==(rb_tree const & other) const;
	bool isEmpty()
	{
		if (root_ == nullptr)
		{
			return true;
		}
		return false;
	}
	node_t * grandparent(node_t * n) const
	{
		if (n != nullptr && n->parent != nullptr)
		{
			return n->parent->parent;
		}
		else return nullptr;
	}
	node_t * uncle(node_t * n) const
	{
		node_t * grand = grandparent(n);
		if (grand == nullptr)
		{
			return nullptr; // если не существует grand, то нет и uncle
		}
		else if (n->parent == grand->left)
		{
			return grand->right;
		}
		else return grand->left;
	}

	void rotate_left(node_t *n)
	{
		node_t *vetka = n->right;
		if (root_ == n) // Если поворот вокруг корня, то корень дерева переопределяем
		{
			root_ = n->right;
		}
		vetka->parent = n->parent; /* при этом, возможно, vetka становится корнем дерева */
		if (n->parent != nullptr) { // если у n есть отец
			if (n->parent->left == n) // и n-его левый сын
				n->parent->left = vetka; // перенаправим левый указатель отца на временный элемент
			else // если же n-правый сын
				n->parent->right = vetka; // перенаправим правый указатель отца на временный элемент
		}

		n->right = vetka->left; // переопределип правый указатель для n
		if (vetka->left != nullptr) // и если временный имеет левого сына
		{
			vetka->left->parent = n; // то его отцом станет n
		}
		n->parent = vetka; // отцом для n становится временный элемент
		vetka->left = n; // и левый указатель временного направляем на наш n  
	}

	void rotate_right(node_t *n)
	{
		node_t *vetka = n->left;
		if (root_ == n) // Если поворот вокруг корня, то корень дерева переопределяем
		{
			root_ = n->left;
		}
		vetka->parent = n->parent;
		if (n->parent != nullptr) {
			if (n->parent->left == n)
				n->parent->left = vetka;
			else
				n->parent->right = vetka;
		}

		n->left = vetka->right;
		if (vetka->right != nullptr)
			vetka->right->parent = n;

		n->parent = vetka;
		vetka->right = n;
	}

	/* Текущий узел N в корне дерева.
	В этом случае, он перекрашивается в чёрный цвет, чтобы оставить верным Свойство 2 (Корень — чёрный).
	Так как это действие добавляет один чёрный узел в каждый путь,
	Свойство 5 (Все пути от любого данного узла до листовых узлов содержат одинаковое число чёрных узлов) не нарушается.*/
	void insert_case1(node_t *vetka)
	{
		if (vetka->parent == nullptr)
			vetka->color = false;
		else
			insert_case2(vetka);
	}

	/*Предок P текущего узла чёрный, то есть Свойство 4 (Оба потомка каждого красного узла — чёрные) не нарушается.
	В этом случае дерево остаётся корректным.
	Свойство 5 (Все пути от любого данного узла до листовых узлов содержат одинаковое число чёрных узлов) не нарушается,
	потому что текущий узел N имеет двух чёрных листовых потомков, но так как N является красным,
	путь до каждого из этих потомков содержит такое же число чёрных узлов, что и путь до чёрного листа,
	который был заменен текущим узлом, так что свойство остается верным.*/
	void insert_case2(node_t *vetka)
	{
		if (vetka->parent->color == false)
			return; /* Дерево сбалансировано */
		else
			insert_case3(vetka);
	}

	/* Если и родитель P и дядя U — красные, то они оба могут быть перекрашены в чёрный и дедушка G станет красным
	(для сохранения свойства 5 (Все пути от любого данного узла до листовых узлов содержат одинаковое число чёрных узлов)).
	Теперь у текущего красного узла N чёрный родитель. Так как любой путь через родителя или дядю должен проходить через дедушку,
	число чёрных узлов в этих путях не изменится. Однако, дедушка G теперь может нарушить свойства 2 (Корень — чёрный)
	или 4 (Оба потомка каждого красного узла — чёрные) (свойство 4 может быть нарушено, так как родитель G может быть красным).
	Чтобы это исправить, вся процедура рекурсивно выполняется на G из случая 1.*/
	void insert_case3(node_t *vetka)
	{
		node_t *u = uncle(vetka);
		node_t *g;

		if ((u != nullptr) && (u->color == true)) {
			// && (n->parent->color == true) Второе условие проверяется в insert_case2, то есть родитель уже является красным.
			vetka->parent->color = false;
			u->color = false;
			g = grandparent(vetka);
			g->color = true;
			insert_case1(g);
		}
		else {
			insert_case4(vetka);
		}
	}

	/*Случай 4: Родитель P является красным, но дядя U — чёрный.
	Также, текущий узел N — правый потомок P, а P в свою очередь — левый потомок своего предка G.
	В этом случае может быть произведен поворот дерева, который меняет роли текущего узла N и его предка P.
	Тогда, для бывшего родительского узла P в обновленной структуре используем случай 5,
	потому что Свойство 4 (Оба потомка любого красного узла — чёрные) все ещё нарушено.
	Вращение приводит к тому, что некоторые пути (в поддеревe) проходят через узел N, чего не было до этого.
	Это также приводит к тому, что некоторые пути не проходят через узел P.
	Однако, оба эти узла являются красными, так что Свойство 5 (Все пути от любого данного узла до листовых узлов
	содержат одинаковое число чёрных узлов) не нарушается при вращении. Однако Свойство 4 всё ещё нарушается,
	но теперь задача сводится к Случаю 5.*/
	void insert_case4(node_t *vetka)
	{
		node_t *g = grandparent(vetka);

		if ((vetka == vetka->parent->right) && (vetka->parent == g->left)) {
			rotate_left(vetka->parent);

			vetka = vetka->left;
		}
		else if ((vetka == vetka->parent->left) && (vetka->parent == g->right)) {
			rotate_right(vetka->parent);

			vetka = vetka->right;
		}
		insert_case5(vetka);
	}

	/* Родитель P является красным, но дядя U — чёрный, текущий узел N — левый потомок P и P — левый потомок G.
	В этом случае выполняется поворот дерева на G. В результате получается дерево, в котором бывший родитель P
	теперь является родителем и текущего узла N и бывшего дедушки G. Известно, что G — чёрный,
	так как его бывший потомок P не мог бы в противном случае быть красным (без нарушения Свойства 4).
	Тогда цвета P и G меняются и в результате дерево удовлетворяет Свойству 4 (Оба потомка любого красного узла — чёрные).
	Свойство 5 (Все пути от любого данного узла до листовых узлов содержат одинаковое число чёрных узлов) также остается верным,
	так как все пути, которые проходят через любой из этих трех узлов, ранее проходили через G, поэтому теперь они все проходят через P.
	В каждом случае, из этих трёх узлов только один окрашен в чёрный.*/
	void insert_case5(node_t *vetka)
	{
		node_t *g = grandparent(vetka);

		vetka->parent->color = false;
		g->color = true;
		if ((vetka == vetka->parent->left) && (vetka->parent == g->left)) {
			rotate_right(g);
		}
		else {
			rotate_left(g);
		}
	}

	// Для удаления
	

	bool remove(T value)
	{
		if (root_ == nullptr)
		{
			return false;
		}
		else
		{
			node_t* param1 = nullptr;
			node_t* param2 = root_;
			// Цикл поиска удаляемого эл-та
			while (1)
			{
				if (param2->value == value)
				{
					break;
				}
				else if (value > param2->value)
				{
					param1 = param2;
					param2 = param2->right;
				}
				else if (value < param2->value)
				{
					param1 = param2;
					param2 = param2->left;
				}
				if (param2 == nullptr)
				{
					return false;
				}
			}
			//если у найденного эл-та нет детей и он яв-ся корнем - занулим корень
			if (param2->left == nullptr && param2->right == nullptr)
			{
				if (param2 == root_)
				{
					node_t* node = root_;
					root_ = nullptr;
					delete node;
				}
				else
				{
					// если удаляемый эл-т левый сын отца
					if (param1->left == param2)
					{
						if (param2->color == false) //если уд. эл-т чёрный обращаемся к кейсу удаления
							delete_case1(param2);
						if (param2->parent->left == param2)
							param2->parent->left = nullptr;
						else if (param2->parent->right == param2)
							param2->parent->right = nullptr;
						delete param2;
					}
					if (param1->right == param2)
					{
						if (param2->color == false)
							delete_case1(param2);
						if (param2->parent->left == param2)
							param2->parent->left = nullptr;
						else if (param2->parent->right == param2)
							param2->parent->right = nullptr;
						delete param2;
					}
				}
			}
			else if ((param2->left != nullptr && param2->right == nullptr)
				|| (param2->left == nullptr && param2->right != nullptr))
			{
				if (param2 == root_)
				{
					node_t* node = root_;
					if (param2->left != nullptr)
					{
						root_ = param2->left;
						root_->parent = nullptr;
					}
					else if (param2->right != nullptr)
					{
						root_ = param2->right;
						root_->parent = nullptr;
					}
					delete node;
				}
				else
				{
					if (param1->left == param2)
					{
						if (param2->left != nullptr)
						{
							param1->left = param2->left;
							param2->left->parent = param1;
							if (param2->color == false)
							{
								if (param1->left->color == true)
									param1->left->color = false;
								else
									delete_case1(param1->left);
							}
							delete param2;
						}
						else if (param2->right != nullptr)
						{
							param1->left = param2->right;
							param2->right->parent = param1;
							if (param2->color == false)
							{
								if (param1->left->color == true)
									param1->left->color = false;
								else
									delete_case1(param1->left);
							}
							delete param2;
						}
					}
					else if (param1->right == param2)
					{
						if (param2->left != nullptr)
						{
							param1->right = param2->left;
							param2->left->parent = param1;
							if (param2->color == false)
							{
								if (param1->right->color == true)
									param1->right->color = false;
								else
									delete_case1(param1->right);
							}
							delete param2;
						}
						else if (param2->right != nullptr)
						{
							param1->right = param2->right;
							param2->right->parent = param1;
							if (param2->color == false)
							{
								if (param1->right->color == true)
									param1->right->color = false;
								else
									delete_case1(param1->right);
							}
							delete param2;
						}
					}
				}
			}
			else if (param2->left != nullptr && param2->right != nullptr)
			{
				node_t* node = param2;
				param1 = param2;
				param2 = param2->right;
				if (param2->left == nullptr)
				{
					if (param2->right == nullptr)
					{
						bool tr = node->color;
						node->value = param2->value;
						node->color = param2->color;
						node->right = nullptr;
						delete param2;
						if (tr == false)
						{
							if (node->color == true)
								node->color = false;
							else
								delete_case1(node);
						}
					}
					else
					{
						bool tr = node->color;
						node->value = param2->value;
						node->color = param2->color;
						node->right = param2->right;
						param2->right->parent = node;
						delete param2;
						if (tr == false)
						{
							if (node->color == true)
								node->color = false;
							else
								delete_case1(node);
						}
					}
				}
				else
				{
					while (param2->left != nullptr)
					{
						param1 = param2;
						param2 = param2->left;
					}
					if (param2->right == nullptr)
					{
						bool tr = node->color;
						node->value = param2->value;
						node->color = param2->color;
						param1->left = nullptr;
						delete param2;
						if (tr == false)
						{
							if (node->color == true)
								node->color = false;
							else
								delete_case1(node);
						}
					}
					else
					{
						bool tr = node->color;
						node->value = param2->value;
						node->color = param2->color;
						param1->left = param2->right;
						param2->right->parent = param1;
						delete param2;
						if (tr == false)
						{
							if (node->color == true)
								node->color = false;
							else
								delete_case1(node);
						}
					}
				}
			}
			return true;
		}
	}



	// поиск брата n
	node_t* sibling(node_t* n) 
	{
		if (n == n->parent->left)
			return n->parent->right;
		else
			return n->parent->left;
	}

	//N стал корнем. В этом случае, все сделано. Мы удалили один чёрный узел из каждого пути и новый корень является чёрным узлом
	void delete_case1(node_t* n)
	{
		if (n->parent != nullptr)
			delete_case2(n);
	}

	//Если его брат красный, то вращаем вокруг отца N либо влево, либо вправо.
	void delete_case2(node_t* n)
	{
		node_t* s = sibling(n);
		if (s == nullptr)
			return;
		if (s->color == true)
		{
			n->parent->color = true;
			s->color = false;
			if (n == n->parent->left) // если n левый сын отца - вращаем влево
				rotate_left(n->parent);
			else
				rotate_right(n->parent); // если n правый сын отца - вращаем вправо
		}
		delete_case3(n);
	}

	// Если P, N, S, Sl,Sr - черные, то перекрашиваем S в красный. и вызываем от P первый кейс
	void delete_case3(node_t* n)
	{
		node_t* s = sibling(n);
		if (s->left == nullptr || s->right == nullptr)
			return;
		if ((n->parent->color == false) && (s->color == false) && (s->left->color == false)
			&& (s->right->color == false))
		{
			s->color = true;
			delete_case1(n->parent);
		}
		else
			delete_case4(n);
	}

	//Если P - красный, а N, S, Sl и Sr - черные, то меняем цвета у P и S
	void delete_case4(node_t* n)
	{
		node_t* s = sibling(n);
		if ((n->parent->color == true) && (s->color == false) && (s->left->color == false)
			&& (s->right->color == false))
		{
			s->color = true;
			n->parent->color = false;
		}
		else
			delete_case5(n);
	}

	//Если S чёрный и Sl либо Sr красный, то поворачиваем
	// Если Sr - чёрный, то влево
	// Если Sl - чёрный, то вправо
	//Затем меняем цвета
	void delete_case5(node_t* n)
	{
		node_t* s = sibling(n);
		if (s->color == false)
		{
			if ((n == n->parent->left) && (s->right->color == false) && (s->left->color == true))
			{
				s->color = true;
				s->left->color = false;
				rotate_right(s);
			}
			else if ((n == n->parent->right) && (s->left->color == false)
				&& (s->right->color == true))
			{
				s->color = true;
				s->right->color = false;
				rotate_left(s);
			}
		}
		delete_case6(n);
	}
	
	// Если N и S - чёрные, а один (существующий) из Sr и Sl - красный, то поворот вокруг P 
	// Если у S был правый сын, то поворот влево
	// Если левый - вправо
	void delete_case6(node_t* n)
	{
		node_t* s = sibling(n);
		s->color = n->parent->color;
		n->parent->color = false;
		if (n == n->parent->left)
		{
			s->right->color = false;
			rotate_left(n->parent);
		}
		else
		{
			s->left->color = false;
			rotate_right(n->parent);
		}
	}
};

template <typename T>
void rb_tree<T>::insert(T value)
{
	// Вставка в пустое дерево. Достаточно вставить чёрный элемент 

	node_t* node = new node_t;
	node->value = value;
	node->right = nullptr;
	node->left = nullptr;
	node->color = true;

	if (root_ == nullptr)
	{
		node->parent = nullptr;
		node->color = false;
		root_ = node;
	}

	//Иначе происходит поиск места для вставка красного элемента
	else
	{
		node_t* vetka = root_;
		while (vetka != nullptr)
		{
			if (vetka->value > value)
			{
				// если левый сын отсутствует
				if (vetka->left == nullptr)
				{
					vetka->left = node; //выделяем память и заполняем структуру элемента
					node->parent = vetka;
					insert_case1(node); //вызываем функцию для возможной перекраски или поворота
					return;
				}
				else
				{
					vetka = vetka->left;
				};
			}
			else if (vetka->value <= value)
			{
				if (vetka->right == nullptr)
				{
					vetka->right = node;
					node->parent = vetka;
					insert_case1(node);
					return;
				}
				else vetka = vetka->right;
			}
		}
	}
}



template <typename T>
rb_tree<T>::rb_tree(std::initializer_list<T> keys)
{
	root_ = nullptr;

	for (auto i : keys)
	{
		insert(i);
	}
}

template <typename T>
rb_tree<T>::rb_tree()
{
	root_ = nullptr;
}

template <typename T>
rb_tree<T>::~rb_tree()
{
	destroy(root_);
}

template <typename T>
void rb_tree<T>::check_operator(std::ostream& stream, char op, T value)
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
void rb_tree<T>::print(std::ostream& stream, int level, node_t* node)
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
bool rb_tree<T>::find(T value) const
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
void rb_tree<T>::destroy(node_t* node)
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
bool rb_tree<T>::equal(node_t* first, node_t* second) const {
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
auto rb_tree<T>::operator==(rb_tree const & other) const {
	node_t* first = root_; node_t* second = other.root_;
	return (equal(first, second));
}
