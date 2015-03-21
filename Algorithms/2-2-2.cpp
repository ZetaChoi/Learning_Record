#include <stdio.h>
#include <iostream>
using namespace std;

void selection_sort(int a[]){
	int key;
	for (int i = 0; i <10-1; ++i){
		int min = 65535;
		for (int j = i; j <10; ++j){
			if (a[j] < min){
				min = a[j];
				key = j;
			}
		}
		swap(a[i], a[key]);
	}
}
void main(){
	int a[] = { 1, 5, 7, 3, 8, 2, 4, 0, 9, 6 };
	selection_sort(a);

	for (int i = 0; i < 10; ++i)
		cout<<a[i]<<endl;
	system("pause");
}