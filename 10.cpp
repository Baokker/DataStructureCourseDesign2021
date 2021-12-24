#include<iostream>
#include<iomanip>
#include<time.h>
#include<cmath>

using namespace std;

//struct for partitionDutchFlag in quicksort
struct twoNum {
	int first, second;
	twoNum(int first, int second) :first(first), second(second) {}
};

class Sort
{		
	private:
		//store* array keeps values and useData* for operations
		int* store, * useData, size, times;
	public:
		Sort(int n);

		~Sort() { 
			delete[]useData;
			delete[]store;
		}
			
		inline void swap(int& left, int& right);
		void reset();
		void output(string method) { cout << setw(20) << left << method + "的次数" << times << endl; }
		void log();//for test 
		void print();

		void bubble_sort(bool(*compare_func)(int left, int right));
		void quick_sort(bool(*compare_func)(int left, int right)); 
		void quick_sort(bool(*compare_func)(int left, int right), int low, int high);
		void selection_sort(bool(*compare_func)(int left, int right));
		void heap_sort(bool(*compare_func)(int left, int right));
		void straight_insertion_sort(bool(*compare_func)(int left, int right));
		void shell_sort(bool(*compare_func)(int left, int right));
		//merge_sort uses iterative version so you dont need to reset manually 
		//(you can switch to recursive version via notation)
		void merge_sort(bool(*compare_func)(int left, int right));

		void radix_sort(bool(*compare_func)(int left, int right), int base = 10); // 基排序

		//for merge sort
		void merge_sort(bool(*compare_func)(int left, int right), int start, int end); //recursive
		void merge(bool(*compare_func)(int left, int right), int start, int mid, int end);

		//for radix sort
		int getDigit();

		twoNum partitionDutchFlag(bool(*compare_func)(int left, int right), int low, int high, int pivotIndex);
		// dutch national flag partition method for quick_sort
		// improve the situation of different pivot locations compared to lomuto's algorithm
};

// for heap sort
struct Heap
{
	int* data;
	int size;
	bool(*compare_func)(int left, int right);
	int times; // for statistics

	Heap() :data(nullptr), size(0) {}
	Heap(int* copy, int length, bool(*compare)(int left, int right));
	~Heap();

	int parent(int index);
	int left(int index);
	int right(int index);

	void makeHeap(int length);
	void heapify(int index, int length);
};

Heap::~Heap() { delete data; }

int Heap::parent(int index) { return floor((index - 1) / 2); }

int Heap::left(int index) { return 2 * index + 1; }

int Heap::right(int index) { return 2 * index + 2; }

void Heap::makeHeap(int length)
{
	for (int i = length / 2 - 1; i >= 0; i--) {
		heapify(i, length);
	}
}

void Heap::heapify(int index, int length)
{
	int compare_index, left_index, right_index;
	while (left(index) < length) {
		// 最终的bug就是在这里！循环终止应该是这个节点的孩子超出了范围 而非这个节点超出范围！
		// 不然会导致越界 在heap_sort时把已经排序好的又拿回来了！！
		compare_index = left_index = left(index);
		right_index = right(index);
		if (left_index >= length) return;
		if (right_index < length && compare_func(data[right_index], data[left_index]))
			compare_index++; // compare_index = right(i);
		if (compare_func(data[index], data[compare_index]))
			return;
		else {
			times++;
			swap(data[index], data[compare_index]);
			index = compare_index;
		}
	}
}

//copy an array with a fixed size and then convert it into a heap
Heap::Heap(int* copy, int length, bool(*compare)(int left, int right))
{
	size = length;
	data = new int[size];
	compare_func = compare;
	for (int i = 0; i < size; i++)
		data[i] = copy[i];

	times = 0;

	makeHeap(size);
}

Sort::Sort(int n)
{
	store = new int[n];
	useData = new int[n];
	srand(time(NULL));
	for (int i = 0; i < n; i++)
		store[i] = useData[i] = rand();
	times = 0;
	size = n;
}

void Sort::log()
{
	for (int i = 0; i < size; i++)
		cout << useData[i] << " ";
	cout << endl;
}

void Sort::print()
{
	for (int i = 0; i < size; i++)
		cout << store[i] << " ";
	cout << endl;
}

void Sort::bubble_sort(bool(*compare_func)(int left, int right))
{
	for (int i = 0; i < size; i++) {
		for (int j = 0; j < size - i - 1; j++) { 
			// size - i - 1 非常重要！i表示已有i个数据整好放在最后 1表示防溢出！
			if (compare_func(useData[j], useData[j + 1]))
				swap(useData[j], useData[j + 1]);
			times++;
		}

	}

	//log();
	output("冒泡排序");
	reset();
}

