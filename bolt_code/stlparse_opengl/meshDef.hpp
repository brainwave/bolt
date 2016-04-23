#include <iostream>
#include <string.h>

#include "libs/glm/glm.hpp"
#include "libs/glm/gtx/string_cast.hpp"
#include <vector>

using namespace std;
using namespace glm;

struct triangle {
	
	vec3 vertex[3], normal;

	friend triangle& operator>>(const vec3 vertex[4], triangle &t){ 

		t.normal = vertex[0];	
		for(int i=0;i<3;i++) 
			t.vertex[i] 	= vertex[i+1];
		
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
	
};

struct plane {
	
	float distanceFromPoint (vec3 point);

	vec3 normal; float distance;

	void create_plane (vec3 n, float d=0.0f ){
			normal=n;
			distance=d;
	}

};

struct linesegment {
	
	linesegment (vec3 point1, vec3 point2) : startpoint (point1), endpoint (point2) {}
	
	vec3 startpoint, endpoint;
};

struct slice {

	vector<linesegment> slice;
	
	void display_slice ( void );

	void write_slice ( void);
};

class stlMesh {
	
	vector <triangle> mesh;

	float min_z = 0.0f, max_z = 0.0f;

	
public:
	void slice_mesh ();

	void displayMesh ( triangle &t );

	void display_all_elements ();

	void set_min_max_var_z ( void ) ;
	
	int readStlFile ( const char *filename );
};

