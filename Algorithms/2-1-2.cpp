#include <stdio.h>
#include <iostream>
using namespace std;

void insertion_sort(int a[]){
	for (int i = 1; i <10; ++i){
		int j = i - 1;
		int key = a[i];
		while ( j >= 0 && (key > a [j]) ){
			a[j + 1] = a[j];
			--j;
		}
		a[j+1] = key;
	}
}
void main(){
	int a[] = { 1, 5, 7, 3, 8, 2, 4, 0, 9, 6 };
	insertion_sort(a);

	for (int i = 0; i < 10; ++i)
		cout<<a[i]<<endl;
	system("pause");
}