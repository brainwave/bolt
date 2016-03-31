//Include header #defines later
//
#include <iostream>
#include <vector>
#include <string.h>
#include <string>
#include <stdio.h>
using namespace std;

struct vec3 {
		//
		// Idea is to store a vector from file in struct vec3, then
		// 	Store all relevant vectors of each triangle in struct 'triangle', then
		//	Read all triangles sequentially into memory
		//
	vec3(float x_=0, float y_=0, float z_=0):x(x_), y(y_), z(z_){}

	float x, y, z;

	friend vec3& operator>>(const vec3& read_vec, vec3& vec){
		vec.x=read_vec.x; vec.y=read_vec.y;vec.z=read_vec.z; 
		return vec;
	}
		//overload >> to copy a vector
		
	friend ostream& operator<<(ostream& output, const vec3 vec){
		output<<vec.x<<"i+"<<vec.y<<"j+"<<vec.z<<"k ";
		return output;
	}
		//overload << to print values conveniently 

};

struct triangle{
	
	friend triangle& operator>>(const vec3 vertex[4], triangle &t){ 

		t.normal.x 	= vertex[0].x;
		t.normal.y 	= vertex[0].y;
		t.normal.z	= vertex[0].z;
		
		for(int i=0;i<3;i++) {
			t.vertex[i].x 	= vertex[i+1].x;
			t.vertex[i].y	= vertex[i+1].y;
			t.vertex[i].z	= vertex[i+1].z;
		}
		//vertex[0] is normal, vertex 1, 2, 3 are the vertices. Read accordingly.
		return t;
	}
	
	friend ostream& operator<<(ostream &output, const triangle &t) {
	
		output	<<"Triangle\n"
			<<"Vertex 1 : "<<t.vertex[0].x<<"i+"<<t.vertex[0].y<<"j+"<<t.vertex[0].z<<"k\n"
			<<"Vertex 2 : "<<t.vertex[1].x<<"i+"<<t.vertex[1].y<<"j+"<<t.vertex[1].z<<"k\n"
			<<"Vertex 3 : "<<t.vertex[2].x<<"i+"<<t.vertex[2].y<<"j+"<<t.vertex[2].z<<"k\n"
			<<"Normal   : "<<t.normal.x<<"i+"<<t.normal.y<<"j+"<<t.normal.z<<"k\n";

		return output;
	}
			//overload to push data from a vector array (read from file) into struct triangle

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
		//Using C style file IO over C++ style streams, as streams cause data corruption.
		//May be fixable, need to check advantages of both approaches
		
	do{
		parser=fgetc(file);
	}while((parser!=EOF) && parser<=127 );
		//Check if file is ASCII. If true, every character read from file should be <=127
		//Need to check for robustness of this approach.
		
	fclose(file);

	if(parser==EOF)
		isASCII=true;

	if(!isASCII) {
		//block to read binary stl file
		
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
			cin.get();

			while(!feof(file)){
				for(int i=0;i<4;i++){

					fread((void *)&vertex[i].x, 4, 1 , file);
					fread((void *)&vertex[i].y, 4, 1 , file);
					fread((void *)&vertex[i].z, 4, 1 , file);
				}
	
				vertex>>t;

				mesh->push_back(t);

				fread((void *)discarder, 2, 1, file);
					//discard 2 bytes end of each loop
			}
		return 0;
	}

	else{
		//block for ASCII
		
		cout<<"\nUsing ASCII format";
		
		file = fopen(filename, "r");
		
		char header[6], modelName[256];

		fscanf(file,"%s",header);
			//Again, choosing C style formatted read of C++ style stream.
			//Check advantages of each approach, change if beneficial.
			
		if(!strcmp(header,"solid")){

			fgets(modelName,255,file);
			cout<<"\nModel Name is : "<<modelName;

			while (!feof(file)){

				char string0[11], string1[11];
				
				fscanf(file,"%s", string0);

				if(!strcmp(string0,"endsolid")) {

					cout<<"\nEnd of file reached";
					return 1;
				}

				fscanf(file,"%s", string1);

				if( !strcmp(string0, "facet") && !strcmp(string1,"normal")) {

					fscanf(file, "%f", &vertex[0].x);
					fscanf(file, "%f", &vertex[0].y);
					fscanf(file, "%f", &vertex[0].z);

					fscanf(file, "%s", string0);
					fscanf(file, "%s", string1);
						
					if( !strcmp(string0, "outer") && !strcmp(string1,"loop")){
				
						for(int i=1; i<=3; i++){

								fscanf(file, "%s", string0);
								
								if( !strcmp(string0, "vertex")){

									fscanf(file, "%f", &vertex[i].x);
									fscanf(file, "%f", &vertex[i].y);
									fscanf(file, "%f", &vertex[i].z);
								}
	
						}

						fscanf(file, "%s", string0);
						if( strcmp(string0, "endloop"))
						{
							cout<<"\nERROR - Expected 'endloop', found "<<string0<<" instead";
							return 1;
						}

						fscanf(file, "%s", string0);

						if( strcmp(string0, "endfacet")) {

							cout<<"ERROR - expected 'endfacet', found "<<string1<<" instead";
							return 1;
						}
					}
				}

				vertex>>t;
				mesh->push_back(t);

			}
		}
	
		else
			cout<<"\nHeader Mismatch, incorrect ASCII file";

		return 0;
	}	
		
}	

