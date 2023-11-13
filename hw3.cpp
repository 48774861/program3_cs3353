#include <iostream>
#include <fstream>
#include <vector>
#include <chrono> // Remove Later

using namespace std;

int recursiveBestApplicant(
      const vector<pair<float, float> >& applicants, 
      vector< pair<float, int> >& indices, vector<int>& eligible, int first, int last) {

	if(first >= last) {
		eligible.push_back(indices.at(last).second);
		return indices.at(last).second;
	}
	int mid = (first + last) / 2;
	// Call for second half.
	int index_best_category2 = recursiveBestApplicant(applicants, indices, eligible, mid + 1, last);
	for(int temp = mid; temp >= first; temp--) {
		int i = indices.at(temp).second;
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
   vector< pair<float, int> > indices;
	for(int i = 0; i < applicants.size(); i++) {
		indices.push_back(pair<float, int>(applicants.at(i).first, i));
	} // O(2n). Indices takes 4 sseconds out of 91.
   auto end2 = chrono::high_resolution_clock::now();
  auto elapsed2 = chrono::duration_cast<chrono::microseconds>(end2 - start2);
  cout << "Time Duration of Indices: " << elapsed2.count() << "\n";

   start2 = chrono::high_resolution_clock::now();
   // Sorting takes 77 out of the 91 seconds!
	sort(indices.begin(), indices.end()); // O(nlogn)
   end2 = chrono::high_resolution_clock::now();
  elapsed2 = chrono::duration_cast<chrono::microseconds>(end2 - start2);
  cout << "Time Duration of Sorting: " << elapsed2.count() << "\n";

   // for(int i = 0; i < indices.size(); i++) {
   //    std::cout << "Index: " << indices.at(i).second << " " << indices.at(i).first << " " << applicants.at(indices.at(i).second).second << "\n";
   // }

	vector<int> eligible;
   start2 = chrono::high_resolution_clock::now();
   // Takes 2 out of the 91 seconds.
	int S1 = recursiveBestApplicant(applicants, indices, eligible, 0, indices.size() - 1);
   end2 = chrono::high_resolution_clock::now();
  elapsed2 = chrono::duration_cast<chrono::microseconds>(end2 - start2);
  cout << "Time Duration of Recursion: " << elapsed2.count() << "\n";
	return eligible;
}
