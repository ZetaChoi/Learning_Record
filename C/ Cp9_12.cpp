#include <iostream>
#include <vector>
using namespace std;

bool Cp9_12(vector<int>::iterator begin, vector<int>::iterator end, int val){
	while(begin != end){
		if(*begin == val)
			return true;
		else
			++begin;
	}
	return false;
}
void main(){
	int val1 = 7;
	int val2 = 10;
	int a[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
	vector<int> ivec(a, a+10);
	bool b1 = Cp9_12(ivec.begin(), ivec.end(), val1);
	bool b2 = Cp9_12(ivec.begin(), ivec.end(), val2);
	cout<< "b1=" << b1 << ", b2=" << b2 <<endl;
	system("pause");
}
