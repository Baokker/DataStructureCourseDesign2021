#include<iostream>
#include<string>

using namespace std;

//two options
constexpr int FIRSTCHILD = 0;
constexpr int NEXTSIBLING = 1;

template <typename Type>
struct TreeNode
{
	Type val;
	TreeNode<Type>* FirstChild;
	TreeNode<Type>* NextSibling;

	TreeNode<Type>() : val(Type()), FirstChild(nullptr), NextSibling(nullptr) {}
	TreeNode<Type>(Type val, TreeNode<Type>* FirstChild = nullptr, TreeNode<Type>* NextSibling = nullptr) :
		val(val), FirstChild(FirstChild), NextSibling(NextSibling) {}
};

template <typename Type>
struct Tree
{
	TreeNode<Type>* root = nullptr, * cursor = nullptr;

	~Tree() { removeSubTree(root); delete root; }
	void create(Type val);
	TreeNode<Type>* find(Type val);
	void insert(TreeNode<Type>* position, Type val, int tag = FIRSTCHILD);
	bool revise(TreeNode<Type>* pre, Type val);
	void removeChildren(TreeNode<Type>* position);
	void removeSubTree(TreeNode<Type>* position); // subtree beneath the pos

	// the target should be nullptr at first
	void inOrderFind(Type val, TreeNode<Type>* &target, TreeNode<Type>* &src = nullptr);

	void inOrder(TreeNode<Type>* node);
	void PreOrder(TreeNode<Type>* node);
	void PostOrder(TreeNode<Type>* node);

	void ReturntoRoot() { cursor = root; }
};

class Family_Tree
{
	Tree<string>* tree;
public:
	Family_Tree() { tree = new Tree<string>; }

	Tree<string>* GetTree() { return tree; }

	void create();

	TreeNode<string>* find() ;
	bool insert();
	bool revise();
	void remove_family();
	void display();

	bool empty() { return tree->root == nullptr; }
};

template<typename Type>
void Tree<Type>::create(Type val)
{
	root = cursor = new TreeNode<Type>(val);
}

template<typename Type>
TreeNode<Type>* Tree<Type>::find(Type val)
{
	TreeNode<Type>* result = nullptr;
	inOrderFind(val, result, root);
	return result;
}

template<typename Type>
void Tree<Type>::insert(TreeNode<Type>* position, Type val, int tag)
{
	if (!position) return;
	
	if ((tag == FIRSTCHILD)) //if insert the element as position's child
	{
		if (position->FirstChild == nullptr)
			position->FirstChild = new TreeNode<Type>(val);
		else
			insert(position->FirstChild, val, NEXTSIBLING);
	}
	else //if insert the element as position's sibling
	{
		if (position->NextSibling == nullptr)
			position->NextSibling = new TreeNode<Type>(val);
		else
			insert(position->NextSibling, val, tag);
	}	
}

template<typename Type>
bool Tree<Type>::revise(TreeNode<Type>* pre, Type val)
{
	if (!pre) return false;

	pre->val = val;
	return true;
}

template<typename Type>
void Tree<Type>::removeChildren(TreeNode<Type>* position)
{
	removeSubTree(position->FirstChild);
	position->FirstChild = nullptr;
}

template<typename Type>
void Tree<Type>::removeSubTree(TreeNode<Type>* position)///////////////////////////
{
	if (!position) return;
	else if (!position->FirstChild && !position->NextSibling)
	{
		delete position;
		position = nullptr;
	}
	else
	{
		removeSubTree(position->FirstChild);
		removeSubTree(position->NextSibling);
		position->FirstChild = position->NextSibling = nullptr;
		delete position;
	}
}

template<typename Type>
void Tree<Type>::inOrderFind(Type val, TreeNode<Type>*& target, TreeNode<Type>*& src)// !!!!!! *&表示对指针引用！
{
	if (!src) 
		return;

	inOrderFind(val, target, src->FirstChild);

	if (src->val == val)
	{
		target = src;
		return;
	}

	inOrderFind(val, target, src->NextSibling);
}

template<typename Type>
void Tree<Type>::inOrder(TreeNode<Type>* node)
{
	if (!node) return;

	inOrder(node->FirstChild);
	if (node)
		cout << node->val << endl;
	inOrder(node->NextSibling);
}

template<typename Type>
void Tree<Type>::PreOrder(TreeNode<Type>* node)
{
	if (!node) return;

	if (node)
		cout << node->val << endl;
	inOrder(node->FirstChild);
	inOrder(node->NextSibling);
}

template<typename Type>
void Tree<Type>::PostOrder(TreeNode<Type>* node)
{
	if (!node) return;

	inOrder(node->FirstChild);
	inOrder(node->NextSibling);
	if (node)
		cout << node->val << endl;
}

void Family_Tree::create()
{
	cout << "首先建立起家谱\n";
	cout << "输入祖先名字：\n";
	string ancestor;
	cin >> ancestor;
	tree->create(ancestor);
}

