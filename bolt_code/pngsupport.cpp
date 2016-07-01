#include <png++/png.hpp>

png::image <png::rgb_pixel> image(800,600);

// Bresenham's line drawing algorithm
void drawLine(int xa, int ya, int xb, int yb){
	
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

void generatePNG(slice s, int slice_counter, float min_x, float max_x, float min_y, float max_y){

	vector<glm::vec3> vertices;
	
	for ( auto it = s.boundary.begin(); it != s.boundary.end(); it++ ) {
	
		vertices.push_back(it->startpoint);
		vertices.push_back(it->endpoint);
	}

	if(s.isFilled){
		
		for(auto it=s.fill.begin(); it!=s.fill.end(); it++){
		
			vertices.push_back(it->startpoint);
			vertices.push_back(it->endpoint);
		}
	}

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
			
			_x = (it->x - min_x)/(max_x - min_x) * 599;
			_y = (it->y - min_y)/(max_y - min_y) * 599;
		
			x1 = (int)_x;
			y1 = (int)_y;

		}
		
		else{
			first = true;
			
			_x = (it->x - min_x)/(max_x - min_x) * 599;
			_y = (it->y - min_y)/(max_y - min_y) * 599;

			x2 = (int)_x;
			y2 = (int)_y;
		
			drawLine(x1,y1,x2,y2);
		}
	}	

	string pngFileName="png/slice_"+(to_string(slice_counter))+".png";
	image.write(pngFileName);
}
