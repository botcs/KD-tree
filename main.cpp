//============================================================================
// Name        : main.cpp
// Author      : botcs
// Version     :
// Copyright   : bcs
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include <fstream>
#include <string>
#include "kdtree.h"
#include <vector>
#include <algorithm>
#include <utility>
#include <ctime>
#include <cmath>
#include <stack>

#include <random>
#include <chrono>
#include "presort.h"
using namespace std;

using c 	= vector<double>;
using p		= pair<c, string>;


int main() {

	try{


		cout<<"TEST with large data\n ********************* \n";
		kdtree<3, double, string> test_bal, test_unbal, new_test;
		vector<p> test;
		for(int i=0; i<20; i++)
			for(int j=0; j<20; j++)
				for(int k=0; k<20; k++)
					test.push_back(p{c{i,j,k}, ""});
        //test.push_back(p{c{0,0,0}, ""});
		cout << test.size() <<" POINTS GENERATED\n";

		auto t=clock();
		unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
		shuffle (test.begin(), test.end(), std::default_random_engine(seed));
		cout << "Shuffled them in " << clock() - t << "miliseconds \n";

		/*for (auto& i : test) {
			cout<<"(";
			for (auto& c : i.first) cout<<c<<';';
			cout<<")\n";
		}*/

		t = clock();
		auto PS = presort(test);
		cout << "Sorting finished in " << clock() - t << " miliseconds\n";

		t = clock();
		ins(test_bal, PS[0], PS[1], PS[2]);
		cout << "pre-balanced Inserting finished in " << clock() - t << " miliseconds\n";
		cout << "balanced insert HEIGHT: " << test_bal.height << '\n';


		t = clock();
		for(auto& i : test)
			test_unbal.insert(i.first, i.second);
		cout << "unbalanced Inserting finished in " << clock() - t << " miliseconds\n";
		cout << "unbalanced insert HEIGHT: " << test_unbal.height << '\n';


        t = clock();
		new_test.insert(test);
		cout << "new Inserting finished in " << clock() - t << " miliseconds\n";
        cout << "ITEM COUNT: " << test.size()<<" new_test size: "<< new_test.size()<<'\n';
		cout << "new insert HEIGHT: " << new_test.height << '\n';
		//test_unbal.print();
		//test_bal.print();
		cout<<" **************************************\n";




	}


	catch (const exception& e)
	{
		cout << "HIBA: " << e.what() << endl;
	}

	return 0;
}