void Sort::quick_sort(bool(*compare_func)(int left, int right))
{
	quick_sort(compare_func, 0, size - 1);

	//log();
	output("快速排序");
	reset();
}

void Sort::quick_sort(bool(*compare_func)(int left, int right), int low, int high)
{
	if (low < high) {
		// you can random a number here to improve the stablity
		int random = (rand() % (high - low + 1)) + low;
		twoNum result = partitionDutchFlag(compare_func, low, high, random);
		quick_sort(compare_func, low, result.first - 1);
		quick_sort(compare_func, result.second + 1, high);
	}
}

void Sort::selection_sort(bool(*compare_func)(int left, int right))
{
	for (int i = 0; i < size; i++) {
		int minIndex = i;
		for (int j = i + 1; j < size; j++) {
			if (compare_func(useData[minIndex], useData[j])) 
				minIndex = j;
			times++;

		}
		swap(useData[minIndex], useData[i]);
	}

	//log();
	output("选择排序");
	reset();
}

void Sort::heap_sort(bool(*compare_func)(int left, int right))
{
	Heap heap(store, size, compare_func);

	for (int i = size - 1; i > 0; i--) { // 堆在堆他已经排序好的东西！！
		swap(heap.data[i], heap.data[0]);
		heap.heapify(0, i);
	}

	//for (int i = 0; i < size; i++)
	//	cout << heap.data[i] << " ";
	//cout << endl;

	cout << setw(20) << left << "堆排序的次数" << heap.times << endl;
	//reset();
}

void Sort::straight_insertion_sort(bool(*compare_func)(int left, int right))// oh my god
{
	for (int i = 0; i < size - 1; i++) {
		int temp = useData[i + 1], j = i + 1;
		while (j) {
			useData[j] = useData[j - 1];
			times++;
			if (compare_func(temp, useData[j - 1])) {
				break;
			}
			j--;
		}
		swap(useData[j], temp);
	}

	//another proper logic
	//for (int i = 0; i < size - 1; i++) {
	//	int j = i + 1;
	//	while (j && compare_func(useData[j - 1], useData[j])) {
	//		swap(useData[j - 1], useData[j]);
	//		j--;
	//	}
	//}

	//log();
	output("直接插入排序");
	reset();
}

void Sort::shell_sort(bool(*compare_func)(int left, int right))
{
	int subSize = size / 2;
	while (subSize) {
		for (int i = 0; i < subSize; i++) { //insertion sort by gap of subsize
			int j = i + subSize;
			while (j < size) {
				times++;
				int m = j, temp = useData[j];
				while (m - subSize >= 0 && compare_func(useData[m - subSize], temp)){
					times++;
					useData[m] = useData[m - subSize];
					m -= subSize;
				}
				swap(temp, useData[m]);
				j += subSize;
			}
		}
		subSize /= 2;
	}

	//log();
	output("希尔排序");
	reset();
}

void Sort::merge_sort(bool(*compare_func)(int left, int right))
{
	//recursive version
	//need to reset and output outside the function
	//up->bottom
	//merge_sort(compare_func, 0, size - 1);

	//iterative version
	//bottom->up
	int partSize, startIndex, midIndex, endIndex;
	for (partSize = 1; partSize < size; partSize *= 2) {
		for (startIndex = 0; startIndex < size; startIndex += (2 * partSize)) {
			midIndex = min(startIndex + partSize - 1, size - 1); //闭区间
			endIndex = min(startIndex + 2 * partSize - 1, size - 1);
			merge(compare_func, startIndex, midIndex, endIndex);
		}
	}

	//log();
	output("合并排序");
	reset();
}

void Sort::merge_sort(bool(*compare_func)(int left, int right), int start, int end) //[start,end]
{
	if (start < end) {
		int mid = (start + end) / 2;
		merge_sort(compare_func, start, mid);
		merge_sort(compare_func, mid + 1, end);
		merge(compare_func, start, mid, end);
	}
}

