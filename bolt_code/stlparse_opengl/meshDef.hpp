#include <iostream>
#include "libs/glm/glm.hpp"
#include <vector>

struct vec3 {

	glm::vec3 pv; //position vector

	vec3(float x, float y, float z) {
		
		pv.x=x;
		pv.y=y;
		pv.z=z;

	}

	friend vec3& operator>>(const vec3& read_vec, vec3& vec) {
		
		vec.pv.x = read_vec.pv.x;
		vec.pv.y = read_vec.pv.y;
		vec.pv.z = read_vec.pv.z;

		return vec;
	}

	friend std::ostream& operator<<(std::ostream& output, const vec3 vec){
		output<<round((vec.pv.x)*1000.0)/1000.0<<"i+"<<round((vec.pv.y)*1000.0)/1000.0<<"j+"<<round((vec.pv.z)*1000.0)/1000.0<<"k ";
		return output;
	}
	
	vec3 operator-(const vec3& vector);

	vec3 operator+(const vec3& vector);

	vec3 operator*(const float f);

	float dot(const vec3& vec);
	
	void normalize();

};

struct triangle {
	
	vec3 vertex[3], normal;
	
	friend triangle& operator>>(const vec3 vertex[4], triangle &t);

	friend std::ostream& operator<<(std::ostream &output, const triangle &t);

};

struct plane {

	plane (vec3 n, float d=0.0f);
	
	void create_plane (vec3 n, float d=0.0f);

	float distanceFromPoint (vec3 point);

	vec3 normal; float distance;
};

struct linesegment {
	
	linesegment (vec3 point1, vec3 point2);
	
	vec3 startpoint, endpoint;
};

struct slice {

	std::vector<linesegment> slice;
	
	void display_slice ();

	void write_slice ();

};

class triangleMesh {
	
	std::vector <triangle> mesh;

public:
	void slice_mesh (plane *p, slice *s) ;

	void displayMEsh (triangle &t);

	void push_back (triangle t);

	void display_all_elements ();

	void find_min_max_var_z (float &min_z, float &max_z);
	
	int readStlFile (const char *filename);
	//ideally functionality to read stl file should exist here
};

