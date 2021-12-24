#include<iostream>

using namespace std;

//easy to read
constexpr int SIZE = 7;
constexpr int PASSABLE = 1;
constexpr bool VISITED = true;
constexpr bool UNVISITED = false;

//point
struct Point
{
	int x, y;
	Point(int x, int y) :x(x), y(y) {}
	Point() :x(0), y(0) {}
	
	// overload the compare operator
	bool operator==(Point point) { return point.x == x && point.y == y; } 
	bool operator!=(Point point) { return point.x != x || point.y != y; }
};

ostream& operator<<(ostream& output, Point point) 
// !类内类外的运算符重载区别！
// 类内默认调用this指针 类外/友元则不会
// 为确保<< >>能正常使用 只能采取类内友元/类外实现的方法
{
	output << "<" << point.x << "," << point.y << ">";
	return output;
}

// rewrite list
template<typename Type>
struct ListNode
{
	Type val;
	ListNode* next;

	ListNode(Type val) :val(val), next(nullptr) {}
	ListNode() :val(Type()), next(nullptr) {}
};

template<typename Type>
class List
{
public:
	ListNode<Type>* head, *tail;
	int length;
	List():length(0)
	{
		head = new ListNode<Type>;
		head->next = nullptr;
		tail = head;
	}
	~List()
	{
		while (head != nullptr)
		{
			ListNode<Type>* temp = head;
			head = head->next;
			delete temp;
		}
	}

	void add(const Type& val); // add from the last
	bool empty() { return length == 0; }
	void del_last();

	// specific
	void print();
};


//dfs
void DFS_For_Maze(Point Start, Point End, List<Point>& list, int n, int Maze[SIZE][SIZE], bool flag[SIZE][SIZE]);

template<typename Type>
void List<Type>::add(const Type& val)
{
	ListNode<Type>* newData = new ListNode<Type>(val);
	tail->next = newData;
	tail = newData;
	length++;
}

template<typename Type>
void List<Type>::del_last()
{
	if (empty()) //judge first
	{
		cerr << "链表为空！\n";
		return;
	}

	ListNode<Type>* temp = head;
	while (temp->next != tail) //locate temp in the position in front of tail
		temp = temp->next;

	delete tail;
	tail = temp;
	tail->next = nullptr;
	return;
}

template<typename Type>
void List<Type>::print()
{
	ListNode<Type>* cursor = head;
	while (cursor->next != tail)
	{
		cursor = cursor->next;
		cout << cursor->val << "->";
	}

	cout << cursor->next->val;
}

int main()
{
	// store the maze
	// you can define the maze and src/dest yourself
	int Maze[SIZE][SIZE] =
	{   {0,0,0,0,0,0,0},
		{0,1,0,0,0,0,0},
		{0,1,0,0,0,0,0},
		{0,1,1,1,0,0,0},
		{0,0,0,1,1,1,0},
		{0,0,0,0,0,1,0},
		{0,0,0,0,1,1,0} };

	bool flag[SIZE][SIZE];
	Point Start(1, 1), End(6, 4); 
	
	List<Point> list;
	list.add(Start);

	for (int i = 0; i < SIZE; i++)
		for (int j = 0; j < SIZE; j++)
			flag[i][j] = UNVISITED; //initialize

	cout << "迷宫地图\n    ";
	for (int i = 0; i < SIZE; i++)
		cout << i << "   ";
	cout << endl << endl;
	for (int i = 0; i < SIZE; i++)
	{
		cout << i << "   ";
		for (int j = 0; j < SIZE; j++)
		{
			cout << (Maze[i][j] == 1 ? "X" : "O") << "   ";
		}
		cout << endl;
	}

	cout << "Path:\n";
	DFS_For_Maze(Start, End, list, SIZE, Maze, flag);
	
	return 0;
}

void DFS_For_Maze(Point Start, Point End, List<Point>& list,int n, int Maze[SIZE][SIZE], bool flag[SIZE][SIZE])
{
	if (Start == End) // finish condition
	{
		list.print(); // print the answer
		return;
	}
	else // search in four directions
	{
		// up
		if (Start.y - 1 >= 0 && Maze[Start.x][Start.y - 1] == PASSABLE && flag[Start.x][Start.y - 1] == UNVISITED)
		{
			Point newStart(Start.x, Start.y - 1);
			flag[Start.x][Start.y - 1] = VISITED; // avoid visit over and over again
			list.add(newStart);
			DFS_For_Maze(newStart, End, list, n, Maze, flag);
			list.del_last();
			flag[Start.x][Start.y - 1] = UNVISITED; // reset
		}
		// right
		if (Start.x + 1 < n && Maze[Start.x + 1][Start.y] == PASSABLE && flag[Start.x + 1][Start.y] == UNVISITED)
		{
			Point newStart(Start.x + 1, Start.y);
			flag[Start.x + 1][Start.y] = VISITED;
			list.add(newStart);
			DFS_For_Maze(newStart, End, list, n, Maze, flag);
			list.del_last();
			flag[Start.x + 1][Start.y] = UNVISITED;
		}
		// down
		if (Start.y + 1 < n && Maze[Start.x][Start.y + 1] == PASSABLE && flag[Start.x][Start.y + 1] == UNVISITED)
		{
			Point newStart(Start.x, Start.y + 1);
			flag[Start.x][Start.y + 1] = VISITED;
			list.add(newStart);
			DFS_For_Maze(newStart, End, list, n, Maze, flag);
			list.del_last();
			flag[Start.x][Start.y + 1] = UNVISITED;
		}
		// left
		if (Start.x - 1 >= 0 && Maze[Start.x - 1][Start.y] == PASSABLE && flag[Start.x - 1][Start.y] == UNVISITED)
		{
			Point newStart(Start.x - 1, Start.y);
			flag[Start.x - 1][Start.y] = VISITED;
			list.add(newStart);
			DFS_For_Maze(newStart, End, list, n, Maze, flag);
			list.del_last();
			flag[Start.x - 1][Start.y] = UNVISITED;
		}
	}
}
