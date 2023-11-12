#include <iostream>
#include <fstream>
#include <vector>

using namespace std;

int recursiveBestApplicant(
      const vector<pair<float, float> >& applicants, 
      vector< pair<float, int> >& indices, vector<int>& eligible, int first, int last) {

	if(first >= last) {
		eligible.push_back(indices.at(last).second);
		return last;
	}
	int mid = (first + last) / 2;
	// Call for second half.
	int index_best_category2 = recursiveBestApplicant(applicants, indices, eligible, mid + 1, last);
	for(int temp = mid; temp >= first; temp--) {
		int i = indices.at(temp).second;
		if(applicants.at(i).second < applicants.at(index_best_category2).second) {
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
   vector< pair<float, int> > indices;
	for(int i = 0; i < applicants.size(); i++) {
		indices.push_back(pair<float, int>(applicants.at(i).first, i));
	} // O(2n)
	sort(indices.begin(), indices.end()); // O(nlogn)

	vector<int> eligible;
	int S1 = recursiveBestApplicant(applicants, indices, eligible, 0, indices.size() - 1);
	return eligible;
}
