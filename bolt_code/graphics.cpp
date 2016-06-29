#include <iostream>
#include <cassert>
#include <vector>
#include <fstream>
#include <algorithm>
#include <limits>
using namespace std;

#include <stdio.h>
#include <unistd.h>

#include "glm/glm.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include <png++/png.hpp>

#include "meshDef.cpp"
#include <time.h>
// Shader sources
//
//Global Variables
float  xscale, yscale, zscale;
uint8_t *pixels = new uint8_t[800 * 600 * 3];

int showSlice(slice *s, float &, float &, float &);
int max_slice_no=0,cur_slice_no=0, vertexCount=0;
int width=0, height=0;

int endPointCount=0;
png::image< png::rgb_pixel > image(800,600);

//functions
float minimum (float x, float y, float z) {
	float min = x; /* assume x is the largest */
	if (y < min) 
		min = y;
	if (z < min ) 
		min = z;

	return min ; /* max is the largest value */
} 

//R(ange)scale_x, y and z
int glInit(int slicecounter, const float Rscale_x, const float Rscale_y, const float Rscale_z, const float pixels_per_mm = 0.5) {
	//store globally relevant info
	max_slice_no=slicecounter;

	xscale = (pixels_per_mm)/(2*Rscale_x);
	yscale = (pixels_per_mm)/(2*Rscale_y);
	zscale = 1/(2*Rscale_z);
	return 1;
}

int boundaryVertexCount=0;

bool xCoordinateComparision(const glm::vec3 &a, const glm::vec3 &b){

	return a.x<b.x;
}


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



vector <glm::vec3>  lineFill(vector <glm::vec3> vertices, int c) {


	if(boundaryVertexCount == 0)
		return vertices;

	vector <glm::vec3> addedVertices, intersections;

	float yMin = vertices[0].y, yMax = vertices[0].y, xMin = vertices[0].x, xMax = vertices[0].x, step;
	float zCoord = vertices[0].z;	

	bool first = true;
	bool odd = true;

	
	// find ymax and ymin
	for(auto it = vertices.begin(); it!=vertices.end(); it++){

		yMin = (it->y < yMin) ? it->y : yMin; yMax = (it->y > yMax) ? it->y : yMax;
		xMin = (it->x < xMin) ? it->x : xMin; xMax = (it->x > xMax) ? it->x : xMax;
	}

	step =(yMax - yMin)/600;
	
	// EDGE TABLE ALGORITHM

	glm::vec3 vertex1, vertex2, leftVertex, rightVertex;
	
	vector <EdgeTableRecord> global,active;

	// creation of the global edge table
	for(auto it = vertices.begin(); it!=vertices.end(); it++){
	
		if(first){
		
			vertex1 = *it;
			
			first = false;
		}

		else{
			
			vertex2 = *it;
	
			first = true;			

			float y_min, y_max, x_y_min,x_y_max, inverse_slope;
			
			if(vertex1.y<vertex2.y){
	
				y_min = vertex1.y;
				y_max = vertex2.y;
				x_y_min = vertex1.x;
				x_y_max = vertex2.x;
			}
			else if(vertex1.y == vertex2.y){
			
				y_min = y_max = vertex1.y;
				if(vertex1.x<vertex2.x){
					
					x_y_min = vertex1.x;
					x_y_max = vertex2.x;
				}
				else{
			
					x_y_min = vertex2.x;
					x_y_max = vertex1.x;
				}
			}
			else{
	
				y_min = vertex2.y;
				y_max = vertex1.y;
				x_y_min = vertex2.x;
				x_y_max = vertex1.x;
			}


			if(y_max == y_min)
				inverse_slope = numeric_limits<float>::max();
			else
				inverse_slope = (vertex1.x - vertex2.x)/(vertex1.y - vertex2.y);
			
			EdgeTableRecord edge;

			edge.y_min = y_min;
			edge.y_max = y_max;
			edge.x_y_min = x_y_min;
			edge.x_y_max = x_y_max;
			edge.inverse_slope = inverse_slope;
			edge.checked = false;
				
			global.push_back(edge);
		}
	}

	// sort based on y_min, then x_y_min, then y_max	
	sort(global.begin(),global.end(),globalEdgeTableComparision);




	bool flag;

	for(float y = yMin; y<=yMax; y+=step){

		

		// remove active edges with y_max < y
		for(auto it = active.begin(); it!=active.end(); ){

			if(it->y_max<y){
				active.erase(it);
			}
			else{
				it++;
			}
		}
	
		
	
		// add edges with y_min = y to the active edge list
		for(auto it = global.begin(); it!=global.end() && it->y_min<=y; ) {

			active.push_back(*it);
			global.erase(it);
		}
	
		// sort active edge table records in ascending order of x_y_min
		sort(active.begin(),active.end(),activeEdgeTableComparision);

		odd = true;

		intersections.clear();


		for(auto it = active.begin(); it!=active.end(); it++){
	
			vertex1.x = it->x_y_min;
			vertex1.y = y;
			vertex1.z = zCoord;
				
			if(y>it->y_min && y<it->y_max) {
	
				intersections.push_back(vertex1);
			}
			else if(y==it->y_min && y!=it->y_max){	
			
				for(auto iter = active.begin(); iter!=active.end(); iter++){
	
					if(iter->y_max==y){
						
						if(!(find(intersections.begin(),intersections.end(),vertex1)!=intersections.end())){
	
							intersections.push_back(vertex1);	
						}
					}
				}
			}
			else if(y==it->y_max && y!=it->y_min){
	
				for(auto iter = active.begin(); iter!=active.end(); iter++){		
			
					if(iter->y_min==y){
					
						if(!(find(intersections.begin(),intersections.end(),vertex1)!=intersections.end()))
	
							intersections.push_back(vertex1);
					}
				}
			}

		}
		
		for(auto it = active.begin(); it!=active.end(); it++){	
			
			if(it->y_max != it->y_min)	
				it->x_y_min = it->x_y_min + step*it->inverse_slope;
		}

		if(intersections.size() % 2 == 1)
			intersections.erase(intersections.end() - 1);

		for(auto x = intersections.begin(); x!=intersections.end();x++){
		
			vertices.push_back(*x);
		}		

	}
	
	return vertices;
}



