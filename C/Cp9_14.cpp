#include <iostream>
#include <vector>
#include <string>
using namespace std;

void main(){
	string s;
	vector <string> svec;
	while (cin >> s){
		svec.push_back(s);
	}
	for (vector <string>::iterator it = svec.begin(); it != svec.end(); ++it)
		cout << *it << endl;
	system("pause");
}
