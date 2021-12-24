#include<iostream>
#include<string>

using namespace std;

//table display settings
constexpr int IDLENGTH = 10;
constexpr int NAMELENGTH = 12;
constexpr int SEXLENGTH = 8;
constexpr int AGELENGTH = 10;
constexpr int CATEGORYLENGTH = 20;

//operation code
//convenient for reading
constexpr int SEARCH = 1;
constexpr int INSERT = 2;
constexpr int ADD = 3;
constexpr int REVISE = 4;
constexpr int DEL = 5;
constexpr int DISPLAY = 6;
constexpr int CLEAR = 7;
constexpr int EXIT = 0;

class Candidate
{
private:
	//associated information
	int id;
	string name;
	string sex;
	short int age;
	string category;

public:
	//operator overload
	friend ostream& operator<<(ostream& output, const Candidate candidate)
	{
		output << candidate.id;
		int n = IDLENGTH - to_string(candidate.id).size();
		for (int i = 0; i < n; i++)
			output << " ";

		output << candidate.name;
		n = NAMELENGTH - candidate.name.size();
		for (int i = 0; i < n; i++)
			output << " ";

		output << candidate.sex;
		n = SEXLENGTH - candidate.sex.size();
		for (int i = 0; i < n; i++)
			output << " ";

		output << candidate.age;
		n = AGELENGTH - to_string(candidate.age).size();
		for (int i = 0; i < n; i++)
			output << " ";

		output << candidate.category;
		n = CATEGORYLENGTH - candidate.category.size();
		for (int i = 0; i < n; i++)
			output << " ";

		output << endl;

		return output;
	}

	//constructor
	Candidate():id(0),name(""),sex(""),age(0),category(""){}
	Candidate(int id, string name, string sex, short int age, string category) :id(id), name(name), sex(sex), age(age), category(category) {}

	//set the data
	void Setid(int n) { id = n; }
	void Setname(string data) { name = data; }
	void Setsex(string data) { sex = data; }
	void Setage(short int data) { age = data; }
	void Setcategory(string data) { category = data; }

	//get the data
	int Getid() { return id; }
	string Getname() { return name; }
	string Getsex() { return sex; }
	short int Getage() { return age; }
	string Getcategory() { return category; }

};

struct LinkNode
{
	Candidate candidate;
	LinkNode* next;

	//constructor
	LinkNode(int id, string name, string sex, short int age, string category):candidate(id,name,sex,age,category),next(nullptr){}
	LinkNode() :candidate(), next(nullptr) {}
};

class Exam_Registration_System // the same as list
{
private:
	LinkNode* Candidate_Table_Tail;
	LinkNode* Candidate_Table_Head;
	int length;

public:
	//constructor
	Exam_Registration_System() : length(0)
	{
		Candidate_Table_Tail = nullptr;
		Candidate_Table_Head = nullptr;
	}

	//destructor
	~Exam_Registration_System();

	//get the length
	int Getlength(){ return length; }

	//function
	void create();
	LinkNode* search_by_id(int key);
	void insert(int order, int id, string name, string sex, short int age, string category);
	void add(int id, string name, string sex, short int age, string category); // add from the end
	void revise_by_id(int key, int key_id, string key_name, string key_sex, short int key_age, string key_category);
	void del_by_id(int key);
	void display();
	bool empty() { return length == 0; }
	void clear();
};



Exam_Registration_System::~Exam_Registration_System()
{
	cout << "退出中..\n"
		<< "欢迎下次再见子\n";

	LinkNode* cursor = Candidate_Table_Head;
	while (cursor != nullptr) //delete nodes by iteration
	{
		LinkNode* temp = cursor;
		cursor = cursor->next;
		delete temp;
	}
}

void Exam_Registration_System::create()
{
	int candidate_num = 0, id = 0;
	short age = 0;
	string name, sex, category;

	cout << "现在开始建立考生信息系统\n"
		<< "请输入考生人数：";

	while (true)
	{
		cin >> candidate_num;
		if (cin.fail())
		{
			cin.clear();
			cin.ignore(1024, '\n');
			cout << "\n输入有误！请重新输入子\n";
			continue;
		}
		else
			break;
	}

	cout << "请依次输入考生的考号、姓名、性别（男/女）、年龄及报考类别\n";
	for (int i = 0; i < candidate_num; i++)
	{
		while (true)
		{
			cin >> id >> name >> sex >> age >> category;
			if (cin.fail())
			{
				cin.clear();
				cin.ignore(1024, '\n');
				cout << "\n输入有误！请重新输入子\n";
				continue;
			}
			else
				break;
		}

		add(id, name, sex, age, category); //using add function
	}

	cout << "ok啦！初步已经建立完毕了呢！\n";
	return;
}

