/**
* @file slicer.hpp
* \brief Header file for the slicer.
*
* Defines structures that are used by the slicer.
*/

#include <vector>
#include <iostream>
#include <algorithm>
#include "glm/glm.hpp"

using namespace std;
using namespace glm;

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
	float distance; ///< Signed distance of the plane from origin.

	/**
		Instantiates members <c>normal</c> and <c>distance</c> with the given arguments.
	
		@param n Normal vector.
		@param d Distance from origin.
	*/
	void create_plane (vec3 n, float d = 0.0f){
			
			normal = n;
			distance = d;
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
*/
struct slice {

	vector <linesegment> boundary;	///< Vector of line segments that constitute the boundary of the slice. 
	vector <linesegment> fill;	///< Vector of line segments that represent the fill lines.		

	bool isFilled = false;		///< True if <c>fillSlice()</c> has been called once on this slice.

	/**
	 \brief Fills the slice.

		Uses the <i>Scan Line Algorithm</i> along with <i>Global</i> and <i>Active</i> Edge Table data structures.
		Each fill line is broken down into line segments and are pushed into the <c>fill</c> vector.
	*/
	void fillSlice();
	
};

/**
 \brief Record structure of the edge tables

	Represents the structure of the records in the <c>global</c> and
	<c>active</c> edge tables.
*/
struct EdgeTableRecord{

	float y_min;	///< Minimum y-coordinate of the edge.
	float y_max;	///< Maximum y-coordinate of the edge.

	float x_y_min;	///< x coordinate of the vertex with minimum y-coordinate.
	float x_y_max;	///< x coordinate of the vertex with maximum y-coordinate.

	float inverse_slope;	///< Inverse of the slope of the edge.
		
};

/**
 \brief Comparision function to perform STL sort on the <c>active</c> edge table.

	Compares two records based on x_y_min, then y_min, then y_max and finally inverse_slope.
*/
bool activeEdgeTableComparision(const EdgeTableRecord &a, const EdgeTableRecord &b);

/**
 \brief Comparision function to perform STL sort on the <c>global</c> edge table.

	Compares two records based on their y_min, then x_y_min and finally y_max.
*/
bool globalEdgeTableComparision (const EdgeTableRecord &a, const EdgeTableRecord &b);


