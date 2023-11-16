#include <iostream>
#include <fstream>
#include <vector>
#include <stack>
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

    int temp = arr[pivotIndex];
    arr[pivotIndex] = arr[start];
    arr[start] = temp;
 
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
            temp = arr[i];
            arr[i] = arr[j];
            arr[j] = temp;
            i++;
            j--;
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
    // auto start2 = chrono::high_resolution_clock::now();
    // Partitioning on its own takes a very short time.
    int p = partition(arr, applicants, start, end);
    // auto end2 = chrono::high_resolution_clock::now();
    // auto elapsed2 = chrono::duration_cast<chrono::microseconds>(end2 - start2);
    // cout << "Time Duration of Partitioning " << start << " to " << end << ": " << elapsed2.count() << "\n";
 
    // Sorting the left part
    // start2 = chrono::high_resolution_clock::now();
    quickSort(arr, applicants, start, p - 1);
//     end2 = chrono::high_resolution_clock::now();
//   elapsed2 = chrono::duration_cast<chrono::microseconds>(end2 - start2);
//   cout << "Time Duration of in-Sorting " << start << " to " << end << ": " << elapsed2.count() << "\n";
 
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
    pair<float, float> best = applicants.at(index_best_category2);
	for(int temp = mid; temp >= first; temp--) {
		int i = indices.at(temp);
		// if(applicants.at(i).second < applicants.at(index_best_category2).second) {
        //  // std::cout << "Comparing index " << i << " and " << index_best_category2 << "\n";
        //  // std::cout << applicants.at(i).second << " < " << applicants.at(index_best_category2).second << "\n";
		// 	if(applicants.at(i).first == applicants.at(index_best_category2).first) {
		// 		eligible.back() = i;
		// 	} else {
		// 		eligible.push_back(i);
		// 	}
		// 	index_best_category2 = i;
		// }
        if(applicants.at(i).second > best.second && applicants.at(i).first < best.first) {
            continue;
        }
        index_best_category2 = i;
        eligible.push_back(i);
        best = applicants.at(index_best_category2);
        // if(applicants.at(i).second < applicants.at(index_best_category2).second) {
        //  // std::cout << "Comparing index " << i << " and " << index_best_category2 << "\n";
        //  // std::cout << applicants.at(i).second << " < " << applicants.at(index_best_category2).second << "\n";
		// 	if(applicants.at(i).first == applicants.at(index_best_category2).first) {
		// 		eligible.back() = i;
		// 	} else {
		// 		eligible.push_back(i);
		// 	}
		// 	index_best_category2 = i;
        //     continue;
		// }
        // if(applicants.at(i).second == applicants.at(index_best_category2).second && applicants.at(i).first == applicants.at(index_best_category2).first) {
        //  // std::cout << "Comparing index " << i << " and " << index_best_category2 << "\n";
        //  // std::cout << applicants.at(i).second << " < " << applicants.at(index_best_category2).second << "\n";
		// 	eligible.push_back(i);
		// 	index_best_category2 = i;
		// }
	}
	return index_best_category2;
}

vector<int> BestApplicants(const vector<pair<float, float> >& applicants)
{
    vector<int> finalSolution;

    // Finding the Max is faster than the Recursion itself!
    int index_bestwpm = 0;
    int index_bestipm = 0;
    auto best_wpm = applicants.at(index_bestwpm);
    auto best_ipm = applicants.at(index_bestipm);
	
	// First value is WPM, second value is IPM.
	for(int i = 1; i < applicants.size(); i++) {
		auto& current = applicants.at(i);
        
		// Chooses the best wpm, or if equal wpms, which one has the better ipm.
		if(current.first > best_wpm.first || (current.first == best_wpm.first && current.second < best_wpm.second)) {
			index_bestwpm = i;
            best_wpm = applicants.at(index_bestwpm);
		}
		// Chooses the best ipm or, if equal ipms, which one has the better wpm.
		if(current.second < best_ipm.second || (current.second == best_ipm.second && current.first > best_ipm.first)) {
			index_bestipm = i;
            best_ipm = applicants.at(index_bestipm);
		}
	}
//   cout << "Best WPM is at index " << index_bestwpm << " at " << best_wpm.first << "\n";
//   cout << "Best IPM is at index " << index_bestipm << " at " << best_ipm.second << "\n";
   

   vector<int> indices;
	for(int i = 0; i < applicants.size(); i++) {
		//indices.push_back(pair<float, int>(applicants.at(i).first, i));
        if(applicants.at(i).first >= best_ipm.first && applicants.at(i).second <= best_wpm.second)
            indices.push_back(i);
	} // O(2n). Indices takes 4 seconds out of 91.

    quickSort(indices, applicants, 0, indices.size() - 1);

//    for(int i = 0; i < indices.size(); i++) {
//       std::cout << "Index: " << indices.at(i) << " " << applicants.at(indices.at(i)).first << " " << applicants.at(indices.at(i)).second << "\n";
//    }

	vector<int> eligible;
	int S1 = recursiveBestApplicant(applicants, indices, eligible, 0, indices.size() - 1);
	return eligible;
}
