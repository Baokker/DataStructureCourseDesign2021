#include<iostream>
#include<fstream> // for file
#include<string>
#include <cstring> // memset

using namespace std;

//rewrite
//the KMP method can't completely solve the fully match problem


constexpr int ALPHABET_SIZE = 26;
struct TrieNode
{
	bool isWord;
	int count = 0; // occurence frequency
	TrieNode* next[ALPHABET_SIZE];

	TrieNode() :isWord(false),count(0)
	{
		memset(next, 0, sizeof(next));
	}
};

int toOrder(char ch); // convert [A..Z] into [0..25] order

struct TrieTree
{
	TrieNode* root;

	TrieTree() :root(nullptr) {}
	TrieTree(string text);
	~TrieTree();
	void insert(string word);
	int search(string key);
	void clear(TrieNode* pos);
};

int main()
{
	cout << "关键字检索系统\n"
		<< "请输入您想用于保存的文件名:\n";

	string filename;
	cin >> filename;
	cin.get(); // 读取换行

	// 发现读取也很难呢..
	ofstream fout(filename, ios::out);
	if (!fout)
	{
		cerr << "输出文件打开失败！\n";
		exit(1);
	}

	cout << "请输入一段英文（换行表示结束）（不支持数字）\n";

	char temp;
	while (true)
	{
		temp = cin.get();
		if (temp == '\n' || temp == '\r')
			break;
		else
			fout << temp;
	}
	fout.close();

	cout << "文件已经保存在本地的" << filename << "文件中！\n"
		<< "现在请您输入想要查找的关键字\n";

	string pattern;
	cin >> pattern;
	ifstream fin(filename, ios::in);
	if (!fin)
	{
		cerr << "输入文件打开失败！\n";
		exit(1);
	}

	string buf;
	string input_text;
	while (!fin.eof())
	{
		getline(fin, buf);
		input_text += buf;
	}

	cout << "源文本内容为：\n" << input_text << endl;
	TrieTree trietree(input_text);
	cout << "使用字典树查得关键词出现次数为：" << trietree.search(pattern) << "\n（ps 不区分大小写 并且要求完全匹配 不支持数字 -1表示不存在）\n";

	fin.close();
	return 0;
}


TrieTree::TrieTree(string text)
{
	root = new TrieNode();
	int size = text.size();
	string word;
	for (int i = 0; i < size; i++)
	{
		word = "";
		while (isalpha(text[i]))
		{
			word += text[i];
			i++;
		}
		if (word != "")
			insert(word);
	}
}

TrieTree::~TrieTree()
{
	clear(root);
}

void TrieTree::insert(string word)
{
	int size = word.size();
	TrieNode* cursor = root;
	for (int i = 0; i < size; i++)
	{
		if (cursor->next[toOrder(word[i])] == nullptr)
			cursor->next[toOrder(word[i])] = new TrieNode();
		cursor = cursor->next[toOrder(word[i])];
	}
	cursor->isWord = true;
	cursor->count++;
}

int TrieTree::search(string key)
{
	int size = key.size();
	TrieNode* cursor = root;
	for (int i = 0; i < size; i++)
	{
		if (cursor->next[toOrder(key[i])] != nullptr)
			cursor = cursor->next[toOrder(key[i])];
		else
			return -1; //don't exist
	}
	if (cursor->isWord)
		return cursor->count;
	else
		return -1; //do not exist
}

void TrieTree::clear(TrieNode* pos)
{
	if (!pos) return;

	for (int i = 0; i < ALPHABET_SIZE; i++)
	{
		if (!(pos->next[i]))
			clear(pos->next[i]);
	}

	delete pos;
}

int toOrder(char ch)
{
	return (ch >= 'A' && ch <= 'Z' ? ch - 'A' : ch - 'a');
}
