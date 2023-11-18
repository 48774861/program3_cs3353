#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <chrono>

using namespace std;

vector<int> BestApplicants(const vector<pair<float, float> >& applicants);

vector<int> naiveBestApplicants(const vector<pair<float, float> >& applicants)
{
   vector<int> sol;

   for (int i = 0; i < applicants.size(); i++)
	{
	    bool eligible = true;
	    for (int j = 0; j < applicants.size(); j++)
		{
		   if ((i != j) && (applicants[i].first < applicants[j].first) && (applicants[i].second > applicants[j].second))
			{
			    eligible = false;
			    break;
			}
		}
	    if (eligible)
		sol.push_back(i);
	}
   return sol;
}

int main(int argc, char** argv)
{
  int n;
   

  //ifstream ifile(argv[1]);
  ifstream ifile("../mixed_2000.txt");
  vector<pair<float, float> > applicants;

  ifile >> n;
  
  cout << "n : " << n << endl;
  for (int i = 0; i < n; i++)
	{
  	   float p, q;
	   ifile >> p >> q;
	   applicants.push_back(make_pair(p, q));
	}

  auto start2 = chrono::high_resolution_clock::now();
  vector<int> sol2 = BestApplicants(applicants);
  auto end2 = chrono::high_resolution_clock::now();
  auto elapsed2 = chrono::duration_cast<chrono::microseconds>(end2 - start2);
  cout << "Time Duration of Recursive Algorithm: " << elapsed2.count() << "\n";

  sort(sol2.begin(), sol2.end());
  cout << "Solution for Recursive : ";
  for (int i = 0; i < sol2.size(); i++)
	cout << sol2[i] << " ";
  cout << endl;

  auto start1 = chrono::high_resolution_clock::now();
  vector<int> sol = naiveBestApplicants(applicants);
  auto end1 = chrono::high_resolution_clock::now();
  auto elapsed1 = chrono::duration_cast<chrono::microseconds>(end1 - start1);
  cout << "Time Duration of Naive Algorithm: " << elapsed1.count() << "\n";


  sort(sol.begin(), sol.end());
  cout << "Solution for Naive : ";
  for (int i = 0; i < sol.size(); i++)
	cout << sol[i] << " ";
  cout << endl;

  bool good = true;
  for(int i = 0; i < sol.size(); i++) {
    if(sol[i] != sol2[i]) {
      cout << "Naive Solution has " << sol[i] << " while Recursive has " << sol2[i] << "\n";
      good = false;
      break;
    }
  }
  if (good == false) {
    cout << "Gotta fix something!\n";
  } else {
    cout << "The solutions match!\n";
  }


  return 0;
}
