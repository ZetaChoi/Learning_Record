#include <iostream>
#include <vector>
#include <list>
#include <deque>
#include <string>
using namespace std;

void main(){
	char* ca[] = {"mary", "tom", "bob", "alice"};
	list <char*> clst(ca, ca+4);
	vector <string> svec;
	svec.assign(clst.begin(), clst.end());
	for(vector <string>::iterator it = svec.begin(); it != svec.end(); ++it){
		cout << *it << " ";
	}
	cout << endl;
	system("pause");
}