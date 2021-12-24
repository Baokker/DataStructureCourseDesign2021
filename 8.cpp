#include<iostream>
#include<iomanip>
#include<string>
#include<functional>
#include <climits>

using namespace std;

//using adjacency matrix
//undirected graph
//using int to represent weight value
template<typename Type,typename Weight = int>
struct Graph
{
	int size; // the reserved space for storage
	int currentSize;
	Type* vertexs;

	Weight INFFlag; // represent that two vertexs are not connected
	Weight** adjacencyMatrix;

	Graph() :size(0), currentSize(0), vertexs(nullptr), INFFlag(Weight()), adjacencyMatrix(nullptr) {}
	Graph(Weight INFFlag, int size = 10) :currentSize(0), size(size), INFFlag(INFFlag) {
		vertexs = new Type[size];
		adjacencyMatrix = new Weight * [size];
		for (int i = 0; i < size; i++) {
			adjacencyMatrix[i] = new Weight[size];
			for (int j = 0; j < size; j++)
				adjacencyMatrix[i][j] = INFFlag;
		}
	}

	void enlarge(int num);// dynamically adjust the matrix size

	int locateVertex(Type vertex); // return the position of vertex(start from 0)(-1 if fails)
	void insertVertex(Type vertex);
	void insertEdge(Type start, Type end, Weight weight);
	void deleteEdge(Type start, Type end);

	bool isEmpty() { return vertexs == nullptr; }
	void print();
	//Prim algorithm
	void prim();
};

template<typename Type, typename Weight>
void Graph<Type, Weight>::enlarge(int num)
{
	//judge if the number is smaller than original
	if (num < size) {
		cerr << "smaller than the original size\n";
		return;
	}

	//copy new
	Type * newVertexs = new Type[num];
	for (int i = 0; i < num; i++) {
		if (i < size)
			newVertexs[i] = vertexs[i];
	}

	Weight** newAdjacencyMatrix = new Weight * [num];
	for (int i = 0; i < num; i++) {
		newAdjacencyMatrix[i] = new Weight[num];
		for (int j = 0; j < num; j++)
			if (i < size && j < size)
				newAdjacencyMatrix[i][j] = adjacencyMatrix[i][j];
			else
				newAdjacencyMatrix[i][j] = INFFlag;
	}

	//delete the old one
	auto oldVertexs = vertexs;
	auto oldAdjacencyMatrix = adjacencyMatrix;
	vertexs = newVertexs;
	adjacencyMatrix = newAdjacencyMatrix;

	delete[]oldVertexs;
	for (int i = 0; i < size; i++) {
		delete[]oldAdjacencyMatrix[i];
	}
	delete[]oldAdjacencyMatrix;

	size = num;
}

template<typename Type, typename Weight>
int Graph<Type, Weight>::locateVertex(Type vertex)
{
	for (int i = 0; i < currentSize; i++) {
		if (vertexs[i] == vertex)
			return i;
	}
	
	return -1;
}

template<typename Type, typename Weight>
void Graph<Type, Weight>::insertVertex(Type vertex)
{
	if (locateVertex(vertex) != -1) {
		cerr << "already in the graph\n";
		return;
	}

	if (currentSize >= size)
		enlarge(2 * size);

	vertexs[currentSize++] = vertex;
}

template<typename Type, typename Weight>
void Graph<Type, Weight>::insertEdge(Type start, Type end, Weight weight)
{
	int startPos = locateVertex(start);
	int endPos = locateVertex(end);

	if (startPos == -1) {
		cerr << "fail to find start vertex\n";
		return;
	}
	if (endPos == -1) {
		cerr << "fail to find end vertex\n";
		return;
	}

	adjacencyMatrix[startPos][endPos] = adjacencyMatrix[endPos][startPos] = weight;
}

template<typename Type, typename Weight>
void Graph<Type, Weight>::deleteEdge(Type start, Type end)
{
	int startPos = locateVertex(start);
	int endPos = locateVertex(end);

	if (startPos == -1) {
		cerr << "fail to find start vertex\n";
		return;
	}
	if (endPos == -1) {
		cerr << "fail to find end vertex\n";
		return;
	}

	adjacencyMatrix[startPos][endPos] = adjacencyMatrix[endPos][startPos] = INFFlag;
}

template<typename Type, typename Weight>
void Graph<Type, Weight>::print()
{
	const int fixedLength = 5;
	cout << setfill(' ') << setw(fixedLength) << " ";

	for (int i = 0; i < currentSize; i++) {
		cout << setfill(' ') << setw(fixedLength) << vertexs[i];
	}

	cout << endl;

	for (int i = 0; i < currentSize; i++) {
		cout << setfill(' ') << setw(fixedLength) << vertexs[i];
		for (int j = 0; j < currentSize; j++) {
			if (adjacencyMatrix[i][j] == INFFlag)
				cout << setfill(' ') << setw(fixedLength) << "INF";
			else
				cout << setfill(' ') << setw(fixedLength) << adjacencyMatrix[i][j];
		}
		cout << endl;
	}

	cout << endl;
}

