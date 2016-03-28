//Demonstrating Vector IO using standard library
//
//Test program - Enter 3 components of a vector and output them using iterators
//
//Written by - Brahm Prakash Mishra, 28-03-2016
//

#include <iostream>

using namespace std;

//#include <vector>
	//commented out vector library since we want to do vector IO with GLM library

#include "../glm/glm.hpp"


//code to overload << and >> operators to enable output and input of data into glm::vec3 type
//
ostream& operator<<(ostream &output, glm::vec3 &myVector)
{
	return output<<myVector.x<<" "<<myVector.y<<" "<<myVector.z;
}

istream& operator>>(istream &input, glm::vec3 &myVector)
{
	return input>>myVector.x>>myVector.y>>myVector.z;
}

//Code becomes much simpler than prog 01 after overload
int main()
{

	glm::vec3 myVector;
	cout<<"Program starts here\n";
	
	cin>>myVector;
	cout<<myVector;	
}
