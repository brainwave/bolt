//Include header #defines later
//
#include <iostream>
#include <vector>
#include <string>

using namespace std;

#include <stdio.h>
#include <math.h>
#include <string.h>

static	float     global_min_z_val 	= 0.0f
		, global_max_z_val	= 0.0f
		, max_z_var		= 0.0f ;
			//static global variables, shared across functions, classes and structures	

struct vec3 {
		//
		// Idea is to store a vector from file in struct vec3, then
		// 	Store all relevant vectors of each triangle in struct 'triangle', then
		//	Read all triangles sequentially into memory
		//
		// Using struct since data in vec changed using external functions
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

	vec3 operator-(const vec3& vector) {
		return vec3((x - vector.x),(y-vector.y),(z-vector.z));
	}
	

	vec3 operator+(const vec3& vector) {
		return vec3((x + vector.x),(y+vector.y),(z+vector.z));
	}

	vec3 operator*(const float f) {
		return vec3((x*f),(y*f),(z*f));
	}

	/*

	void operator=(const vec3& vector) {

		if((x!=vector.x)||(y!=vector.y)||(z!=vector.z)) {
			x=vector.x; y=vector.y; z=vector.z;
			}
				//To avoid circular assignment if equating vector to itself
	}
				Commenting since looks to be implemented already in C++11
	*/
	float dot(const vec3& vec) {
		return x*vec.x+y*vec.y+z*vec.z;
	}

	void normalize(){
		float magnitude = sqrt(x*x+y*y+z*z);
		x /= magnitude; y /= magnitude; z/= magnitude;
	}
};

struct triangle{
	//using struct since 'triangle' data pushed from external function

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
	
		output	<<"\nVertex 1 : "<<t.vertex[0].x<<"i+"<<t.vertex[0].y<<"j+"<<t.vertex[0].z<<"k"
			<<"\nVertex 2 : "<<t.vertex[1].x<<"i+"<<t.vertex[1].y<<"j+"<<t.vertex[1].z<<"k"
			<<"\nVertex 3 : "<<t.vertex[2].x<<"i+"<<t.vertex[2].y<<"j+"<<t.vertex[2].z<<"k"
			<<"\nNormal   : "<<t.normal.x<<"i+"<<t.normal.y<<"j+"<<t.normal.z<<"k";

		return output;
	}
			//overload to push data from a vector array (read from file) into struct triangle

	vec3 vertex[3], normal;
};

struct plane {

	plane (vec3 n=0, float d=0.0f) {

		normal.x=n.x; normal.y=n.y; normal.z=n.z;
		distance=d;
	}

	vec3 normal; float distance;

	float distanceFromPoint (vec3 point) {

		normal.normalize();
		return normal.dot(point)-distance;
	}
};
/*
struct slice {

	vector<vec3> slice;

	void planeTriangleIntersect ( triangle &t, plane &p) {
		vector<vec3> intersections;

		for (int i=0; i < 3; i++) {

			float dp1 = p.distanceFromPoint(t.vertex[i]);
			float dp2 = p.distanceFromPoint(t.vertex[(i+1)%3]);
			
			if( dp1*dp2 < 0) 
				intersections.push_back(t.vertex[i]+((t.vertex[(i+1)%3]-t.vertex[i])*(dp1/(dp1-dp2))) );
		}

		if(intersections.size()==2)
			slice.push_back(intersections[1]-intersections[0]);
	}
};
*/
struct triangleMesh{
	//Using class since data in triangleMesh accessible only to member functions
	
	vector<triangle> mesh;

	void displayMesh(triangle &t) { cout<<"\n"<<t; }

	void push_back(triangle t) { mesh.push_back(t); }

	void display_all_elements () {

		int counter=0;

		cout<<"\nData in Mesh : ";
		for( auto meshIterator=mesh.begin();meshIterator!=mesh.end();meshIterator++ )				
			cout<<"\nTriangle No. "<<++counter<<" "<<*meshIterator;

	}
 
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
			float 	  local_min_z_val 	= 0.0f
				, local_max_z_val	= 0.0f;


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
				
					if(i==1)
						local_min_z_val=local_max_z_val=vertex[i].z;
								//set local minimum and maximum to the first vertex z value, whatever it is	
					if(i!=0 && local_min_z_val > vertex[i].z)
						local_min_z_val = vertex[i].z;

					if(i!=0 && local_max_z_val < vertex[i].z)
						local_max_z_val = vertex[i].z;
				}
					
				if ( global_min_z_val > local_min_z_val )
					global_min_z_val = local_min_z_val ;

				if ( global_max_z_val < local_max_z_val )
					global_max_z_val = local_max_z_val;
				
				if( max_z_var < local_max_z_val - local_min_z_val )
					max_z_var = (local_max_z_val - local_min_z_val);

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
		float 	  local_min_z_val 	= 0.0f
			, local_max_z_val	= 0.0f;


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
					break;
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
		
								if ( i==1 )
								{
									local_min_z_val=local_max_z_val=vertex[1].z;
									cout<<"\nFirst value assigned";
								}
								if ( local_min_z_val > vertex[i].z)
								{
									local_min_z_val = vertex[i].z;
									cout<<"\nZ min reassigned "<<local_min_z_val;
								}
								if ( local_max_z_val < vertex[i].z ) {
									local_max_z_val = vertex[i].z;
									cout<<"\nZ max reassigned "<<local_max_z_val;
								}
								
						}
						if ( global_min_z_val > local_min_z_val )
							global_min_z_val = local_min_z_val ;

						if ( global_max_z_val < local_max_z_val )
							global_max_z_val = local_min_z_val ;
						
						if ( max_z_var < local_max_z_val - local_min_z_val )
							max_z_var = local_max_z_val - local_min_z_val;

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

int main(int argc, char *argv[]){

	cout<<"\nProgram Starts, file name is : ";
	cout<<argv[1];

	float sliceSize=atof(argv[2]);
	
	triangleMesh mesh;


	if(readStlFile(argv[1],  &mesh))
		cout<<"\nProgram Failed";
	else {
		cout<<"\nGlobal Max Z: "<<global_max_z_val<<" Global Min Z "<<global_min_z_val<<" Global Z Variation "<<max_z_var;
		cout<<"\nProgram Sucess";
	}
}
