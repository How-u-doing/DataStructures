#include"SeqList.h"
#include<iostream>
#include<cstdlib>
using namespace std;
template<typename T>
SeqList<T>::SeqList(int sz )
{
	if (sz > 0)
	{
		listSize = sz;	length = 0;
		data = new T[sz];
		if (data == nullptr)
		{cerr << "Memory allocation error!" << endl;exit(1);}
	}
}

template<typename T>
SeqList<T>::SeqList(SeqList<T>&L)
{
	listSize = L.Size(); length = L.Length();
	data = new T[listSize];
	if (data == nullptr)
	{cerr << "Memory allocation error!" << endl; exit(1);}
	T temp;
	for (int i = 0; i < length; ++i)
	{
		L.getData(i + 1, temp);
		data[i] = temp;
	}
}

template<typename T>
SeqList<T>::~SeqList() { delete[]data; }

template<typename T>
int SeqList<T>::Size()const
{
	return listSize;
}

template<typename T>
int SeqList<T>::Length()const
{
	return length;
}

template<typename T>
int SeqList<T>::Search(const T &x)const
{
	for (int i = 0; i < length; ++i)
		if (x == data[i])return i + 1;
	return 0;//未找到
}

template<typename T>
int SeqList<T>::Locate(int i)const
{//定位判断第i个表项是否在表内,若在,则返回第i(1<=i<=length)个表项,若不在,返回0(定位失败)
	if (i >= 1 && i <= length)return i;
	return 0;//定位失败
}

template<typename T>
bool SeqList<T>::getData(int i, T &x)const
{
	if (i >= 1 && i <= length) {
		x = data[i - 1]; return true;
	}
	else return false;
}

template<typename T>
bool SeqList<T>::setData(int i, const  T &x)
{
	if (i >= 1 && i <= length) {
		data[i - 1] = x; return true;
	}
	else return false;
}

template<typename T>
bool SeqList<T>::Insert(int i, const  T &x)
{//将x插入在第i个表项之前
	if (length == listSize) {
		cout << "The list is full.Failed to insert!" << endl; return false;}//表满,不能插入
	
	if (i<1 || i>length) {
		cout << "Invalid parameter i=" << i << ".Failed to insert!"<< endl; return false;}//参数i不合理,不能插入
	
	for (int j = length - 1; j >= i - 1; --j)
		data[j + 1] = data[j];
	data[i - 1] = x;
	++length;
	return true;
}

template<typename T>
bool SeqList<T>::Remove(int i, T &x)
{//只是移除某一项数据,并不改变表的大小,实际长度减一
	if (i >= 1 && i <= length) {
		x = data[i - 1];
		for (int j = i; j < length; ++j)
			data[j-1] = data[j];
		--length;
		return true;
	}
	else return false;
}

template<typename T>
bool SeqList<T>::IsEmpty()const
{
	if (length)return false;
	return true;
}

template<typename T>
bool SeqList<T>::IsFull()const
{
	if (listSize == length)return true;
	else return false;
}

template<typename T>
void SeqList<T>::Sort()
{//根据T的类型待定
	
}

template<typename T>
void SeqList<T>::Input()
{
	cout << "Please input the data item(s)  (Attention:end up with Ctrl+Z)" << endl;
	int i = 0;
	while (cin >> data[i++]) {
		++length; 
		if (length >= listSize) {
			cout << "The list is full" << endl; return;
		}
	}
	cout << "You have input " << length << " data item(s)" << endl;
}

template<typename T>
void SeqList<T>::Output()const
{
	for (int i = 0; i < length; ++i)
		cout << data[i] << endl;
}

template<typename T>
SeqList<T> SeqList<T>::operator=(SeqList<T>&L)
{
	if(&L!=this){
	T* data0 = data;
	length = L.Length(); listSize = L.Size();
	data = new T[length];
	if (data == nullptr)
	{cerr << "Memory allocation error!" << endl; exit(1);}
	T temp;
	for (int i = 0; i < length; ++i){
		L.getData(i + 1, temp); data[i] = temp;
	}
	delete[]data0;//释放之前表的内存
	}
	return *this;
}

template<typename T>
void SeqList<T>::reSize(int newSize)
{
	if (newSize <= 0)
	{cerr << "Invalid size of the new SeqList!" << endl; return;}
	//以下为newSize>=1的情况
	if (newSize > listSize)//需要申请新的存储空间,一般均为此种情况
	{
		T *newData = new T[newSize];
		if (newData == nullptr)
		{cerr << "Memory allocation error!" << endl; exit(1);}
		for (int i = 0; i < length; ++i)
			newData[i] = data[i];
		delete[]data;
		data = newData; listSize = newSize;
		cout << "Successfully augmented the list's size!" << endl;
		return;//不再进行下面的if判断
	}
	if (newSize>length && newSize < listSize)//之前申请的表的大小太大,想把多余的一些空白项的存储空间释放
	{//注意上面两个比较的先后顺序,与下面的else的微妙关系
		delete[](data + newSize);//problems remaining !
		listSize = newSize;
		cout << "The size of the list has decreased!" << endl;
	}
	else  //1<=newSize<length,可能会删除一些数据项
	{
		cout << "Waring:newSize<length,some data will be deleted!" << endl;
		cout << "Are you sure to go on?\nPress Y or y to continue,n or N to cancel" << endl;
		char ch;
		cin.clear();
		cin.sync();
		cin >> ch;
		if (ch == 'y' || 'Y')
		{
			delete[](data + newSize);//still exist some problems
			listSize = newSize;
			cout << "Successfully augmented the list's size!" << endl;
		}
		else cout << "Successfully cancelled!" << endl;
	}
}

