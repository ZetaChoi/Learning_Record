#include <iostream>
#include <vector>
#include <list>
#include <deque>
using namespace std;

void main(){
	int i[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
	list <int> ils(i, i+10);
	deque <int> ide1,ide2;
	for(list <int>::iterator it = ils.begin(); it != ils.end(); ++it){
		if ((*it % 2) == 0)
			ide2.insert (ide2.end(), *it);
		else
			ide1.push_back (*it);
	}
	for (deque <int>::iterator it = ide1.begin(); it != ide1.end(); ++it) {
		cout<< *it << " ";
	}
	cout<<endl;
	for (deque <int>::iterator it = ide2.begin(); it != ide2.end(); ++it) {
		cout<< *it << " ";
	}
	cout<<endl;
	system("pause");
}