TreeNode<string>* Family_Tree::find()
{
	cout << "输入要寻找的家人的名字：\n";
	string name;
	cin >> name;
	TreeNode<string>* result = tree->find(name);
	if (result)
	{
		cout << "有的子！\n";
	}
	else
		cout << "sorry~真的没有呢\n";

	return result;
}

bool Family_Tree::insert()
{
	if (empty())
	{
		create();
		return true;
	}

	cout << "请选择通过parent建立/通过sibling建立\n"
		<< "前者输入0，后者输入1\n";
	int num;
	while (true)
	{
		cin >> num;
		if (cin.fail())
		{
			cout << "输入有误 重新输入子\n";
			cin.clear();
			cin.ignore(1024, '\n');
			continue;
		}
		else if (num != 0 && num != 1)
		{
			cout << "输入有误 重新输入子\n";
			continue;
		}
		else
			break;
	}

	if (num == FIRSTCHILD)
		cout << "请输入要建立家庭的人的姓名：\n";
	else
		cout << "请输入任一兄弟姐妹的姓名：\n";

	string name;
	cin >> name;
	TreeNode<string>* result = tree->find(name);
	if (!result)
	{
		cout << "查找失败！\n";
		return false;
	}
	else if (result == tree->root && num == NEXTSIBLING)
	{
		cout << "无法以根节点建立兄弟节点！\n";
		return false;
	}

	cout << "请输入添加的数量\n";
	int count;
	while (true)
	{
		cin >> count;
		if (cin.fail())
		{
			cout << "输入有误 重新输入子\n";
			cin.clear();
			cin.ignore(1024, '\n');
			continue;
		}
		else if (count <= 0)
		{
			cout << "输入有误 重新输入子\n";
			continue;
		}
		else
			break;
	}

	cout << "请输入加入的新家人姓名（以空格区分）（ex:aa bb cc）：\n";

	for (int i = 0; i < count; i++) {
		string new_name;
		cin >> new_name;

		tree->insert(result, new_name, num);
	}

	if (num == FIRSTCHILD)
	{
		tree->cursor = result->FirstChild;
		cout << result->val << "的子孙为：";
		while (tree->cursor != nullptr)
		{
			cout << tree->cursor->val << " ";
			tree->cursor = tree->cursor->NextSibling;
		}
		tree->ReturntoRoot();
		cout << endl;
	}
	return true;
}

bool Family_Tree::revise()
{
	cout << "请输入要修改的家人的姓名：\n";
	string name;
	cin >> name;
	TreeNode<string>* result = tree->find(name);
	if (!result)
	{
		cerr << "查找失败！\n";
		return false;
	}

	cout << "请输入修改后的家人姓名：\n";
	string new_name;
	cin >> new_name;
	result->val = new_name;
	return true;
}

void Family_Tree::remove_family()
{
	cout << "请输入要解散的家人的头头（最顶端）的姓名：\n";
	string name;
	cin >> name;
	TreeNode<string>* result = tree->find(name);
	if (!result)
	{
		cout << "查找失败！\n";
		return;
	}


	if (result == tree->root) {
		cout << "? 把自己祖宗都给扒了？（删除为根节点）\n";
		return;
	}

	tree->cursor = result->FirstChild;
	cout << result->val << "的第一代子孙为：";
	while (tree->cursor != nullptr)
	{
		cout << tree->cursor->val << " ";
		tree->cursor = tree->cursor->NextSibling;
	}
	tree->ReturntoRoot();
	cout << endl;

	tree->removeChildren(result);
}

void Family_Tree::display()
{
	cout << "ps 目前仅支持查询第一代子孙\n请输入查询名字：";
	string name;
	cin >> name;

	TreeNode<string>* result = tree->find(name);

	if (!result)
	{
		cerr << "查找失败！\n";
		return;
	}

	tree->cursor = result->FirstChild;
	cout << result->val << "的子孙为：";
	while (tree->cursor != nullptr)
	{
		cout << tree->cursor->val << " ";
		tree->cursor = tree->cursor->NextSibling;
	}
	tree->ReturntoRoot();

	cout << endl;
}

int input() // 0~
{
	int num;
	while (true)
	{
		cout << "1-展示 2-添加 3-查找 4-修改 5-删除 0-退出\n"
			<< "请输入操作码：\n";
		cin >> num;
		if (cin.fail())
		{
			cin.clear();
			cin.ignore(1024, '\n');
			cout << "输入有误！重新输入！\n";
			continue;
		}
		else if (num < 0 || num > 5)
		{
			cout << "操作码听不懂子~重新输入吧~\n";
			continue;
		}
		else
			break;
	}
	
	return num;
}

int main()
{
	Family_Tree family;
	family.create();
	while (true)
	{
		int choice = input();
		if (!choice)
		{
			cout << "拜拜嘞\n";
			break;
		}
		switch (choice)
		{
			case 1:
				family.display();
				break;
			case 2:
				family.insert();
				break;
			case 3:
				family.find();
				break;
			case 4:
				family.revise();
				break;
			case 5:
				family.remove_family();
				break;
		}
	}

	return 0;
}