//compare it to double pointers
void Sort::merge(bool(*compare_func)(int left, int right), int start, int mid, int end)
{
	int leftIndex = start, rightIndex = mid + 1, * tempData = new int[end - start + 1], tempIndex = 0;
	while (leftIndex <= mid && rightIndex <= end) {
		times++;
		if (compare_func(useData[rightIndex], useData[leftIndex]))
			tempData[tempIndex++] = useData[leftIndex++];
		else
			tempData[tempIndex++] = useData[rightIndex++];
	}

	while (leftIndex <= mid) {
		tempData[tempIndex++] = useData[leftIndex++];
		times++;
	}

	while (rightIndex <= end) {
		tempData[tempIndex++] = useData[rightIndex++];
		times++;
	}

	for (int i = start; i <= end; i++)
		useData[i] = tempData[i - start];

	delete[]tempData;
}

void Sort::radix_sort(bool(*compare_func)(int left, int right), int base)
{
	int digit = getDigit(), radix = 1, num;
	int* nums = new int[base], * tempData = new int[size];
	for (int i = 0; i < digit; i++, radix *= 10) {
		for (int j = 0; j < base; j++) // 清零
			nums[j] = 0;

		for (int j = 0; j < size; j++) {
			times++;
			(nums[useData[j] / radix % 10])++;
		}

		for (int j = 1; j < base; j++) // 妙哉
			nums[j] += nums[j - 1];

		for (int j = size - 1; j >= 0; j--) //应从尾部开始 不然后面数字的权重不符
			tempData[--(nums[useData[j] / radix % 10])] = useData[j];
		
		for (int j = 0; j < size; j++)
			useData[j] = tempData[j];
	}

	delete[] tempData;
	delete[] nums;

	//log();
	output("基排序");
	reset();
}

int Sort::getDigit()
{
	int result = 1, maxNum = store[0];

	for (int i = 0; i < size; i++)
		if (store[i] > maxNum)
		maxNum = store[i];

	while (maxNum /= 10)
		result++;

	return result;
}

twoNum Sort::partitionDutchFlag(bool(*compare_func)(int left, int right), int low, int high, int pivotIndex)
{
	int smaller = low, equal = low, larger = high, pivot = useData[pivotIndex];
	while (equal <= larger) {
		times++;
		if (compare_func(useData[equal], pivot)) {
			swap(useData[equal], useData[larger]);
			larger--;
		}
		else if (useData[equal] == pivot)
			equal++;
		else {
			swap(useData[smaller], useData[equal]);
			equal++;
			smaller++;
		}
	}

	return twoNum(smaller, larger);
}

int main()
{
	int size;
	cout << "请输入要产生的随机数的个数：";
	cin >> size;
	Sort sort(size);

	cout << "请选择从大到小或从小到大排序(y/n)(默认从大到小)\n"
		<< "ps 基数排序目前仅支持从小到大排序\n";
	bool(*compare_func)(int left, int right) = [](int left, int right) -> bool {return left < right; };
	char input;
	cin >> input;
	if (input == 'n' || input == 'N')
		compare_func = [](int left, int right) -> bool {return left > right; };

	int choice = -1;
	while (true) {
		while (true) {
			cout << "1-冒泡排序 2-选择排序 3-直接插入排序 4-希尔排序\n"
				<<"5-快速排序 6-堆排序 7-归并排序 8-基数排序 9-输出原始 0-退出\n";

			cin >> choice;

			if (cin.fail()) {
				cin.clear();
				cin.ignore(1024, '\n');
			}
			else if (choice < 0 || choice>9)
				cout << "输入存在错误！请重新输入\n";
			else 
				break;
		}

		if (!choice) break;
		auto timeStart = clock();
		switch (choice)
		{
			case 1:
				sort.bubble_sort(compare_func);
				break;
			case 2:
				sort.selection_sort(compare_func);
				break;
			case 3:
				sort.straight_insertion_sort(compare_func);
				break;
			case 4:
				sort.shell_sort(compare_func);
				break;
			case 5:
				sort.quick_sort(compare_func);
				break;
			case 6:
				sort.heap_sort(compare_func);
				break;
			case 7:
				sort.merge_sort(compare_func);
				break;
			case 8:
				sort.radix_sort(compare_func);
				break;
			case 9:
				sort.print();
				break;
		}
		auto timeEnd = clock();
		cout << "花费时间为" << double((timeEnd - timeStart)) / CLOCKS_PER_SEC * 1000 << "ms\n";
	}
	return 0;
}

void Sort::swap(int& left, int& right)
{
	if (left == right) return;

	int temp = left;
	left = right;
	right = temp;
}

void Sort::reset()
{
	times = 0;
	for (int i = 0; i < size; i++) {
		useData[i] = store[i];
	}
}