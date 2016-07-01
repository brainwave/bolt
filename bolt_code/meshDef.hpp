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

/**
 \brief overloaded << operator to write a vec3 into an outputstream
	
 	@param output Output stream.
	@param vector The vec3 to be written.
 
*/
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

	vector<linesegment> slice;	///< Vector of line segments that constitute the slice. 
	bool isFilled = false;
public:
	void fillSlice();
	
};

/**
 \brief Represents the input mesh. 
	
	Specified by a list of triangles.
*/
class stlMesh {
	
	vector <triangle> mesh;	///< Vector of triangles that constitute the mesh.
	
	float min_x = numeric_limits<float>::max(); ///< Minimum x coordinate in the mesh
	float min_y = numeric_limits<float>::max(); ///< Minimum y coordinate in the mesh
	float min_z = numeric_limits<float>::max(); ///< Minimum z coordinate in the mesh
	float max_x = numeric_limits<float>::lowest(); ///< Maximum x coordinate in the mesh
	float max_y = numeric_limits<float>::lowest(); ///< Maximum y coordniate in the mesh
	float max_z = numeric_limits<float>::lowest(); ///< Maximum z coordinate in the mesh
	
public:
	float getMinX();
	float getMaxX();
	float getMinY();
	float getMaxY();
	float getMinZ();
	float getMaxZ();	
	
	/**
		Caculates xrange, yrange, zrange and minimum and maximum x, y, z coordinates and re-centers the model around
		(xrange/2,yrange/2,zrange/2) 

		@todo Remove parameters
	*/
	void recenter ();

	/**
		Reads the given stl file in ASCII or Binary format and pushes the trianges into <c>mesh</c>.
	
		@param filename Name of the stl file.
	*/
	int readStlFile ( const char *filename );

	/**
		\brief Slices the model to obtain 2D slices.
	
		For all the trianlges in the mesh, the intersection line segments with the relevant planes
		are computed and pushed into the appropriate slice.
	
		@param p Pointer to the beginning of the array of intersection planes.
		@param s Pointer to the beginning of the array of slices.
		@param sliceSize Specified slice size.
		@param arr_len Length of the plane and slice arrays.
	
	*/
	void sliceMesh(plane*p, slice*s,  float sliceSize, int arr_len);

};

struct EdgeTableRecord{

	float y_min;
	float y_max;
	float x_y_min;
	float x_y_max;
	float inverse_slope;
	bool checked;
};

bool activeEdgeTableComparision(const EdgeTableRecord &a, const EdgeTableRecord &b){
	
	if( a.x_y_min < b.x_y_min)		
		return true;
	else{
		if(a.x_y_min == b.x_y_min){	
			
			if(a.y_min < b.y_min){
			
				return true;
			}
			else{
		
				if(a.y_min == b.y_min){
				
					if(a.y_max < b.y_max){
	
						return true;
					}
					else{
						if(a.y_max ==  b.y_max){
							
							if(a.inverse_slope<b.inverse_slope){
					
								return true;
							}
							else{
				
								return false;
							}
						}
						else{
						
							return false;
						}
					}
				}
				else{
					
					return false;
				}
			}
		}
		else{
		
			return false;
		}
	}
}

bool globalEdgeTableComparision (const EdgeTableRecord &a, const EdgeTableRecord &b){

		if(a.y_min < b.y_min)
			return true;

		else if(a.y_min == b.y_min){
			
			if(a.x_y_min < b.x_y_min)
				return true;

			else if(a.x_y_min == b.x_y_min){
			
				if(a.y_max < b.y_max)
					return true;
				
				else
					return false;
			}
	
			else
				return false;
		}
		
		else
			return false;
}
		
#endif
