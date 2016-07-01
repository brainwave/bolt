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

int showSlice(slice *s, float &, float &, float &);

png::image< png::rgb_pixel > image(800,600);

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

int showSlice(slice *s, float min_x, float max_x, float min_y, float max_y) {

//	int boundaryVertexCount = 0;	

	s->fillSlice();

	vector<glm::vec3> vertices;
	
	for ( auto it = s->slice.begin(); it != s->slice.end(); it++ ) {
	
		vertices.push_back(it->startpoint);
		vertices.push_back(it->endpoint);
//		boundaryVertexCount+=2;
	}

//	if(boundaryVertexCount != 0)
//		vertices = lineFill(vertices);

	bool first = true;	

	
	for (size_t y = 0; y < image.get_height(); y++ )
		for (size_t x=0; x < image.get_width(); x++) {
		
		image[y][x] = png::rgb_pixel(0,0,0);
	}


	float _x,_y;
	int x1,x2,y1,y2;

	for( auto it = vertices.begin(); it!= vertices.end(); it++){

		if(first){
			
			first = false;
			
			_x = (it->x - min_x)/(max_x - min_x) * 600 + 1 ;
			_y = (it->y - min_y)/(max_y - min_y) * 600 + 1 ;
		
			x1 = (int)_x - 1;
			y1 = (int)_y - 1;

		}
		
		else{
			first = true;
			
			_x = (it->x - min_x)/(max_x - min_x) * 600 + 1;
			_y = (it->y - min_y)/(max_y - min_y) * 600 + 1;

			x2 = (int)_x - 1;
			y2 = (int)_y - 1;
		
		//	if( x1>=0 && x1<600 && x2>=0 && x2<600 && y1>=0 && y1<600 && y2>=0 && y2<600)
				lineDraw(x1,y1,x2,y2);
		//	else
		//		 cout<<"\n Out of bounds";
		}
	}	

	return 0;
}

int showWindow(slice *s, int max_slice_no, float max_x, float min_x, float max_y, float min_y) {
	
	int cur_slice_no = 0;	

	clock_t startTime = clock();

	while ( cur_slice_no < max_slice_no) {
	
		showSlice(s, min_x, max_x, min_y, max_y);	

		string pngFileName="png/slice_"+(to_string(cur_slice_no))+".png";
		image.write(pngFileName);

		cur_slice_no++;
		s++;
	}

	cout<<"\nTotal time taken by slicer is "<<(double)(clock() - startTime)/CLOCKS_PER_SEC;
	
	return 0;
}
