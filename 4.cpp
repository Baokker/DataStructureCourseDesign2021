#include<iostream>

using namespace std;

constexpr bool QUEEN = true;
constexpr bool CHESS = false;

struct chessBoard
{
    // Queen -> true
    // Chess -> false
    bool** data = nullptr;
    int n;

    chessBoard(int n):n(n)
    {
        //can't allocate two-dimensional array directly
        data = new bool*[n]; 
        for (int i = 0; i < n; i++)
        {
            data[i] = new bool[n];
            for (int j = 0; j < n; j++)
                data[i][j] = CHESS;
        }
    }

    ~chessBoard()
    {
        //can't free two-dimensional array directly
        for (int i = 0; i < n; i++)
        {
            delete data[i];
            data[i] = nullptr;
        }

        delete[]data;
    }

    void SetQueen(int x, int y) 
    {
        data[x][y] = QUEEN;
    }

    void SetChess(int x, int y)
    {
        data[x][y] = CHESS;
    }

    bool GetData(int x, int y) { return data[x][y]; }

};

ostream& operator<<(ostream& output, chessBoard& chessboard) 
{
    for (int i = 0; i < chessboard.n; i++)
    {
        for (int j = 0; j < chessboard.n; j++)
            if (chessboard.GetData(i, j) == QUEEN)
                output << "Q ";
            else
                output << "X ";

        output << endl;
    }
    output << endl;
    return output;
}

class N_Queens
{
public:
	int n;
    int num_solutions; // the number of solutions

	N_Queens(int n) :n(n),num_solutions(0) {}
	void solve();
	void backTracking(int row,chessBoard& chessboard);
    bool isValid(chessBoard& chessboard, int row, int col);
};

void N_Queens::solve()
{
	if (n < 1)
	{
		cerr << "don't meet the command!\n";
		return;
	}

	chessBoard board(n);
	backTracking(0, board);
}

void N_Queens::backTracking(int row, chessBoard& chessboard)
{
	if (row == n)
	{
		cout << chessboard;
		num_solutions++;
		return;
	}

	for (int i = 0; i < n; i++)
	{
		chessboard.SetQueen(row, i);
		if (isValid(chessboard, row, i))
			backTracking(row + 1, chessboard);
		chessboard.SetChess(row, i); // reset back
	}
}

bool N_Queens::isValid(chessBoard& chessboard, int row, int col)
{
	//as the Queen is set by line in the backTracking function
	//hence every Queen is bound to be fixed in different lines
	//it's no need to check whether there are two or more queen in one line
	//as well as the lines below the line where current queen is in
	//(you don't even put any queen in the beneath)

	for (int i = row - 1; i >= 0; i--) // col
	{
		if (chessboard.GetData(i,col) == QUEEN)
			return false;
	}

	for (int i = row - 1, j = col - 1; i >= 0 && j >= 0; i--, j--) //左上对角线
	{
		if (chessboard.GetData(i,j) == QUEEN)
			return false;
	}

	for (int i = row - 1, j = col + 1; i >= 0 && j < n; i--, j++) //右上对角线
	{
		if (chessboard.GetData(i,j) == QUEEN)
			return false;
	}

	return true;
}

int main()
{
	while (true)
	{
		cout << "N皇后问题：在NxN的棋盘中放入n个皇后\n"
			<< "要求不能吃到对方（不能同行/同列/同对角线）\n"
			<< "输入n以获取所有方案(-1表示退出）\n";

		int n = -1;//get the size of the chessboard
		while (true)
		{
			cin >> n;
			if (cin.fail())
			{
				cerr << "重新输入！有错误！\n";
				cin.clear();
				cin.ignore(1024, '\n');
				continue;
			}
			else if (n < 0 && n != -1)
			{
				cout << "n不符合要求！重新输入子！\n";
				continue;
			}
			else
				break;
		}

		if (n == -1)
			break;

		N_Queens n_queens(n);
		n_queens.solve();
		cout << "共有" << n_queens.num_solutions << "种方案\n\n";
	}
}