LinkNode* Exam_Registration_System::search_by_id(int key)
{
	if (empty()) // judge if the table is empty
	{
		cout << "报名表现在为空！先来添加一点数据吧\n";
		return nullptr;
	}

	LinkNode* cursor = Candidate_Table_Head;

	while (cursor != nullptr && cursor->candidate.Getid() != key)
	{
		cursor = cursor->next;
	}

	if (cursor == nullptr)
	{
		cout << "阿欧 但是真的没有找到呢\n";
		return nullptr;
	}
	else
	{
		cout << "查到啦~~\n";
		return cursor;
	}
}

void Exam_Registration_System::insert(int order,int id, string name, string sex, short int age, string category)
{
	// judge whether the command meet the request
	if (empty())
	{
		// cout << "报名表现在为空！先来添加一点数据吧\n";
		add(id, name, sex, age, category);
		return;
	}
	else if (order < 1 || order > length)
	{
		cerr << "啊偶 插入的位置貌似不对呢 要不再看看？\n";
		return;
	}

	//operate
	if (order == length) //equal to add function
	{
		add(id, name, sex, age, category);
		return;
	}


	int i = 1; // 初始位置为1

	LinkNode* cursor = Candidate_Table_Head;
	LinkNode* new_data = new LinkNode(id, name, sex, age, category);

	if (order != 1)
	{
		while (i != order - 1) // 移动到之前一个
		{
			cursor = cursor->next;
			i++;
		}

		LinkNode* temp = cursor->next;
		cursor->next = new_data;
		new_data->next = temp;
	}
	else
	{
		new_data->next = Candidate_Table_Head;
		Candidate_Table_Head = new_data;
	}

	length++;

	cout << "插入成功！\n";
}

void Exam_Registration_System::add(int id, string name, string sex, short int age, string category)
{
	LinkNode* new_data = new LinkNode(id, name, sex, age, category);

	if (empty())
	{
		Candidate_Table_Head = Candidate_Table_Tail = new_data;
	}
	else
	{
		Candidate_Table_Tail->next = new_data;
		Candidate_Table_Tail = Candidate_Table_Tail->next;
	}

	length++;
}

void Exam_Registration_System::revise_by_id(int key, int key_id, string key_name, string key_sex, short int key_age, string key_category)
{
	LinkNode* target = search_by_id(key);
	if (target == nullptr)
	{
		cerr << "但是但是 没有找到这个考生呢 再试一试？\n";
		return;
	}
	
	target->candidate.Setid(key_id);
	target->candidate.Setname(key_name);
	target->candidate.Setsex(key_sex);
	target->candidate.Setage(key_age);
	target->candidate.Setcategory(key_category);

	cout << "修改成功！"
		<< "现在的情况是：\n"
		<< target->candidate;

	return;
}

void Exam_Registration_System::del_by_id(int key)
{
	if (empty())
	{
		cout << "报名表现在为空！先来添加一点数据吧\n";
		return;
	}

	LinkNode* cursor = Candidate_Table_Head;

	if (cursor->candidate.Getid() == key)
	{
		Candidate_Table_Head = Candidate_Table_Head->next;

		if (length == 1)
			Candidate_Table_Tail = Candidate_Table_Head;

		delete cursor;
		length--;
		return;
	}
	else
	{
		while (cursor->next != nullptr && cursor->next->candidate.Getid() != key)
		{
			cursor = cursor->next;
		}

		if (cursor->next == nullptr)
		{
			cerr << "没有找到要删除的目标子！" << endl;
			return;
		}
	}

	LinkNode* temp = cursor->next;
	cursor->next = temp->next;
	delete temp;
	length--;

	if (cursor->next == nullptr)
		Candidate_Table_Tail = cursor;

	return;
}


void Exam_Registration_System::display()
{
	if (empty()) // judge if the table is empty
	{
		cerr << "报名表现在为空！先来添加一点数据吧\n";
		return;
	}

	string ID = "考号";
	string NAME = "姓名";
	string SEX = "性别";
	string AGE = "年龄";
	string CATEGORY = "报考类别";

	cout << ID;
	for (int i = 0; i < IDLENGTH - ID.size(); i++)
	{
		cout << " ";
	}

	cout << NAME;
	for (int i = 0; i < NAMELENGTH - NAME.size(); i++)
	{
		cout << " ";
	}

	cout << SEX;
	for (int i = 0; i < SEXLENGTH - SEX.size(); i++)
	{
		cout << " ";
	}

	cout << AGE;
	for (int i = 0; i < AGELENGTH - AGE.size(); i++)
	{
		cout << " ";
	}

	cout << CATEGORY;
	for (int i = 0; i < CATEGORYLENGTH - CATEGORY.size(); i++)
	{
		cout << " ";
	}

	cout << endl;

	LinkNode* cursor = Candidate_Table_Head;
	for (int i = 0; i < length; i++)
	{
		cout << cursor->candidate;
		cursor = cursor->next;
	}

}

