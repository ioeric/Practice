#include <string>
#include <iostream>
#include <algorithm>
using namespace std;

void replaceFirst(string& str1, const string str2){
	string::size_type pos = str1.find(str2);
	if ( pos!= string::npos ){
		str1.replace(pos, str2.length(), "HAHAHA"  );
	}
}

void replaceAll(string& str1, const string str2){
	string::size_type pos = 0;
	while ( ( pos = str1.find(str2,pos) )!= string::npos ){
		str1.replace(pos, str2.length(), "HAHAHA");

	}
}



int main(){
	string str1, str2;
	string str3;
	cin >> str1;
	string str4 = str1;
	cout << "str1 capacity: " << str1.capacity() << endl;
	cin >> str2;
	cout << "str2 capacity: " << str2.capacity() << endl;
	cin >> str3;
	cout << "str3 capacity: " << str3.capacity() << endl;

	replaceFirst(str1, str3);
	cout << str1 << endl;
	cout << "str1 capacity: " << str1.capacity() << endl;
	replaceAll(str2, str3);
	cout << str2 << endl;
	cout << "str2 capacity: " << str2.capacity() << endl;
	std::replace(str4.begin(), str4.end(), '1', 'O');
	cout << str4 << endl;
	
	return 0;
	
}
