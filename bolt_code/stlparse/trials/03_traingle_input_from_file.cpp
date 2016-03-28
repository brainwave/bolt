
#include <iostream>
#include <vector>
#include <fstream>
#include <string>
using namespace std;

//#include <vector>
	//commented out vector library since we want to do vector IO with GLM library

#include "../glm/glm.hpp"


//code to overload << and >> operators to enable output and input of data into glm::vec3 type
//
std::ostream& operator<<(std::ostream &output, glm::vec3 &myVector)
{
	return output<<myVector.x<<" "<<myVector.y<<" "<<myVector.z;
}

std::istream& operator>>(std::istream &input, glm::vec3 &myVector)
{
	return input>>myVector.x>>myVector.y>>myVector.z;
}

struct triangle{
	//Just defining an array of 3 vertices for now. Later will add functions, etc to it.
	triangle(glm::vec3 v0
		,glm::vec3 v1
		,glm::vec3 v2
		,glm::vec3 n)
		:normal(n) {
				vertices[0]=v0;
				vertices[1]=v1;
				vertices[2]=v2;
			}

	glm::vec3 vertices[3], normal;

};

struct triangleMesh{
	vector<triangle> mesh;
	void display(triangle t)
	{
	    cout<<"\n"<<t.vertices[0]
		<<" "<<t.vertices[1]
		<<" "<<t.vertices[2]
		<<" "<<t.normal;
	}
};

void readStlFile(const char *filename, triangleMesh *mesh){

	ifstream file(filename, ios::binary);
	char modelName[80];
	string identifier;
	size_t facetNo;

/*	float normalx, normaly, normalz,
	      vertex1x, vertex1y, vertex1z,
	      vertex2x, vertex2y ,vertex2z,
	      vertex3x, vertex3y, vertex3z;
*/	
	float Points[12];
	char discarder[2];

	file.read(modelName, 80);
	cout<<"\nModel Name is : "<<modelName;

	file.read((char *)facetNo, 4);
	cout<<"\nNo. Of Facets is : "<<facetNo;
	getchar();	
	while(!file.eof()){
	
		for(int i=0;i<12;i++){
			file.read((char *)&Points[i],sizeof(float));
		}

	file.read(discarder,2);
			
	}
			
	//	for(int i=0;i<12;i++)
	//		cout<<"\n"<<Points[i]<<" ";

}	




int main(int argc, char *argv[]){
	cout<<"\nProgram Starts\n";
	cout<<argv[1];
	
	triangleMesh mesh;

	readStlFile(argv[1],  &mesh);
	cout<<"\nProgram ends";

}
