/*
	递归版插入排序
	运行时间：
		T(n) =	T(n-1) + C(n - 1)	n != 1
				θ(1)				n = 1
*/
#include <stdio.h>
#include <iostream>
using namespace std;

void insert(int a[], int i){
	int key = a[i];
	int j = i - 1;
	while(a[j] > key && j >= 0){
			a[j + 1] = a[j];
			--j;
	}
	a[j + 1] = key;
}
void insertion_sort(int a[],int i){
	int len = i;
	if(len > 0){
		insertion_sort(a, --len);
		insert(a, len);		//C(n - 1)
	}
}
void main(){
	int a[] = { 1, 5, 7, 3, 8, 2, 4, 0, 9, 6 };
	insertion_sort(a, 10);

	for (int i = 0; i < 10; ++i)
		cout<<a[i]<<endl;
	system("pause");
}