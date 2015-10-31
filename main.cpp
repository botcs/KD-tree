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
		/*
		kdtree<2, int, char> myTree;
		using pt =vector<int>;
		myTree.insert(pt{5,5}, '3');
		myTree.insert(pt{7,7}, '4');
		myTree.insert(pt{2,3}, '1');
		myTree.insert(pt{7,4}, '2');
		myTree.insert(pt{2,8}, '5');
		myTree.print();
		cout<<"\n\n";

		//cout<<myTree.nearest_neighbour(pt{2,1},'?').val();

		for(auto& it : myTree.n_nearest_neighbour(5, pt{2,1}, '?'))
			cout<<it.val()<<'\t';

		cout<<"\n\n";
		for(auto& it : myTree.n_nearest_neighbour(3, pt{2,1}, '?'))
			cout<<it.val()<<'\t';

		*/


		kdtree<3, double, string> kdt, balanced_kdt;
		ifstream f("COORDINATES.TXT");

		string object_name;
		double c1, c2, c3;
		vector<p> orig;



		auto t = clock();
		while (f>>object_name>>c1>>c2>>c3)
		{
			kdt.insert(c{c1,c2,c3}, object_name);
			orig.push_back(p{c{c1,c2,c3}, object_name});
		}

		cout 	<< "File reading finished in " << clock() - t << " miliseconds\n" << '\n'
				<< "Number of points: " << orig.size();
		vector<p*> xyz, yzx, zxy;

		for(auto& i : orig) xyz.push_back(&i);

		t = clock();
		auto PS = presort(orig);
		cout << "Sorting in 3 axes finished in " << clock() - t << " miliseconds\n";

		cout << "naive insert HEIGHT: " << kdt.height << '\n';
		kdt.print(cout);

		ins(balanced_kdt, PS[0], PS[1], PS[2]);
		cout << "balanced insert HEIGHT: " << balanced_kdt.height << '\n';
		balanced_kdt.print(cout);


		cout<<"TEST with large data\n ********************* \n";
		kdtree<3, double, string> test_kdt, test_unbal;
		vector<p> test;
		for(int i=0; i<20; i++)
			for(int j=0; j<66; j++)
				for(int k=0; k<100; k++)
					test.push_back(p{c{i,j,k}, ""});
		cout << test.size() <<" POINTS GENERATED\n";

		t=clock();
		unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
		shuffle (test.begin(), test.end(), std::default_random_engine(seed));
		cout << "Shuffled them in " << clock() - t << "miliseconds \n";

		/*for (auto& i : test) {
			cout<<"(";
			for (auto& c : i.first) cout<<c<<';';
			cout<<")\n";
		}*/

		t = clock();
		PS = presort(test);
		cout << "Sorting finished in " << clock() - t << " miliseconds\n";

		t = clock();
		ins(test_kdt, PS[0], PS[1], PS[2]);
		cout << "pre-balanced Inserting finished in " << clock() - t << " miliseconds\n";
		cout << "balanced insert HEIGHT: " << test_kdt.height << '\n';


		t = clock();
		for(auto& i : test)
			test_unbal.insert(i.first, i.second);
		cout << "unbalanced Inserting finished in " << clock() - t << " miliseconds\n";
		cout << "unbalanced insert HEIGHT: " << test_unbal.height << '\n';

		/*
		 * FUN NOTE:
		 * above 132 000 elements a random shuffle makes a better inserting order
		 * than the given pre-order algorithm (which should make the best order...)
		 *
		 * if have any ideas to track this down, contact me:
		 * botos.csaba@tdk.koki.mta.hu
		 *
		 */

		//test_unbal.print();

		cout<<"**************************************\n";

		//test_kdt.print();

	}


	catch (const exception& e)
	{
		cout << "HIBA: " << e.what() << endl;
	}
	return 0;
}
