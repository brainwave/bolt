#include <iostream>
#include <cassert>
#include <vector>
#include <fstream>
#include <algorithm>
using namespace std;

#include <stdio.h>
#include <unistd.h>

#include "glm/glm.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include <png++/png.hpp>

#include "meshDef.cpp"

float  xscale, yscale, zscale;

int saveSlice(slice *s, float &, float &, float &);
int max_slice_no=0,cur_slice_no=0, vertexCount=0;
int width=0, height=0;

uint8_t *pixels = new uint8_t [ 800 * 600 * 3 ] ;

int endPointCount=0;
//functions

float minimum (GLfloat x, GLfloat y, GLfloat z) {
	float min = x; /* assume x is the largest */
	if (y < min) 
		min = y;
	if (z < min ) 
		min = z;

	return min ; /* max is the largest value */
} 

int scaleInit(int slicecounter, const GLfloat xshift, const GLfloat yshift, const GLfloat zshift, const GLfloat pixels_per_mm = 0.5) {
	//store globally relevant info
	max_slice_no=slicecounter;

	xscale = 1/(2*xshift); //(pixels_per_mm)/(2*xshift);
	yscale = 1/(2*yshift);//(pixels_per_mm)/(2*yshift);
	zscale = 1/(2*zshift);

	return 0;
}

int boundaryVertexCount=0;

bool xCoordinateComparision(const glm::vec3 &a, const glm::vec3 &b){

	return a.x<b.x;
}

vector <glm::vec3>  lineFill(vector <glm::vec3> vertices) {


	if(boundaryVertexCount == 0)
		return vertices;

	vector <glm::vec3> addedVertices, intersections;

	float yMin = vertices[0].y, yMax = vertices[0].y, xMin = vertices[0].x, xMax = vertices[0].x, step;

	bool first = true;
	bool odd = true;

	
	// find ymax and ymin
	for(auto it = vertices.begin(); it!=vertices.end(); it++){

		yMin = (it->y < yMin) ? it->y : yMin; yMax = (it->y > yMax) ? it->y : yMax;
		xMin = (it->x < xMin) ? it->x : xMin; xMax = (it->x > xMax) ? it->x : xMax;
	}

	step =(yMax - yMin)/600;

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
					
						if(endPoints.size() == 2){ // 2 point case
							
							// if vertex has not already been added
							if(! (find(intersections.begin(),intersections.end(),vertex1)!=intersections.end()) ){
						//		intersections.push_back(vertex1);	
								if( (endPoints[0].y - vertex1.y) * (endPoints[1].y - vertex1.y ) < 0 )
									intersections.push_back(vertex1);
							}
						} 			
							
						else if (endPoints.size()>2) {
							
							cout<<"\nJunction no "<<endPointCount<<" detected, has "<<endPoints.size()<<" Convergence";
							endPointCount++;
						}
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
						//	intersections.push_back(vertex2);	
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

                for(auto x = intersections.begin(); x!=intersections.end(); x++){

			addedVertices.push_back(*x);
                }
	}

	// push all added vertices into vertices
	for(auto it = addedVertices.begin(); it!=addedVertices.end(); it++){

		vertices.push_back(*it);
	}
	
	return vertices;
}

int saveSlice(slice *s,  GLfloat &x_scale, GLfloat &y_scale, GLfloat &z_scale) {

	vector<glm::vec3> vertices ;

	boundaryVertexCount = 0;
	vertexCount = 0;	
	glm::vec3 temp_vertex;
	
	for ( auto it = s->slice.begin(); it != s->slice.end(); it++ ) {
	
		vertices.push_back(it->startpoint);
		vertices.push_back(it->endpoint);
		
		boundaryVertexCount++;
	}

//	fclose(file);

	vertices = lineFill(vertices);

	GLfloat aspectratio = width/(float)height;
	
	yscale = minimum(xscale,yscale,zscale);
	xscale = yscale;zscale = yscale;yscale*= aspectratio;	

	x_scale=xscale;y_scale=yscale;z_scale=zscale;

	if(tm ==-1 ) {
			cout<<"\nError, couldn't bind transform ";
			return 1; 
	}


	png::image< png::rgb_pixel > image(800,600);

	int color=0;

	for (size_t y = image.get_height()-1; y >0; --y )
		for (size_t x=0; x < image.get_width(); ++x) {
			image[y][x] = png::rgb_pixel(*(pixels + color++),*(pixels + color++),*(pixels + color++));
		}
	string pngFileName="png/slice_"+(to_string(cur_slice_no))+".png";
	image.write(pngFileName);

	vertexCount = 0;

	return 0;
}

int savePng(slice *s, float x_scale, float y_scale, float z_scale) {
	 //vertex shader
	while(cur_slice_no < max_slice_no) {

	saveSlice(s,x_scale, y_scale, z_scale);	
	cur_slice_no++; s++;
	
	}
	return 0;
}
