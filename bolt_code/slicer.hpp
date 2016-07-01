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

	vector <linesegment> boundary;	///< Vector of line segments that constitute the slice. 
	vector <linesegment> fill;		

	bool isFilled = false;

	void fillSlice();
	
};

struct EdgeTableRecord{

	float y_min;
	float y_max;
	float x_y_min;
	float x_y_max;
	float inverse_slope;
	bool checked;
};

bool activeEdgeTableComparision(const EdgeTableRecord &a, const EdgeTableRecord &b);

bool globalEdgeTableComparision (const EdgeTableRecord &a, const EdgeTableRecord &b);


