#include <stdio.h>
#include <iostream>
using namespace std;

//��д���ֲ����ж�����������valӦ�ڵ�λ�ã�������δ����������val��ֵ
int binary_search(int a[], int p, int r, int val){
	/*
	//����Ҫ������������ķ���
	if(r < p){
		cout << "find out"<< endl;
		return ;
	}

	int q = (p + r) / 2;
	if(a[q] < val)
		binary_search(a, q + 1, r, val);
	else if (a[q] > val )
		binary_search(a, p, q - 1, val);
	else if (a[q] == val)
		cout << "value in :" << q <<endl;
	*/

	//δ���
	int q = (p + r) / 2;
	if (a[q] < val < a[q + 1])
		return q + 1;
	else if (a[q] < val)
		binary_search(a, q + 1, r, val);
	else if (a[q] > val )
		binary_search(a, p, q - 1, val);
	return q + 1;
}

void insertion_sort_binary(int a[], int p, int r){
	for (int j = 1; j < (r - p); ++j){
		int key = a[j];
		int k = binary_search(a, p, j - 1, key);
		for(int i = j - 1; i >= k; --i)
			a[i + 1] = a[i];
		a[k] = key;
	}
}

void main(){
	int a[] = { 1, 5, 7, 3, 8, 2, 4, 0, 9, 6 };
	int b[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};

	insertion_sort_binary(a, 0, 9);
	for (int i = 0; i < 10; ++i)
		cout<<a[i]<<endl;

	system("pause");
}