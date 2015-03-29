#include <stdio.h>
#include <iostream>
using namespace std;

void binary_search(int a[], int p, int r, int val){
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
}

void main(){
	int b[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};

	binary_search(b, 0, 9, -3);
	system("pause");
}