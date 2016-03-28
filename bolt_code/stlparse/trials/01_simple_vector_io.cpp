//Demonstrating Vector IO using standard library
//
//Test program - Enter 3 components of a vector and output them using iterators
//
//Written by - Brahm Prakash Mishra, 28-03-2016
//

#include <iostream>
#include <vector>

using namespace std;

int main()
{

	vector<int> vector;
	cout<<"Program starts here\n";
	
	for(int counter=0; counter<3; counter++)
	{
		cout<<"Value of counter is at : "<<counter<<"\n";
		cout<<"Enter Components : ";
		int x;
		cin>>x;
		vector.push_back(x);
	}
	cout<<"\nOuting vector : ";
	for(auto iter=begin(vector); iter!=end(vector); iter++)
	{

		cout<<*iter<<" ";
	}
	
}
