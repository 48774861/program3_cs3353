#include <iostream>
#include <fstream>
#include <vector>

using namespace std;

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

/**
 * Quick Sorts the arr array where each element is the index of an element in applicants.
 * arr array is sorted based on the corresponding values of applicants.at(arr[i]).
 */
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

/**
 * Assumes that the vector indices is sorted based on the corresponding values of applicants.at(indices.at(i));
 * Returns the index of the applicant with the best IPM (category 2).
*/
int recursiveBestApplicant(
      const vector<pair<float, float> >& applicants,
      vector<int>& indices, vector<int>& eligible, int first, int last, int& index_previous_best) {

    // The recursive method reaches the last element of indices (which is the index of the applicant with the best WPM).
    // We add the index of this applicant to the list of eligible applicants and return the applicant's index.
	if(first >= last) {
		eligible.push_back(indices.at(last));
		return indices.at(last);
	}

    // Separates the applicants into two groups and finds the index of the applicant with best IPM in the second group.
	int mid = (first + last) / 2;
	int index_best_category2 = recursiveBestApplicant(applicants, indices, eligible, mid + 1, last, index_previous_best);

    pair<float, float> previous_best;
    if(index_previous_best == -1) {
        previous_best = pair<float, float>(
	    std::numeric_limits<float>::min(), std::numeric_limits<float>::max());
    } else {
        previous_best = applicants.at(index_previous_best);
    }

    pair<float, float> best = applicants.at(index_best_category2);

    // Go through each eligible candidate from the best WPM (first category) to the worst WPM.
    for(int temp = mid; temp >= first; temp--) {
        int i = indices.at(temp);
        if (applicants.at(i).first < best.first) { //Current is worse in the first category.
            if (applicants.at(i).second > best.second) {
                continue; // If current is worse in second category, do nothing.
            }
            eligible.push_back(i);
            if (applicants.at(i).second < best.second) {
                index_previous_best = index_best_category2;
                previous_best = best;
                index_best_category2 = i;
                best = applicants.at(index_best_category2);
            }
        } else { // If equal
            // Need to check for cases where all the category 1's are equal.
            if (applicants.at(i).second > previous_best.second) {
                continue; // If current is worse in second category, do nothing.
            }
            eligible.push_back(i);
            // Keep the index of best category, but do not update the previous best.
            // Previous best is kept the same to check for all category 1's that are equal.
            if (applicants.at(i).second < best.second) {
                index_best_category2 = i;
                best = applicants.at(index_best_category2);
            }
        }
    }
	return index_best_category2;
}

vector<int> BestApplicants(const vector<pair<float, float> >& applicants)
{
    vector<int> finalSolution;
    // Finds the applicant with the best WPM and the best IPM.
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
   
    /**
        Adds the applicant to be considered for eligibility if it meets two conditions.
        (1) It has a better IPM than the applicant with the best WPM.
        (2) It has a better WPM than the applicant with the best IPM.
        This step significantly reduces the runtime of the sorting algorithm, which takes most of the time.
    */
    vector<int> indices;
	for(int i = 0; i < applicants.size(); i++) {
        auto& applicant = applicants.at(i);
        if((applicant.first >= best_ipm.first && applicant.second <= best_wpm.second) || applicant.second == best_ipm.second || applicant.first == best_wpm.first) {
            indices.push_back(i);
            continue;
        }
	}

    // Sort the applicants by the first category. O(nlogn)
    quickSort(indices, applicants, 0, indices.size() - 1);

    // Fill up the eligible vector with all the eligible applicants.
	vector<int> eligible;
    int previous_best = -1;
	int S1 = recursiveBestApplicant(applicants, indices, eligible, 0, indices.size() - 1, previous_best);
	return eligible;
}
