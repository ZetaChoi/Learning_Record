#include <stdio.h>
#include <iostream>
using namespace std;

/*
	初始化:a[i] == a[0]
	保持:对比 a[i]与val
	终止:val == a[i]则跳出循环,否则输出NIL
*/
void main(){
	int a[] = { 1, 5, 7, 3, 8, 2, 4, 0, 9, 6 };
	int val = 11;
	
	for(int i = 0; i<10; ++i){
		if (val == a[i]){
			cout<< i <<endl;
			break;
		}
		if (i == 9)
			cout<< "NIL" <<endl;
	}

	system("pause");
}