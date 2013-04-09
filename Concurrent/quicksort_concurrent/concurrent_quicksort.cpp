// Concurrent quicksort

#include <iostream>
#include <pthread.h>
#include <stdlib.h>
#include <fstream>
using namespace std;

int thread_num = 0;

struct ThreadArgs{
	int st, ed;
	int* arr;

	ThreadArgs(int i,int j,int *a){
		st = i; ed = j;
		arr = a;
	}	
};

void *quicksort(void* args){
	thread_num++;
	ThreadArgs *data = (ThreadArgs*)args;
	int st = data->st;
	int ed = data->ed;
	int *a = data->arr;
//	cerr << "[Thread " << (unsigned int)pthread_self() << " ] st: " << st << " ed: " << ed << endl;
	if (st>=ed){ 
	//	cerr << "st>=ed" << endl;
		pthread_exit(NULL);
	}
	int pivot = st;
	int tmp;
	for (int i=st+1;i<=ed;i++){
		if (a[i] < a[st]){
			pivot++;
			if (pivot!=i){
				tmp = a[pivot];	
				a[pivot] = a[i];
				a[i] = tmp;
			}
		}
	}
	if (pivot != st){
		tmp = a[pivot];
		a[pivot] = a[st];
		a[st] = tmp;
	}
	pthread_t t1,t2;
	
	ThreadArgs *args1 = new ThreadArgs(st, pivot - 1, a);
	ThreadArgs *args2 = new ThreadArgs(pivot+1, ed, a);
	
	int tid1 = pthread_create(&t1, NULL, quicksort, (void *)args1);
	
	if (tid1){
		cerr << "Error in creating thread" << endl;
	}
	int tid2 = pthread_create(&t2, NULL, quicksort, (void *)args2);
	if (tid2){
		cerr << "Error in creating thread" << endl;
	}
	int rc1, rc2;
	void *status;
	rc1 = pthread_join(t1, &status);
	if (rc1){
		cerr << "Failed when join" << endl;
	}
	rc2 = pthread_join(t2, &status);
	if (rc2){
		cerr << "Failed when join" << endl;
	}
	
	delete args1;	
	delete args2;
	pthread_exit(NULL);

}

int main(){
	int a[10000];
	int tmp;
	int n = 0;
	cout << "File name of numbers: ";
       	string filename;
	cin >> filename;	
	ifstream fin(filename.c_str());
	if (!fin){
		cerr << "Open file fail: " << filename << endl;
		exit(EXIT_FAILURE);
	}
	ofstream fout("Sort_Result.txt");
	fin >> tmp;
	while (!fin.eof()){
		a[n] = tmp;
		n++;
		fin >> tmp;
	}
	/*	
	for (int i=0;i<n;i++){
		cout << a[i] << endl;
	}
	*/
	ThreadArgs *main_args = new ThreadArgs(0,n-1,a);
	pthread_t first;
	int rc = pthread_create(&first, NULL, 
			 	quicksort, (void*)main_args);
	
	if (rc)	{
		cerr << "Error: Unable to start quicksort thread in main" << endl;
		exit(EXIT_FAILURE);
	}
	void* status;
	int rc_j = pthread_join(first, &status);
	
	cout << "Total thread number: " << thread_num << endl;
	delete main_args;
	if (rc_j){
		cerr << "Failed when joining main thread" << endl;
	        exit(EXIT_FAILURE);	
	}
	int sorted = true;
	for (int i=0;i<n-1;i++){
		if (a[i] > a[i+1]){
			sorted = false;
			break;
		}
	}
	if (!sorted){
		cerr << "Not sorted!" << endl;
	}
	else{
		cout << "Sorted!" << endl;
		for (int i=0;i<n;i++){
			fout << a[i] << endl;
		}
	}
	fin.close();
	fout.close();
	return 0;
}
