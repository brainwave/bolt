/** 	
* @file meshDef.hpp
* \brief Header file for the mesh.
*	
* Defines the structures that represent the mesh and the triangles in them.
*/


#ifndef MESHDEF_H
#define MESHDEF_H

#include <iostream>
#include <string.h>

#include "glm/glm.hpp"
#include "glm/gtx/string_cast.hpp"
#include "slicer.hpp"
#include <vector>
#include <list>

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
			t.vertex[i] = vertex[i+1];
		
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

	//setter to set vertices
	void setVertices(vec3 a, vec3 b, vec3 c) {
		vertex[0] = a;
		vertex[1] = b;
		vertex[2] = c;		
	}
	
};

/**
 \brief Represents the input mesh. 
	
	Specified by a list of triangles.
*/
class stlMesh {

private:
		
	vector <triangle> mesh;	///< Vector of triangles that constitute the mesh.
	
	float min_x = numeric_limits<float>::max(); ///< Minimum x coordinate in the mesh
	float min_y = numeric_limits<float>::max(); ///< Minimum y coordinate in the mesh
	float min_z = numeric_limits<float>::max(); ///< Minimum z coordinate in the mesh
	float max_x = numeric_limits<float>::lowest(); ///< Maximum x coordinate in the mesh
	float max_y = numeric_limits<float>::lowest(); ///< Maximum y coordniate in the mesh
	float max_z = numeric_limits<float>::lowest(); ///< Maximum z coordinate in the mesh
	
public:
	
	vector<vec3> supportPoints;  //holds x,y, and max z
	/** Calculates minimum and maximum X,Y,Z co-ordinates **/
	void getMinMax();

	/**
	 \brief Returns minimum x-coordinate in the mesh after recentering.

		\Warning <c>recenter()</c> needs to be called atleast once before this.
	 */
	float getMinX();

	/** 
	 \brief Returns maximum x-coordinate in the mesh after recentering. 
	
		\Warning <c>recenter()</c> needs to be called atleast once before this.
	*/
	float getMaxX();
	
	/**
	 \brief Returns minimum y-coordinate in the mesh after recentering. 
	
		\Warning <c>recenter()</c> needs to be called atleast once before this.
	*/
	float getMinY();	

	/** 
	 \brief Returns maximum y-coordinate in the mesh after recentering. 

		\Warning <c>recenter()</c> needs to be called atleast once before this.
	*/
	float getMaxY();	

	/** 
	 \brief Returns minimum z-coordinate in the mesh after recentering. 
	
		\Warning <c>recenter()</c> needs to be called atleast once before this.
	*/
	float getMinZ();	

	/** 
	 \brief Returns maximum z-coordniate in the mesh after recentering. 

		\Warning <c>recenter()</c> needs to be called atleast once before this.
	*/
	float getMaxZ();
	
	/**
		Caculates xrange, yrange, zrange and minimum and maximum x, y, z coordinates and re-centers the model around
		(xrange/2,yrange/2,zrange/2) 
			
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
	
	void projectTriangle(triangle *t);

	void boundBox(int,int);
};

#endif
