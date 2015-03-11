#include <iostream>
#include <vector>
using namespace std;

vector<int>::iterator Cp9_12(vector<int>::iterator begin, vector<int>::iterator end, int val){
	while(begin != end){
		if(*begin == val)
			break;
		else
			++begin;
	}
	return begin;
}
void main(){
	int val1 = 7;
	int val2 = 10;
	int a[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
	vector<int> ivec(a, a+10);
	vector<int>::iterator it1 = Cp9_12(ivec.begin(), ivec.end(), val1);
	vector<int>::iterator it2 = Cp9_12(ivec.begin(), ivec.end(), val2);
	if (it1 != ivec.end())
		cout << "true" << endl;
	else
		cout << "false" << endl;
	if (it2 != ivec.end())
		cout << "true" << endl;
	else
		cout << "false" << endl;
	system("pause");
}
