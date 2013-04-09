#include <iostream>
#include <algorithm>
#include <vector>

using namespace std;

void output(int x){
	cout << x << " ";
}
int main(){
	vector< int > v;
	int x;
	cin >> x;
	while (!cin.eof()){
		v.push_back(x);
		cin >> x;
	}

	make_heap(v.begin(), v.end());
	cout << "\nMax number: " << v.front() << endl;
	pop_heap(v.begin(), v.end());
	v.pop_back();
	cout << "Max after pop. " << v.front() << endl;
	v.push_back(70); 
	push_heap(v.begin(), v.end());
	cout << "Max after push" << v.front() << endl;
	cout << "Before sorted: " << endl;
	/*for_each(v.begin(), v.end(), [](int n){
		cout << n << ' ';
	});*/
	for_each(v.begin(), v.end(), output);

	cout << endl;

	sort_heap(v.begin(), v.end());
	cout << "After sorted: " << endl;
	/*for_each(v.begin(), v.end(), [](int n){
		cout << n << ' ';
	});*/
	cout << "Front after sort " << v.front() << endl;
	for_each(v.begin(), v.end(), output);
	
	cout << endl;

	return 0;
}
