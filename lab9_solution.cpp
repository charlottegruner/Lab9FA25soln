#include <iostream>
#include <iomanip>
#include <fstream>
#include <vector>
#include <string>
#include <algorithm>
#include <omp.h>
#include <ctime>

using namespace std;

bool readFromFile(vector<string>&);
void parallelBubbleSort(vector<string>&);

int main() 
{
    vector<string> str;
    double compute_time_p;
    clock_t tStart, tStop;
    const int threads_wanted = 4;
	
	cout << setprecision(3) << fixed;
	
	//force setting the number of threads to 10
    omp_set_dynamic(false); //don't allow it to overwrite setting of threads
    omp_set_num_threads(threads_wanted); //set threads

	//lambda (inline) function for printing elements 
	auto print = [](const string& n) { cout << n << endl; };

	if(readFromFile(str)) {

		tStart = clock();
		
		// Parallel string modification with traditional for loop
		#pragma omp parallel for
		for (unsigned int i=0; i<str.size(); i++) {
			int count = str[i].length();
			str[i] = to_string(count) + "_" + str[i];
		}
		
		// Parallel sort
		parallelBubbleSort(str);
		
		//Uncomment the following code to ensure the sort function is working correctly
		//cout << "\n\nAfter sorting:\n";
		//print vector elements by calling the lamda function within the for_each function
		//for_each(str1.cbegin(), str1.cend(), print);

		tStop = clock();
		compute_time_p = (static_cast<double>(tStop - tStart)/CLOCKS_PER_SEC);

		cout << "\n\n******************************************************************************\n";
		cout << "Computation time for OpenMP parallel = " << compute_time_p << " seconds." << endl << endl;
	}
    return 0;
}

/*
	Function:	readFromFile()
	Purpose:	Read each word from the words.txt text file and place
				in the str vector, which was passed by reference by calling
				the Vector push_back function.  push_back adds one new element
				at the end of the vector and places the argument sent to 
				the push_back function in that new element.
*/
bool readFromFile(vector<string>& str)
{
    ifstream file;
	string filename;
    string line;
	bool success;
	
	cout << "\nEnter the name of the text file: ";
	cin >> filename;
	file.open(filename);
	
	if(file.is_open()) {
		while (getline(file, line)) {
			str.push_back(line);
		}
		success = true;
	}
	else{
		cout << "File couldn't be opened!\n";
		success = false;
	}
	return success;
}

/*
	Function: parallelBubbleSort
	Purpose:
	The parallelBubbleSort function performs a parallel version of bubble sort on a vector of strings using OpenMP to accelerate the process by taking advantage of multiple CPU threads.
	
	// Using (||:swapped) will set swapped to true if one out of the two for loops sets swapped to true. (using logical OR)
	
*/
void parallelBubbleSort(vector<string>& v)
{
    int n = v.size();
	bool swapped = true;
	
	while(swapped){
        swapped = false;		
		
		#pragma omp parallel
		{
			#pragma omp for reduction(||:swapped)
			//bubble sort even elements
			for (int i=0; i<n-1; i+=2) {
				if (v[i] > v[i+1]) {
					string temp = v[i];
					v[i] = v[i+1];
					v[i+1] = temp;
					swapped = true;
				}
			}
			#pragma omp for reduction(||:swapped)
			//bubble sort odd elements
			for (int i=1; i<n-1; i+=2) {
				if (v[i] > v[i+1]) {
					string temp = v[i];
					v[i] = v[i+1];
					v[i+1] = temp;
					swapped = true;
				}
			}
		}
	}
}