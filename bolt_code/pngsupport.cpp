/**
* @file pngsupport.cpp
* \brief PNG Support File.
*
*	Defines methods for generating a PNG from the given file.
*/



#define SMALLER_DIM ( yres < xres ? yres : xres )
#include "lodepng.cpp"
#ifdef __linux__

//#include <png++/png.hpp>
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


//png::image <png::rgb_pixel> image(800, 600);
//int pixel_buffer[1000][1000];
//int xres,yres;
string folder = "png";

//unsigned char* image;



void initPNG (int xres, int yres, string pngDir ) {

//	image.resize ( xres+1, yres+1 );

//	image = new unsigned char[ (xres) * (yres) * 3 ];

//	::yres = image.get_height() - 1;

//	::yres = yres;
//	::xres = xres;

//	cout<<"\n Y RES = "<<yres;
		
	check_and_make_directory ( pngDir.c_str() );

	folder = pngDir;

//	boost::filesystem::path dir ( pngDir.c_str() );
//	boost::filesystem::create_directory ( dir );

}


void writePNG(string pngFileName, int **pixel_buffer, int xres, int yres){
	
	unsigned char* image = new unsigned char [xres*yres*3];

	unsigned char* image_iterator = image;

	for(int y = 0; y<yres; y++){
	
		for(int x = 0; x<xres; x++){
	
			if(pixel_buffer[y][x] == 1){
			
				for(int i=0; i<3; i++){
	
					*(image_iterator) = 255;
					image_iterator++;
				}
				
			}
			else{
			
				for(int i=0; i<3; i++){
	
					*(image_iterator) = 0;
					image_iterator++;
				}
			}
		}
	}


	unsigned f = lodepng_encode24_file(pngFileName.c_str(), image, xres, yres); 
	delete image;
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
void drawLine(int xa, int ya, int xb, int yb, int **pixel_buffer, int yres){
	
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

		pixel_buffer[yres-y][x] = 1;	
	
		while(x<xEnd){

			x = x + 1;
			if (p<0)
				p = p+2*dy;
			else{

				y = y+1*(yEnd - yStart)/dy;
				p = p + 2*(dy-dx);
			}

			pixel_buffer[yres-y][x] = 1;	
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

		pixel_buffer[yres-y][x] = 1;	

		while(y<yEnd){

			y = y + 1;
			if (p<0)
				p = p+2*dx;
			else{

				x = x+1*(xEnd-xStart)/dx;
				p = p + 2*(dx-dy);
			}

			pixel_buffer[yres-y][x] = 1;	
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
void generatePNG(slice s, int slice_counter, float min_x, float max_x, float min_y, float max_y, int xres, int yres){

	vector<glm::vec3> vertices;

	int **pixel_buffer = new int * [yres];
	
	for(int i=0;i<yres+1;i++)
		pixel_buffer[i] = new int [xres];
	
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
	
	for (int y = 0; y < yres; y++ )
		for (int x=0; x < xres; x++) {
		
		pixel_buffer[y][x] = 0;
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
		
			drawLine(x1,y1,x2,y2,pixel_buffer,yres);
		}
	}	
	
	string pngFileName=folder+"/slice_"+(to_string(slice_counter))+".png";

	writePNG(pngFileName,pixel_buffer,xres,yres);

	for(int i=0;i<yres;i++)
		delete[] pixel_buffer[i];
	delete pixel_buffer;
}