vector <glm::vec3>  lineFill(vector <glm::vec3> vertices) { 

	if(boundaryVertexCount == 0)
		return vertices;

	vector <glm::vec3> addedVertices, intersections;

	float yMin = vertices[0].y, yMax = vertices[0].y, xMin = vertices[0].x, xMax = vertices[0].x, step, xstep;

	bool first = true;
	bool odd = true;

	
	// find ymax and ymin
	for(auto it = vertices.begin(); it!=vertices.end(); it++){

		yMin = (it->y < yMin) ? it->y : yMin; yMax = (it->y > yMax) ? it->y : yMax;
		xMin = (it->x < xMin) ? it->x : xMin; xMax = (it->x > xMax) ? it->x : xMax;
	}

	step =(yMax - yMin)/600;
	xstep = (xMax - xMin)/800;

	for (float i=yMin; i<=yMax; i+=step){

		glm::vec3 vertex1, vertex2;
		glm::vec3 intersectionPoint;
			
		float dp1, dp2;

		odd = true;
		first = true;

		intersections.clear();
		
		for(auto it = vertices.begin();it != vertices.end(); it++){
	
			// store start vertex of an edge
			if(first){

				vertex1 = *it;
				dp1 = vertex1.y - i;
				first = false;
			}
						
			// store the end vertex of an edge
			else{

				first = true;
				vertex2 = *it;
				dp2 = vertex2.y - i;
			
				// start and end are on opposite sides of the scan line
				if( dp1*dp2 < 0){
					
					intersectionPoint = vertex1 + (vertex2 - vertex1)*(dp1/(dp1-dp2));
					intersections.push_back(intersectionPoint);
				}

				// scan line passes through start point
				else if (dp1 == 0 && dp2 != 0){
					
					vector<glm::vec3> endPoints;

					int counter = 0;

					for (auto iter = vertices.begin(); iter!=vertices.end(); iter++, counter++){
						
						if(iter->x == vertex1.x && iter->y == vertex1.y) {
						
							if(counter%2==0)	
								endPoints.push_back(*(iter+1));
							else
								endPoints.push_back(*(iter-1));
						
					 	}
					}
					
					if(endPoints.size() == 2){ // 2 point case
					
						// if vertex has not already been added
						if(! (find(intersections.begin(),intersections.end(),vertex1)!=intersections.end()) ){
					
							if( (endPoints[0].y - vertex1.y) * (endPoints[1].y - vertex1.y ) < 0 )
								intersections.push_back(vertex1);
						}
					} 			
						
					else if (endPoints.size()>2) {
							
						cout<<"\nJunction no "<<endPointCount<<" detected, has "<<endPoints.size()<<" Convergence";
					
						endPointCount++;
					}
					
				}
				
				// scan line passes through end point
				else if (dp2 == 0 && dp1 != 0){

					vector<glm::vec3> endPoints;

					int counter = 0;

					for (auto iter = vertices.begin(); iter!=vertices.end(); iter++, counter++){
						
						if(iter->x == vertex2.x && iter->y == vertex2.y){
						
							if(counter%2==0)								
								endPoints.push_back(*(iter+1));
							else
								endPoints.push_back(*(iter-1));
						}
					}
						
					if(endPoints.size() == 2){ // 2 point case
						
						// if vertex has not already been added	
						if( !(find(intersections.begin(),intersections.end(),vertex2)!=intersections.end()) ){
						
							if( (endPoints[0].y - vertex2.y) * (endPoints[1].y - vertex2.y ) < 0 )
								intersections.push_back(vertex2);
						}
					} 
	
					else if (endPoints.size()>2) {
							
						cout<<"\nJunction no "<<endPointCount<<" detected, has "<<endPoints.size()<<" Convergence";
					
						endPointCount++;
					}
				}
			}

		}

		// sort intersection points  and push them into addedVertices
                sort(intersections.begin(), intersections.end(),xCoordinateComparision);

		// if the number of intersections is odd, the remove the last intersection
		if(intersections.size() % 2 == 1)
			intersections.erase(intersections.end() - 1);		

		for(auto x = intersections.begin(); x!= intersections.end(); x++){
			addedVertices.push_back(*x);
                }
	}

	// push all added vertices into vertices
	for(auto it = addedVertices.begin(); it!=addedVertices.end(); it++){

		vertices.push_back(*it);
	}
	
	return vertices;

}

