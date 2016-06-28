/** 	
* @file meshDef.hpp
* \brief Header file for the mesh.
*	
* Defines the structures that represent components of the mesh, the
* inersection planes and the slices.
*/


#ifndef MESHDEF_H
#define MESHDEF_H

#include <iostream>
#include <string.h>

#include "glm/glm.hpp"
#include "glm/gtx/string_cast.hpp"
#include <vector>

using namespace std;
using namespace glm;

ostream &operator<<(ostream & output, const vec3 &vector ) {
	output<<vector.x<<" "<<vector.y<<" "<<vector.z;
	return output;
}

/**
 \brief Mesh triangle
	
	Represents one triangle in a mesh. 
	Specified by 3 vertices and a normal vector.
*/
struct triangle {
	
	vec3 vertex[3]; ///< The three vertices of the triangle
	vec3 normal;	///< Outward normal vector of the triangle

	/**
		Overloaded >> operator to read a triangle into a vec3 array of size 4.

		@param vertex Index 0 is the normal. Index 1,2,3 represent the vertices.
		@param &t Reference to the triangle variable.
	*/
	friend triangle& operator>>(const vec3 vertex[4], triangle &t){ 

		t.normal = vertex[0];	
		for(int i=0;i<3;i++) 
			t.vertex[i] 	= vertex[i+1];
		
		return t;
	}

	/**
	 	Overloaded << operator to write a triangle into an output stream.

	 	@param output Reference to the output stream.
	 	@param t Reference to the triangle variable.
	*/
	friend ostream& operator<<(ostream &output, const triangle &t) {
		
		output	<<"\nVertex 1 : "<<t.vertex[0].x<<"i+"<<t.vertex[0].y<<"j+"<<t.vertex[0].z<<"k"
			<<"\nVertex 2 : "<<t.vertex[1].x<<"i+"<<t.vertex[1].y<<"j+"<<t.vertex[1].z<<"k"
			<<"\nVertex 3 : "<<t.vertex[2].x<<"i+"<<t.vertex[2].y<<"j+"<<t.vertex[2].z<<"k"
			<<"\nNormal   : "<<t.normal.x<<"i+"<<t.normal.y<<"j+"<<t.normal.z<<"k";
		
		return output;
	}
	
};

/**
 \brief Intersection plane
	
	Specified by a normal vector and distance from some reference point.
*/

struct plane {
	
	/**
		Returns the <i>signed</i> distance of the plane from a given point after normalizing the normal vector.
	
		@param point The point from which <i>signed</i> distance is to be calculated.
	*/
	float distanceFromPoint (vec3 point);

	vec3 normal;	///< Normal vector of the plane.
	float distance; ///< Signed distance of the plane from a reference point.

	/**
		Instantiates members <c>normal</c> and <c>distance</c> with the given arguments.
	
		@param n Normal vector.
		@param d Distance from a reference point.
	*/
	void create_plane (vec3 n, float d=0.0f ){
			normal=n;
			distance=d;
	}

};

/**
 \brief Line segment in a slice

	Specified by start and end points.
*/
struct linesegment {
	
	/**
		Constructor to initialize <c>startpoint</c> and <c>endpoint</c>.
	
		@param point1 Start point.
		@param point2 End point.
	*/
	linesegment (vec3 point1, vec3 point2) : startpoint (point1), endpoint (point2) {}
	
	vec3 startpoint; ///< Vector representing the start point of the line segment.
	vec3 endpoint;   ///< Vector representing the end point of the line segment. 
};

/**
 \brief Slice

	Represents a 2D cross section of the model.
	Specified by a set of linesegments.
*/

struct slice {

	vector<linesegment> slice;	///< List of line segments that represent the slice. 
	
	/**
	*/
	void display_slice ( void );
	
};

class stlMesh {
	
	vector <triangle> mesh;

	float min_z = 999999.0f, min_x = 999999.0f, min_y = 999999.0f;
	float max_z = -999999.f,max_x = -999999.f, max_y = -9999999.0f;

	
public:
	void slice_mesh (plane *p, slice *s);

	void displayMesh ( triangle &t );

	void display_all_elements ();

	void recenter (float &, float&, float &, float &, float&, float&, float&, float&, float&);

	int readStlFile ( const char *filename );

	void sliceByTriangle(plane*p, slice*s,  float sliceSize, int arr_len);

};

		
#endif