//void Exam_Registration_System::print_candidate(LinkNode* node)
//{
//	if (node == nullptr)
//		return;
//	else
//	{
//		print_candidate(node->next);
//		cout << node->candidate;
//	}
//}

void Exam_Registration_System::clear()
{
	LinkNode* cursor = Candidate_Table_Head;
	Candidate_Table_Head = Candidate_Table_Tail = nullptr;
	while (cursor!=nullptr)
	{
		LinkNode* temp = cursor;
		cursor = cursor->next;
		delete temp;
	}

	length = 0;
}

int GetOrder() // get insertion order
{
	int order;
	while (true)
	{
		cout << "请输入位置子（最开始是1）：";
		cin >> order;
		if (cin.fail())
		{
			cin.clear();
			cin.ignore(1024, '\n');
			cout << "\n输入有误！请重新输入子\n";
			continue;
		}
		else
			break;
	}

	return order;
}

int GetID() // get id
{
	int id = -1;
	while (true)
	{
		cout << "请输入考号子：";
		cin >> id;
		if (cin.fail())
		{
			cin.clear();
			cin.ignore(1024, '\n');
			cout << "\n输入有误！请重新输入子\n";
			continue;
		}
		else
			break;
	}

	return id;
}

int main()
{
	Exam_Registration_System exam_registration_system;

	exam_registration_system.create();

	bool flag = false;
	int operation_code;

	while (true)
	{
		cout << "请输入指令来执行相应命令哦~\n"
			<< "1 查找 2 插入 3 添加 4 修改 5 删除 6 显示 7 清空 0 退出\n"
			<< "请输入操作码：";

		while (true)
		{
			cin >> operation_code;
			if (cin.fail())
			{
				cerr << "额 好像输入错误呢 重新输入试试？\n"
					<< "请输入操作码：";
				cin.clear();
				cin.ignore(1024, '\n');
			}
			else
				break;
		}

		switch (operation_code)
		{
			case SEARCH:
			{
				cout << "请输入考生的考号：\n";
				int key;
				cin >> key;
				LinkNode* result = exam_registration_system.search_by_id(key);
				if (result != nullptr)
				{
					cout << result->candidate;
				}

				break;
			}

			case INSERT:
			{
				int id = 0, order = GetOrder();
				short age = 0;
				string name, sex, category;

				cout << "请输入考生的考号、姓名、性别（男/女）、年龄及报考类别\n";
				cout << "PS 如果报名表内无数据 则无论数字如何都默认添加新数据\n";
			
				while (true)
				{
					cin >> id >> name >> sex >> age >> category;
					if (cin.fail())
					{
						cin.clear();
						cin.ignore(1024, '\n');
						cout << "\n输入有误！请重新输入子\n";
						continue;
					}
					else
						break;
				}

				exam_registration_system.insert(order, id, name, sex, age, category);
				break;
			}
			case ADD:
			{
				int id = 0;
				short age = 0;
				string name, sex, category;

				cout << "请输入考生的考号、姓名、性别（男/女）、年龄及报考类别\n";

				while (true)
				{
					cin >> id >> name >> sex >> age >> category;
					if (cin.fail())
					{
						cin.clear();
						cin.ignore(1024, '\n');
						cout << "\n输入有误！请重新输入子\n";
						continue;
					}
					else
						break;
				}

				exam_registration_system.add(id, name, sex, age, category);
				break;
			}
			case REVISE:
			{
				int key_id = GetID();
				int id = 0;
				short age = 0;
				string name, sex, category;

				cout << "请输入考生的考号、姓名、性别（男/女）、年龄及报考类别\n";

				while (true)
				{
					cin >> id >> name >> sex >> age >> category;
					if (cin.fail())
					{
						cin.clear();
						cin.ignore(1024, '\n');
						cout << "\n输入有误！请重新输入子\n";
						continue;
					}
					else
						break;
				}

				exam_registration_system.revise_by_id(key_id, id, name, sex, age, category);
				break;
			}
			case DEL:
			{
				exam_registration_system.del_by_id(GetID());
				break;
			}
			case DISPLAY:
			{
				exam_registration_system.display();
				break;
			}
			case CLEAR:
			{
				exam_registration_system.clear();
				cout << "已经清空了呜呜\n";
				break;
			}
			case EXIT:
			{
				flag = true;
				break;
			}
			default:
			{
				cout << "额..我没有听懂呢 再发一遍？\n";
				break;
			}
		}
		if (flag)
		{
			cout << "山水有相逢 我们来日再见！\n";
			break;
		}
	}
	return 0;
}
