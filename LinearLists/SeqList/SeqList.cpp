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
	return 0;//δ�ҵ�
}

template<typename T>
int SeqList<T>::Locate(int i)const
{//��λ�жϵ�i�������Ƿ��ڱ���,����,�򷵻ص�i(1<=i<=length)������,������,����0(��λʧ��)
	if (i >= 1 && i <= length)return i;
	return 0;//��λʧ��
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
{//��x�����ڵ�i������֮ǰ
	if (length == listSize) {
		cout << "The list is full.Failed to insert!" << endl; return false;}//����,���ܲ���
	
	if (i<1 || i>length) {
		cout << "Invalid parameter i=" << i << ".Failed to insert!"<< endl; return false;}//����i������,���ܲ���
	
	for (int j = length - 1; j >= i - 1; --j)
		data[j + 1] = data[j];
	data[i - 1] = x;
	++length;
	return true;
}

template<typename T>
bool SeqList<T>::Remove(int i, T &x)
{//ֻ���Ƴ�ĳһ������,�����ı��Ĵ�С,ʵ�ʳ��ȼ�һ
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
{//����T�����ʹ���
	
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
	delete[]data0;//�ͷ�֮ǰ����ڴ�
	}
	return *this;
}

template<typename T>
void SeqList<T>::reSize(int newSize)
{
	if (newSize <= 0)
	{cerr << "Invalid size of the new SeqList!" << endl; return;}
	//����ΪnewSize>=1�����
	if (newSize > listSize)//��Ҫ�����µĴ洢�ռ�,һ���Ϊ�������
	{
		T *newData = new T[newSize];
		if (newData == nullptr)
		{cerr << "Memory allocation error!" << endl; exit(1);}
		for (int i = 0; i < length; ++i)
			newData[i] = data[i];
		delete[]data;
		data = newData; listSize = newSize;
		cout << "Successfully augmented the list's size!" << endl;
		return;//���ٽ��������if�ж�
	}
	if (newSize>length && newSize < listSize)//֮ǰ����ı�Ĵ�С̫��,��Ѷ����һЩ�հ���Ĵ洢�ռ��ͷ�
	{//ע�����������Ƚϵ��Ⱥ�˳��,�������else��΢���ϵ
		delete[](data + newSize);//problems remaining !
		listSize = newSize;
		cout << "The size of the list has decreased!" << endl;
	}
	else  //1<=newSize<length,���ܻ�ɾ��һЩ������
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

