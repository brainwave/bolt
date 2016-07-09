/**
* @file pngsupport.cpp
* \brief PNG Support File.
*
*	Defines methods for generating a PNG from the given file.
*/

//#include <boost/filesystem.hpp>

#define SMALLER_DIM ( image.get_height() < image.get_width() ? image.get_height():image.get_width())

#ifdef __linux__

#include <png++/png.hpp>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>

int check_and_make_directory ( const char* pngDir ) {

	DIR* dir = opendir ( pngDir );
	
	if (dir) {
		cout<<"\nImage destination exists, proceeding";
	}
	else
	{
		cout<<"\nDirectory absent or inaccessible, creating";
		mkdir(pngDir, 0);
	}
}
#endif


png::image <png::rgb_pixel> image(800, 600);
string folder = "png";

void initPNG (int xres, int yres, string pngDir ) {

	image.resize ( xres+1, yres+1 );

	check_and_make_directory ( pngDir.c_str() );

	folder = pngDir;

//	boost::filesystem::path dir ( pngDir.c_str() );
//	boost::filesystem::create_directory ( dir );

}
	
/**
 \brief Draws a line between the points (xa,ya) and (xb,yb).

	Uses <i>Bresenham</i>'s line drawing algorithm to fill all pixels between points
	(xa,ya) and (xb,yb).

	@param xa x-coordinate of first point.
	@param ya y-coordinate of first point.
	@param xb x-coordinate of second point.
	@param yb y-coordinate of second point.
*/
void drawLine(int xa, int ya, int xb, int yb){
	
	int dx,dy,p,x,y,xStart,yStart,xEnd,yEnd;

	dx = abs(xb - xa);
	dy = abs(yb - ya);
	
	int yres = image.get_height()-1;

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

		image[yres-y][x] = png::rgb_pixel(255,255,255);

		while(x<xEnd){

			x = x + 1;
			if (p<0)
				p = p+2*dy;
			else{

				y = y+1*(yEnd - yStart)/dy;
				p = p + 2*(dy-dx);
			}

			image[yres-y][x] = png::rgb_pixel(255,255,255);
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

		image[yres-y][x] = png::rgb_pixel(255,255,255);

		while(y<yEnd){

			y = y + 1;
			if (p<0)
				p = p+2*dx;
			else{

				x = x+1*(xEnd-xStart)/dx;
				p = p + 2*(dx-dy);
			}

			image[yres-y][x] = png::rgb_pixel(255,255,255);
		}
	}
}

/**
 \brief Generates a PNG for the given slice.

	@param s Slice for which PNG is to be generated.
	@param slice_counter Current slice number.
	@param min_x Minimum x-coordinate in the mesh after recentering.
	@param max_x Maximum x-coordinate in the mesh after recentering.
	@param min_y Minimum y-coordinate in the mesh after recentering.
	@param max_y Maximum y-coordinate in the mesh after recentering.
*/
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
			
			_x = (it->x - min_x)/(max_x - min_x) * (SMALLER_DIM-1);
			_y = (it->y - min_y)/(max_y - min_y) * (SMALLER_DIM-1);
		
			x1 = (int)_x;
			y1 = (int)_y;

		}
		
		else{
			first = true;
			
			_x = (it->x - min_x)/(max_x - min_x) * (SMALLER_DIM-1);
			_y = (it->y - min_y)/(max_y - min_y) * (SMALLER_DIM-1);

			x2 = (int)_x;
			y2 = (int)_y;
		
			if(x1>SMALLER_DIM || x2>SMALLER_DIM)
				cout<<"\n Out of bounds";
			else	
			drawLine(x1,y1,x2,y2);
		}
	}	
	
	string pngFileName=folder+"/slice_"+(to_string(slice_counter))+".png";

	image.write(pngFileName);
}