template<typename Type, typename Weight>
void Graph<Type, Weight>::prim()
{
	//indicate the shortest distance to the MST(minimum spanning tree)
	//set as INF if connections don't exist
	Weight* lowcost = new Weight[currentSize]; // [0..currentSize - 1] 
	//where the shortest distance is directed to
	int* closestIndex = new int[currentSize];
	//whether the vertex is in the MST
	bool* isAdded = new bool[currentSize];
	//record the path for output 
	int* resultIndex = new int[currentSize], pos = 0;
	//unexist flag
	const int unExist = -1;

	for (int i = 0; i < currentSize; i++) { //initialize
		lowcost[i] = INFFlag; //don't have any connect
		closestIndex[i] = unExist;
		isAdded[i] = false; //none in the MST
	}

	//the first vertex is put into the set
	isAdded[0] = true; //[0] is in the MST
	resultIndex[pos++] = 0;//record the first path

	//and firstly update
	for (int i = 0; i < currentSize; i++) {
		if (adjacencyMatrix[0][i] != INFFlag) {
			closestIndex[i] = 0;//[0] is closest to this vertex now
			lowcost[i] = adjacencyMatrix[0][i];//<i,0> is update as the shortest distance between them
		}
	}

	auto checkifAllAdded = [&](bool* value)->bool { //lambda function to check if all elements are in MST
		for (int i = 0; i < currentSize; i++)
			if (value[i] == false) return false;
		return true;
	};

	while (checkifAllAdded(isAdded) == false) { //until all nodes are added into the set
		//pick up a node that has the minimun distance with the nodes in the set
		int minIndex = unExist;
		for (int i = 0; i < currentSize; i++) {	
			if (!isAdded[i] && (minIndex == unExist || lowcost[minIndex] > lowcost[i])) {
				minIndex = i;
			}
		}

		//if fail to find the minIndex
		if (minIndex != unExist) {
			isAdded[minIndex] = true; //[minIndex] in MST now
			resultIndex[pos++] = minIndex;//append minIndex to path
		}
		else {//can't find the result  
			cerr << "unable to generate minimum spanning tree\n";
			return;
		}

		//update the lowcost[] and closestIndex[] after adding a new node
		for (int j = 0; j < currentSize; j++) {
			if (adjacencyMatrix[minIndex][j] < lowcost[j]) { //if there is a lower value
				closestIndex[j] = minIndex;
				lowcost[j] = adjacencyMatrix[minIndex][j];
			}
		}
	}

	//print the result
	for (int i = 0; i < currentSize - 1; i++) {
		cout << vertexs[resultIndex[i]] << "-" << adjacencyMatrix[resultIndex[i]][resultIndex[i + 1]] << "->" << vertexs[resultIndex[i + 1]] << "   ";
	}
	cout << endl;

	//delete
	delete[]lowcost;
	delete[]closestIndex;
	delete[]isAdded;
	delete[]resultIndex;
}

int getIntValue(const function<bool(int)>& judgeFunc) {//for freely customizing the int value you want
	int value;
	while (true)
	{
		cin >> value;
		if (cin.fail()) {
			cerr << "input error\n";
			cin.clear();
			cin.ignore(1024, '\n');
		}
		else if (!judgeFunc(value)) {
			cerr << "dont meet the command\n";
		}
		else
			break;
	}

	return value;
}


int main() {
	Graph<string> graph(INT_MAX);

	cout << "本程序默认设INF（不可能的值）为INT_MAX，是否要修改？\n"
		"（1表示修改，0表示不修改）\n";

	int ifChangeINF = getIntValue([](int value)->bool {return value == 0 || value == 1; });

	if (ifChangeINF) {
		int newINFFlag = getIntValue([](int value)->bool {return true; });
		graph.INFFlag = newINFFlag;
	}

	const int maxChoiceNum = 6;
	bool exitFlag = false;

	while (true) {
		cout << "1-插入顶点 2-插入边 3-删除边 4-打印图 5-prim算法求最小生成树 0-退出\n";
		int choice = getIntValue([&](int value)->bool {return value >= 0 && value < maxChoiceNum; });

		switch (choice)
		{
		case 0: {
			exitFlag = true;
			break;
		}
		case 1: {
			cout << "输入顶点个数：\n";
			int vertexNum = getIntValue([](int value)->bool { return value > 0; });
			string vertexName;
			for (int i = 0; i < vertexNum; i++) {
				cout << "输入第" << i + 1 << "个顶点的名字\n";
				cin >> vertexName;
				graph.insertVertex(vertexName);
			}
			break;
		}

		case 2: {
			cout << "输入边的个数：\n";
			int edgeNum = getIntValue([](int value)->bool { return value > 0; }), cost;
			string start, end;

			for (int i = 0; i < edgeNum; i++) {
				cout << "输入第" << i + 1 << "条边的信息（start边，end边，cost权值）\n（ps 此程序表示无向图\n";
				cin >> start >> end;
				cost = getIntValue([](int value)->bool { return value > 0; });
				graph.insertEdge(start, end, cost);
			}
			break;
		}

		case 3: {
			string start, end;
			cout << "输入删除边的信息（start边，end边）\n（ps 此程序表示无向图\n";
			cin >> start >> end;
			graph.deleteEdge(start, end);
			break;
		}
		case 4: {
			if (!graph.isEmpty())
				graph.print();
			else
				cerr << "图仍为空！\n";
			break;
		}

		case 5: {
			if (!graph.isEmpty())
				graph.prim();
			else
				cerr << "图仍为空！\n";
			break;
		}

		default:
			break;
		}

		if (exitFlag) break;
	}
}