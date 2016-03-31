
#include <iostream>
#include <vector>
#include <string>
#include <stdio.h>
using namespace std;

//#include <vector>
	//commented out vector library since we want to do vector IO with GLM library
	//Later use a standard path, or include relevant header files in project specific include directory


//code to overload << and >> operators to enable output and input of data into glm::vec3 type
//

struct vec3 {
	vec3(float x_=0, float y_=0, float z_=0):x(x_), y(y_), z(z_){}
	float x, y, z;
	friend vec3& operator>>(const vec3& read_vec, vec3& vec){vec.x=read_vec.x; vec.y=read_vec.y;vec.z=read_vec.z; return vec;}
	friend ostream& operator<<(ostream& output, const vec3 vec)
	{	output<<"\n Outputting Vector "<<vec.x<<"i+"<<vec.y<<"j+"<<vec.z<<"k "; return output;}
};

struct triangle{
	
	friend triangle& operator>>(const vec3 vertex[4], triangle &t){ t.vertex[0]=vertex[0]; t.vertex[1]=vertex[1]
								; t.vertex[2]=vertex[2]; t.normal=vertex[4];
								return t;
	}
	
	friend ostream& operator<<(ostream &output, const triangle &t){	
		output<<"Vertex 1 components are : "<<t.vertex[1].x<<" "<<t.vertex[1].y<<" "<<t.vertex[1].z<<" ";
		return output;
	}

	vec3 vertex[3], normal;
};

struct triangleMesh{
	vector<triangle> mesh;
	void displayMesh(triangle &t) { cout<<"\n"<<t; }
	void push_back(triangle t) { mesh.push_back(t); }
	};

int readStlFile(const char *filename, triangleMesh *mesh){

	char modelName[80], discarder[2];
	size_t facetNo;
	triangle t;
	vec3 vertex[4];

	FILE *file = fopen(filename, "rb");
	if(!file) {
			cout<<"\nfile opening failed";
			perror("Error");
			return 1;
		}
		fread(modelName, 80, 1, file);
	fread((void *)&facetNo, 4, 1, file);
	cout<<"\nNo. Of Facets is : "<<facetNo;

	while(!feof(file)){
		for(int i=0;i<4;i++)
		{
			fread((void *)&vertex[0], sizeof(float), 3 , file);
			fread((void *)&vertex[1], sizeof(float), 3 , file);
			fread((void *)&vertex[2], sizeof(float), 3 , file);
			fread((void *)&vertex[0], sizeof(float), 3 , file);
			
			vertex>>t;
			mesh->push_back(t);
		
		}
		fread((void *)discarder, 2, 1, file);
	}
	return 0;
}	

int main(int argc, char *argv[]){
	cout<<"\nProgram Starts, file name is : ";
	cout<<argv[1];
	
	triangleMesh mesh;

	if(readStlFile(argv[1],  &mesh))
		cout<<"\nProgram Failed";
	else
		cout<<"\nProgram Sucess";
	cin.get();

}
