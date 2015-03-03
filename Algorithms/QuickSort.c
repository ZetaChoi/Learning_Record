#include <iostream>
#include <fstream>
using namespace std;

int array[10000];
unsigned int count=0;

void readFile(){
	ifstream infile("QuickSort.txt");
	int i=0;
	while(infile>>array[i++]);
	infile.close();
}

int quick_first(int *a,int first,int last){
	count+=last-first;

	int i=first+1,pivot=a[first];
	for(int j=first+1;j<=last;j++){
		if(pivot>=a[j]){
			swap(a[i],a[j]);
			i++;
		}
	}
	swap(a[first],a[i-1]);

	return i-1;
}

int quick_last(int *a,int first,int last){
	//count+=last-first;

	int pivot=a[last];
	swap(a[last],a[first]);
	int i=first+1;
	for(int j=first+1;j<=last;j++){
		if(pivot>=a[j]){
			swap(a[i],a[j]);
			i++;
		}
	}
	swap(a[first],a[i-1]);

	return i-1;
}

int med3(int *x,int a,int b,int c){
	return	x[a] < x[b] ? (x[b] < x[c] ? b : x[a] < x[c] ? c : a): x[b] > x[c] ? b : x[a] > x[c] ? c : a;
}

int quick_median(int *a,int first,int last){
	count+=last-first;

	int middle=((last-first)/2)+first;		
	
	swap(a[first],a[med3(a,first,middle,last)]);

	int pivot=a[first];
	int i=first+1;
	for(int j=first+1;j<=last;j++){
		if(pivot>=a[j]){
			swap(a[i],a[j]);
			i++;
		}
	}
	swap(a[first],a[i-1]);

	return i-1;
}

void quickSort(int *a,int first,int last){
	if(first<last){

		//int middle=quick_first(a,first,last);
		//int middle=quick_last(a,first,last);
		int middle=quick_median(a,first,last);

		quickSort(a,first,middle-1);
		quickSort(a,middle+1,last);
	}
}
int main(){
	readFile();
	quickSort(array,0,9999);
	cout<<count<<endl<<endl;
	//for(int i=0;i<10;i++)
	//cout<<array[i]<<endl;

	system("pause");
}
