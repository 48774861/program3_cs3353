#include <iostream>
#include <fstream>
#include <vector>
#include <chrono> // Remove Later

using namespace std;

vector<int> merge(vector<int> left, vector<int> right, const vector<pair<float, float> >& applicants) {
	size_t ileft = 0, iright = 0;
	vector<int> results;
	while (ileft < left.size() && iright < right.size())
	    if (applicants.at(left[ileft]).first < applicants.at(right[iright]).first)
	        results.push_back(left[ileft++]);
	    else
	        results.push_back(right[iright++]);
	while (ileft  < left.size() ) results.push_back(left [ileft++ ]);
	while (iright < right.size()) results.push_back(right[iright++]);
	return results;
}

vector<int> mergeSort(vector<int>& arr, const vector<pair<float, float> >& applicants) {
	if (arr.size() <= 1)
		return arr;
	int len = arr.size() / 2;
	vector<int> left (arr.begin(), arr.begin() + len);
	vector<int> right(arr.begin() + len, arr.end());
	return merge(mergeSort(left, applicants), mergeSort(right, applicants), applicants);
}

int partition(vector<int>& arr, const vector<pair<float, float> >& applicants, int start, int end)
{
 
    float pivot = applicants.at(arr[start]).first;
 
    int count = 0;
    for (int i = start + 1; i <= end; i++) {
        if (applicants.at(arr[i]).first <= pivot)
            count++;
    }
 
    // Giving pivot element its correct position
    int pivotIndex = start + count;
    swap(arr[pivotIndex], arr[start]);
 
    // Sorting left and right parts of the pivot element
    int i = start, j = end;
 
    while (i < pivotIndex && j > pivotIndex) {
 
        while (applicants.at(arr[i]).first <= pivot) {
            i++;
        }
 
        while (applicants.at(arr[j]).first > pivot) {
            j--;
        }
 
        if (i < pivotIndex && j > pivotIndex) {
            swap(arr[i++], arr[j--]);
        }
    }
 
    return pivotIndex;
}
 
void quickSort(vector<int>& arr, const vector<pair<float, float> >& applicants, int start, int end)
{
 
    // base case
    if (start >= end)
        return;
 
    // partitioning the array
    int p = partition(arr, applicants, start, end);
 
    // Sorting the left part
    quickSort(arr, applicants, start, p - 1);
 
    // Sorting the right part
    quickSort(arr, applicants, p + 1, end);
}

int recursiveBestApplicant(
      const vector<pair<float, float> >& applicants,
      vector<int>& indices, vector<int>& eligible, int first, int last) {

	if(first >= last) {
		eligible.push_back(indices.at(last));
		return indices.at(last);
	}
	int mid = (first + last) / 2;
	// Call for second half.
	int index_best_category2 = recursiveBestApplicant(applicants, indices, eligible, mid + 1, last);
	for(int temp = mid; temp >= first; temp--) {
		int i = indices.at(temp);
		if(applicants.at(i).second < applicants.at(index_best_category2).second) {
         // std::cout << "Comparing index " << i << " and " << index_best_category2 << "\n";
         // std::cout << applicants.at(i).second << " < " << applicants.at(index_best_category2).second << "\n";
			if(applicants.at(i).first == applicants.at(index_best_category2).first) {
				eligible.back() = i;
			} else {
				eligible.push_back(i);
			}
			index_best_category2 = i;
		}
	}
	return index_best_category2;
}

vector<int> BestApplicants(const vector<pair<float, float> >& applicants)
{
   // 73 in total before
   auto start2 = chrono::high_resolution_clock::now();
   //vector< pair<float, int> > indices;
   vector<int> indices;
	for(int i = 0; i < applicants.size(); i++) {
		//indices.push_back(pair<float, int>(applicants.at(i).first, i));
      indices.push_back(i);
	} // O(2n). Indices takes 4 seconds out of 91.
   auto end2 = chrono::high_resolution_clock::now();
  auto elapsed2 = chrono::duration_cast<chrono::microseconds>(end2 - start2);
  cout << "Time Duration of Indices: " << elapsed2.count() << "\n";

   start2 = chrono::high_resolution_clock::now();
   // Sorting takes 77 out of the 91 seconds!
	//sort(indices.begin(), indices.end()); // O(nlogn)
   quickSort(indices, applicants, 0, indices.size() - 1);
   end2 = chrono::high_resolution_clock::now();
  elapsed2 = chrono::duration_cast<chrono::microseconds>(end2 - start2);
  cout << "Time Duration of Sorting: " << elapsed2.count() << "\n";

   for(int i = 0; i < indices.size(); i++) {
      std::cout << "Index: " << indices.at(i) << " " << applicants.at(indices.at(i)).first << " " << applicants.at(indices.at(i)).second << "\n";
   }

	vector<int> eligible;
   start2 = chrono::high_resolution_clock::now();
   // Takes 2 out of the 91 seconds.
	int S1 = recursiveBestApplicant(applicants, indices, eligible, 0, indices.size() - 1);
   end2 = chrono::high_resolution_clock::now();
  elapsed2 = chrono::duration_cast<chrono::microseconds>(end2 - start2);
  cout << "Time Duration of Recursion: " << elapsed2.count() << "\n";
	return eligible;
}