// Bresenham's Line Drawing Algorithm
void lineDraw(int xa, int ya, int xb, int yb){
	
	int dx,dy,p,x,y,xStart,yStart,xEnd,yEnd;

	dx = abs(xb - xa);
	dy = abs(yb - ya);
	
	if(dx>=dy){

		p = 2*dy -dx; //p0

		// draw from left to right
		if(xa>xb){

			x = xb;
			y = yb;
			yStart = yb;
			yEnd = ya;
			xEnd = xa;
		}
		else{

			x = xa;
			y = ya;
			yStart = ya;
			yEnd = yb;
			xEnd = xb;
		}

		image[y][x] = png::rgb_pixel(0,0,255);

		while(x<xEnd){

			x = x + 1;
			if (p<0)
				p = p+2*dy;
			else{

				y = y+1*(yEnd - yStart)/dy;
				p = p + 2*(dy-dx);
			}

			image[y][x] = png::rgb_pixel(0,0,255);
		}
	}
	else{

		p = 2*dx -dy; //p0

		// draw from bottom to top
		if(ya>yb){

			y = yb;
			x = xb;
			xStart = xb;
			xEnd = xa;
			yEnd = ya;
		}
		else{

			y = ya;
			x = xa;
			xStart = xa;
			xEnd = xb;
			yEnd = yb;
		}

		image[y][x] = png::rgb_pixel(0,0,255);

		while(y<yEnd){

			y = y + 1;
			if (p<0)
				p = p+2*dx;
			else{

				x = x+1*(xEnd-xStart)/dx;
				p = p + 2*(dx-dy);
			}

			image[y][x] = png::rgb_pixel(0,0,255);
		}
	}

}

int showSlice(slice *s,  float &x_scale, float &y_scale, float &z_scale, float max_x, float min_x, float max_y, float min_y) {

	vector<glm::vec3> vertices, addedVertices ;

	boundaryVertexCount = 0;
	vertexCount = 0;	
	glm::vec3 temp_vertex;
	
	for ( auto it = s->slice.begin(); it != s->slice.end(); it++ ) {
	
		vertices.push_back(it->startpoint);
		vertices.push_back(it->endpoint);
		
		boundaryVertexCount+=2;
	}

//	fclose(file);

	vertices = lineFill(vertices,1);

	float aspectratio = width/(float)height;
	
	yscale = minimum(xscale,yscale,zscale);
	xscale = yscale;
	zscale = yscale;
	yscale*= aspectratio;	

	x_scale=xscale;
	y_scale=yscale;
	z_scale=zscale;

	bool first = true;	


	vertexCount = 0;
	for ( auto it = vertices.begin(); it != vertices.end(); it++){ 
				
//			cout<<"\n "<<*it;
			vertexCount++;
	}
	
	for (size_t y = 0; y < image.get_height(); y++ )
		for (size_t x=0; x < image.get_width(); x++) {

			
			image[y][x] = png::rgb_pixel(0,0,0);
		}


	float _x,_y;
	int x1,x2,y1,y2;


	for( auto it = vertices.begin(); it!= vertices.end(); it++){
	
		if(first){
			
			first = false;
			
			_x = (it->x - min_x)/(max_x - min_x) * 599 ;
			_y = (it->y - min_y)/(max_y - min_y) * 599 ;
		
			x1 = (int)_x;
			y1 = (int)_y;
	
		}
		else{
			first = true;
			
			_x = (it->x - min_x)/(max_x - min_x) * 599;
			_y = (it->y - min_y)/(max_y - min_y) * 599;
	
			x2 = (int)_x;
			y2 = (int)_y;
		
		//	if( x1>=0 && x1<600 && x2>=0 && x2<600 && y1>=0 && y1<600 && y2>=0 && y2<600)
				lineDraw(x1,y1,x2,y2);
		//	else
		//		 cout<<"\n Out of bounds";
		}
	}


	return 0;
}

int showWindow(slice *s, int window,float x_scale, float y_scale, float z_scale, float max_x, float min_x, float max_y, float min_y) {
	 //vertex shader


	clock_t startTime = clock();

	while ( cur_slice_no < max_slice_no) {
	showSlice(s,x_scale, y_scale, z_scale, max_x, min_x, max_y, min_y);	

	int color=0;

	string pngFileName="png/slice_"+(to_string(cur_slice_no))+".png";
	image.write(pngFileName);

	cur_slice_no++; s++;
	}

	cout<<"\nTotal time taken by slicer is "<<(double)(clock() - startTime)/CLOCKS_PER_SEC;
	
	return 0;
}
