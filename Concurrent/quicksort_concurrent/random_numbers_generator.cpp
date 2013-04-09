#include <fstream>
#include <iostream>
#include <cstdlib>
#include <string>
#include <time.h>

using namespace std;

int main(){
	string filename;
	int num, max_num;
	cout << "Input the number of numbers: ";
	cin >> num;
	cout << "Max number: ";
	cin >> max_num;
	while (max_num <= 0){
		cout << "Max number should be greater than 0! " << endl;
		cout << "Input another one: ";
		cin >> max_num;
	}
	cout << "Input the file name to save numbers:  " << endl;
	cin >> filename;

	ofstream fout(filename.c_str());
	
	srand(time(0));
	int *a = new int[num];
	for (int i=0;i<num;i++){
		a[i] = rand() % max_num;	
	}

	for (int i=0;i<num;i++){
		fout << a[i] << ' ';
	}
	
	delete []a;
	fout.close();
	return 0;
}
