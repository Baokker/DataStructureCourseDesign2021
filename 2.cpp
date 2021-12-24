#include<iostream>
#include<iomanip>

using namespace std;

struct LinkNode
{
	int id;
	bool isdead;
	LinkNode* next;

	LinkNode():id(0),next(nullptr), isdead(false){}
	LinkNode(int x) :id(x), next(nullptr), isdead(false) {}
};

class Joseph_Ring
{
	public:
		LinkNode* head;
		int N, S, M, K;

		Joseph_Ring(int N, int S, int M, int K);
		~Joseph_Ring();
		void run();
};

Joseph_Ring::Joseph_Ring(int N, int S, int M, int K) :N(N), S(S), M(M), K(K), head(nullptr)
{
	head = new LinkNode(1);
	LinkNode* cursor = head;

	for (int i = 2; i <= N; i++)
	{
		LinkNode* passenger = new LinkNode(i);
		cursor->next = passenger;
		cursor = cursor->next;
	}

	cursor->next = head;
}

Joseph_Ring::~Joseph_Ring()
{
	LinkNode* cursor = head;
	while (cursor->next != head) //iterate
	{
		LinkNode* temp = cursor;
		cursor = cursor->next;
		delete temp;
	}
}

void Joseph_Ring::run()
{
	// reach the position
	LinkNode* cursor = head;
	while (cursor->id != S)
	{
		cursor = cursor->next;
	}

	// die
	int count = 0;
	for (int i = 1; i <= (N - K); i++)
	{
		while (true)
		{
			if (!cursor->isdead)
			{
				count++;
				if (count == M)
				{
					cout << "第" << left << setw(4) << setfill(' ') << i << "个死者的位置是" << cursor->id << endl;
					cursor->isdead = true;
					count = 0;
					break;
				}

			}

			cursor = cursor->next;
		}
	}

	// the alive
	cout << "剩余" << left << setw(4) << setfill(' ') << K << "个生存者 其序号分别为（开始为1）：" << endl;

	cursor = head;
	while (true)
	{
		if (!cursor->isdead)
			cout << cursor->id << " ";
		
		if (cursor->next == head)
			break;
		else
			cursor = cursor->next;
	}

	cout << endl << endl;

	return;
}


int main()
{
	int	N, S, M, K;

	while (true)
	{
		cout << "约瑟夫环问题\n"
			<< "人们站在一个等待被处决的圈子里。"
			<< "计数从圆圈中的指定点开始，并沿指定方向围绕圆圈进行。"
			<< "在跳过指定数量的人之后，处刑下一个人。"
			<< "对剩下的人重复该过程，从下一个人开始，朝同一方向跳过相同数量的人，直到只剩下一个人，并被释放。（Wikipedia）\n\n"
			<< "请输入N S M K（表示共有N个人 从第S号开始（初始为1） 报到M的人出列 直到剩下K个人为止\n"
			<< "N为0表示退出\n";
		
		while (true)
		{
			cin >> N >> S >> M >> K;
			if (cin.fail())
			{
				cerr << "输入数据有误！请重新输入子！\n";
				cin.clear();
				cin.ignore(1024, '\n');
				continue;
			}
			else
			{
				break;
			}
		}

		if (N == 0)
			break;

		Joseph_Ring joseph_ring(N, S, M, K);
		joseph_ring.run();
	}

	return 0;
}
