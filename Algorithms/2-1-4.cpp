/*
Ä£·Â¼Ä´æÆ÷
*/

#include <stdio.h>
#include <iostream>
using namespace std;

int cf = 0;

int add(int a, int b){
	
	if (a + b + cf == 0){
		cf = 0;
		return 0;
	}
	else if (a + b + cf == 1){
		cf = 0;
		return 1;
	}
	else if (a + b + cf == 2){
		cf = 1;
		return 0;
	}
	else{
		cf = 1;
		return 1;
	}
}

void main(){
	int a[] = { 1, 0, 0, 1 };
	int b[] = { 1, 0, 1, 1 };
	int c[] = {0, 0, 0, 0, 0};
	for(int i = 3; i>=0; --i){
		c[i+1] = add(a[i], b[i]);
	}
	c[0] = cf;

	for(int i = 0; i < 5; ++i)
		cout<<c[i];

	system("pause");
}