
#include <iostream>
#include <vector>
#include <string.h>
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

	friend vec3& operator>>(const vec3& read_vec, vec3& vec){
		vec.x=read_vec.x; vec.y=read_vec.y;vec.z=read_vec.z; 
		return vec;
	}

	friend ostream& operator<<(ostream& output, const vec3 vec){
		output<<"\n Outputting Vector "<<vec.x<<"i+"<<vec.y<<"j+"<<vec.z<<"k ";
		return output;
	}

};

struct triangle{
	
	friend triangle& operator>>(const vec3 vertex[4], triangle &t){ 
		
		t.vertex[0] 	= vertex[1]; 
		t.vertex[1] 	= vertex[2]; 
		t.vertex[2] 	= vertex[3]; 
		t.normal 	= vertex[0];
			//vertex[0] is normal, vertex 1, 2, 3 are the vertices. Read accordingly.
		return t;
	}
	
	friend ostream& operator<<(ostream &output, const triangle &t){	

		output	<<"Triangle\n"
			<<"Vertex 1 : "<<t.vertex[0].x<<"i+"<<t.vertex[0].y<<"j+"<<t.vertex[0].z<<"k\n"
			<<"Vertex 2 : "<<t.vertex[1].x<<"i+"<<t.vertex[1].y<<"j+"<<t.vertex[1].z<<"k\n"
			<<"Vertex 3 : "<<t.vertex[2].x<<"i+"<<t.vertex[2].y<<"j+"<<t.vertex[2].z<<"k\n"
			<<"Normal   : "<<t.normal.x<<"i+"<<t.normal.y<<"j+"<<t.normal.z<<"k\n";

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

bool isASCII=false; //false -binary, true -ASCII

	triangle t;
	vec3 vertex[4];

	//detect if the file is ASCII or not
	int parser;
	
	FILE *file = fopen(filename, "r");
	
	do{
		parser=fgetc(file);
	}while((parser!=EOF) && parser<=127 );

	fclose(file);

	if(parser==EOF)
		isASCII=true;

	if(!isASCII) {
			char modelName[80], discarder[2];
			size_t facetNo;
	
			cout<<"\nFile is not ASCII, using binary format";

			file = fopen(filename, "rb");
			
			if(!file) {
				cout<<"\nfile opening failed,  with error ";
				perror("Error");
				return 1;
			}
			
			fread(modelName, 80, 1, file);
			fread((void *)&facetNo, 4, 1, file);
			
			cout<<"\nNo. Of Facets is : "<<facetNo;

			while(!feof(file)){
				for(int i=0;i<4;i++){

					fread((void *)&vertex[0], 4, 3 , file);
					fread((void *)&vertex[1], 4, 3 , file);
					fread((void *)&vertex[2], 4, 3 , file);
					fread((void *)&vertex[0], 4, 3 , file);
					
					vertex>>t;
					mesh->push_back(t);
					mesh->displayMesh(t);
				
				}
				fread((void *)discarder, 2, 1, file);
			}
		return 0;
	}

	else{
		cout<<"\nUsing ASCII format";
		
		file = fopen(filename, "r");
		
		char header[6], modelName[256];

		fscanf(file,"%s",header);

		if(!strcmp(header,"solid")){

			fgets(modelName,255,file);
			cout<<"\nModel Name is : "<<modelName;

			while (!feof(file)){

				char string0[11], string1[11];
				
				fscanf(file,"%s", string0);
				fscanf(file,"%s", string1);

				if( !strcmp(string0, "facet") && !strcmp(string1,"normal")) {

					fread((void *)&vertex[0], 4, 3 , file);

					fscanf(file, "%s", string0);
					fscanf(file, "%s", string1);

					if( !strcmp(string0, "outer") && !strcmp(string1,"loop")){
				
						for(int i=1; i<=3; i++){

								fscanf(file, "%s", string0);
								
								if( !strcmp(string0, "vertex"))
									fread((void *)&vertex[i], 4, 3, file);
						}
						
						fscanf(file, "%s", string0);
						fscanf(file, "%s", string1);

						if( strcmp(string0, "endloop"))
							return 1;

						if( strcmp(string1, "endfacet"))
							return 1;

					}
				}

				vertex>>t;
				mesh->push_back(t);
				mesh->displayMesh(t);

			}
		}
		else
			cout<<"\nHeader Mismatch, incorrect ASCII file";

		return 0;
	}	
		
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
