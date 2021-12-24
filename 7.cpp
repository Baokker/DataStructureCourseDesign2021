#include<iostream>
#include<string>

using namespace std;

template<typename Type>
struct BinNode
{
	Type val;
	BinNode* left, * right;

	BinNode() :val(Type()), left(nullptr), right(nullptr) {}
	BinNode(Type val) :val(val), left(nullptr), right(nullptr) {}
	BinNode(Type val, BinNode* left, BinNode* right) :val(val), left(left), right(right) {}
};

template<typename Type>
struct BinTree
{
	BinNode<Type>* root;

	~BinTree() { clear(root); }
	//recursive methods
	void inOrder(BinNode<Type>* pos);
	void PreOrder(BinNode<Type>* pos);
	void PostOrder(BinNode<Type>* pos);

	// 迭代方式实现遍历
	void inOrder(); 
	void PreOrder();
	void PostOrder();

	void clear(BinNode<Type>* pos);
};

template<typename Type>
struct ListNode
{
	Type val;
	ListNode* next;

	ListNode(Type val) :val(val), next(nullptr) {}
	ListNode(Type val, ListNode* next) :val(val), next(next) {}
	ListNode() :val(Type()), next(nullptr) {}
};

template<typename Type>
struct DListNode // 双向链表节点 为非递归后序遍历而写
{
	Type val;
	DListNode* next, * pre;

	DListNode(Type val) :val(val), next(nullptr), pre(nullptr) {}
	DListNode(Type val, DListNode* pre, DListNode* next) :val(val), next(next), pre(pre) {}
	DListNode() :val(Type()), next(nullptr), pre(nullptr) {}
};

template<typename Type>
struct DList // 双向链表 为非递归后序遍历而写
{
	DListNode<Type>* head, * tail;
	DList();
	~DList();

	bool empty() { return head == tail; }
	void append(Type val);
	void del_last();

};

template<typename Type>
struct Stack
{
	ListNode<Type>* Top;
	int Length;

	Stack() :Top(nullptr), Length(0) {}
	bool empty() { return Length == 0; }
	void push(const Type& val); // 非常量引用的表达式必须为左值 所以加上const
	bool pop();
	bool pop(Type& return_val);
	Type* top() { return Top->val; }
	int length() { return Length; }
};

class Expression
{
	private:
		BinTree<string>* tree;
	public:
		Expression(string& formula);

		void Affix_Expression_to_Tree(BinNode<string>*& pos, string formula);//将表达式转化为子树

		void toPolishNotation();//前缀
		void toAffixNotation();//中缀
		void toReversePolishNotation();//后缀

		void inOrder() { tree->inOrder(); }
		void PreOrder() { tree->PreOrder(); }
		void PostOrder() { tree->PostOrder(); }
};

template<typename Type>
DList<Type>::DList()
{
	head = new DListNode<Type>();
	tail = head;
}

template<typename Type>
DList<Type>::~DList()
{
	while (head)
	{
		DListNode<Type>* temp = head;
		head = head->next;
		delete temp;
	}
}

template<typename Type>
void DList<Type>::append(Type val) // from the last
{
	DListNode<Type>* new_data = new DListNode<Type>(val, tail, nullptr);
	tail->next = new_data;
	tail = tail->next;
}

template<typename Type>
void DList<Type>::del_last()
{
	if (!empty())
	{
		tail = tail->pre;
		delete tail->next;
		tail->next = nullptr;
	}
}

template<typename Type>
void BinTree<Type>::inOrder(BinNode<Type>* pos)
{
	if (!pos) return;

	inOrder(pos->left);
	cout << pos->val << " ";
	inOrder(pos->right);
}

template<typename Type>
void BinTree<Type>::PreOrder(BinNode<Type>* pos)
{
	if (!pos) return;

	cout << pos->val << " ";
	PreOrder(pos->left);
	PreOrder(pos->right);
}

template<typename Type>
void BinTree<Type>::PostOrder(BinNode<Type>* pos)
{
	if (!pos) return;

	PostOrder(pos->left);
	PostOrder(pos->right);
	cout << pos->val << " ";
}

template<typename Type>
void BinTree<Type>::inOrder()
{
	Stack<BinNode<Type>*> stack;
	BinNode<Type>* cursor = root;

	while (!stack.empty() || cursor)
	{
		while (cursor)
		{
			stack.push(cursor);
			cursor = cursor->left;
		}

		stack.pop(cursor);

		cout << cursor->val << " ";

		cursor = cursor->right;
	}
}

template<typename Type>
void BinTree<Type>::PreOrder()
{
	Stack<BinNode<Type>*> stack;
	BinNode<Type>* cursor = nullptr;

	if (root) stack.push(root);
	
	// left later in and earlier out
	while (!stack.empty())
	{
		stack.pop(cursor);

		cout << cursor->val << " ";

		if (cursor->right) stack.push(cursor->right);
		if (cursor->left) stack.push(cursor->left);
	}
}

