#include <stdio.h>
#include <iostream>
using namespace std;

void merge(int a[], int p, int q, int r){
	int n1 = q - p + 1;
	int n2 = r - q;
	int *left=new int[n1+1];
	int *right=new int[n2+1];
	for (int i = 0; i < n1; ++i)
		left[i] = a[p + i];
	for (int j = 0; j < n2; ++j)
		right[j] = a[q + j + 1];
	left[n1] = right [n2] = 1000000;
	int i = 0, j =0;
	for (int k = p; k <= r; ++k){
		if (left[i] <= right[j])
			a[k] = left[i++];
		else
			a[k] = right[j++];
	}
}

void merge_sort(int a[], int p, int r){
	if (p < r){
		int q = (p + r) / 2;
		merge_sort(a, p, q);
		merge_sort(a, q + 1, r);
		merge(a, p, q, r);
	}
}
void main(){
	int a[] = { 1, 5, 7, 3, 8, 2, 4, 0, 9, 6 };

	merge_sort(a, 0, 9);

	for (int i = 0; i < 10; ++i)
		cout<<a[i]<<endl;

	system("pause");
}