template<typename Type>
void BinTree<Type>::PostOrder()
{
	Stack<BinNode<Type>*> stack;
	DList<Type> result;
	BinNode<Type>* cursor = nullptr;

	if (root) stack.push(root);

	// right later in and earlier out
	while (!stack.empty())
	{
		stack.pop(cursor);

		result.append(cursor->val);

		if (cursor->left) stack.push(cursor->left);
		if (cursor->right) stack.push(cursor->right);
	}

	DListNode<Type>* result_cursor = result.tail;

	while (result_cursor != result.head)
	{
		cout << result_cursor->val << " ";
		result_cursor = result_cursor->pre;
	}
}

template<typename Type>
void BinTree<Type>::clear(BinNode<Type>* pos)
{
	if (!pos) return;
	else if (!pos->left && !pos->right)
	{
		delete pos;
		pos = nullptr;
	}
	else
	{
		clear(pos->left);
		clear(pos->right);
		pos->left = pos->right = nullptr;
	}
}

template<typename Type>
void Stack<Type>::push(const Type& val)
{
	if (empty())
		Top = new ListNode<Type>(val);
	else
	{
		ListNode<Type>* new_data = new ListNode<Type>(Type(val), Top);
		Top = new_data;
	}

	Length++;
}

template<typename Type>
bool Stack<Type>::pop()
{
	if (empty()) return false;

	ListNode<Type>* del_data = Top;
	Top = Top->next;
	delete del_data;
	Length--;
	return true;
}

template<typename Type>
bool Stack<Type>::pop(Type& return_val)
{
	if (empty()) return false;

	ListNode<Type>* del_data = Top;
	return_val = del_data->val;
	Top = Top->next;
	delete del_data;
	Length--;
	return true;
}

//之前想用栈 但是后来有了更好的方法
//Expression::Expression(string& formula)
//{
//	Stack<int>nums; Stack<char> operators;
//	int n = formula.size();
//	for (int i = 0; i < n; i++)
//	{
//		if (formula[i] == '+')
//		{
//			if (i == 0)
//				nums.push(GetNum(formula, i, n - i));
//			else
//				operators.push('+');
//		}
//		else if (formula[i] == '-')
//		{
//			if (i == 0 || !isdigit(formula[i - 1]))
//				nums.push(-GetNum(formula, i, n - i));
//			else
//				operators.push('-');
//		}
//		else if (formula[i] == '(')
//			operators.push('(');
//		else if ()
//	}
//}

Expression::Expression(string& formula)
{
	tree = new BinTree<string>;
	Affix_Expression_to_Tree(tree->root, formula);
}

void Expression::toPolishNotation()
{
	tree->PreOrder(tree->root);
}

void Expression::toAffixNotation()
{
	tree->inOrder(tree->root);
}

void Expression::Affix_Expression_to_Tree(BinNode<string>*& pos, string formula)
{
	while (formula[formula.size() - 1] == ')' && formula[0] == '(') //erase the brackets
	{
		formula.erase(0, 1);
		formula.erase(formula.size() - 1, 1);
	}

	for (int i = formula.size() - 1; i >= 0; i--)
	{
		if (formula[i] == ')') //skip the brackets
		{
			while (formula[i] != '(')
				i--;
		}
		//check if the operator is + or -(avoid conditions like negative numbers)
		else if (formula[i] == '+' || (formula[i] == '-' && i != 0 && (isdigit(formula[i - 1]) || formula[i - 1] == ')') || (formula[i] == '-' && i != 0 && (isdigit(formula[i - 1]) || formula[i - 1] == '('))))
		{
			pos = new BinNode<string>(string(1, formula[i])); // "+"*1 or "-"*1
			Affix_Expression_to_Tree(pos->left, formula.substr(0, i));
			Affix_Expression_to_Tree(pos->right, formula.substr(i + 1, formula.size() - i));
			return;
		}
	}

	for (int i = formula.size() - 1; i >= 0; i--)
	{
		if (formula[i] == ')')
		{
			while (formula[i] != '(')
				i--;
		}
		//check if the operator is * or /
		else if (formula[i] == '*' || formula[i] == '/')
		{
			pos = new BinNode<string>(string(1, formula[i]));
			Affix_Expression_to_Tree(pos->left, formula.substr(0, i));
			Affix_Expression_to_Tree(pos->right, formula.substr(i + 1, formula.size() - i));
			return; // !!!
		}
	}

	pos = new BinNode<string>(formula);
}

void Expression::toReversePolishNotation()
{
	tree->PostOrder(tree->root);
}

int main()
{
	cout << "请输入表达式：";
	string expression;
	cin >> expression;
	Expression result(expression);

	cout << "前缀表达式为：";
	result.toPolishNotation();

	cout << endl;
	cout << "中缀表达式为：" << expression << endl;

	cout << "后缀表达式为：";
	result.toReversePolishNotation();
	cout << endl;

	cout << "接下来非递归方式输出前缀中缀后缀表达式：\n";
	result.PreOrder();
	cout << endl;
	result.inOrder();
	cout << endl;
	result.PostOrder();
	cout << endl;
	return 0;
}