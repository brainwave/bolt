/**
* @file main.hpp
* \brief Header file for main.cpp
*
* Includes all other header files and programs. 
* Also defines helper methods for main.
*/

#include "meshDef.cpp"
#include "slicer.cpp"
#include "pngsupport.cpp"
#include <stdlib.h>
#include <array>
#include <unistd.h>
#include <time.h>
#include <chrono>
#include <fstream>

bool is_slice_size_sane ( char* sliceArgument , float& sliceSize ){


	if(sliceSize>1 || sliceSize<0.001 || sliceSize==NAN){

		cout<<"\nPlease specify a slice size between 0.001 and 1 ";
		return false;
	}

	else {
		sliceSize = atof(sliceArgument);	
		return true;
	}
}

	
bool checkArguments(int argc, char *argv[], float &sliceSize, string &pngDir, int &xres, int &yres){
	
	sliceSize = 0.1;
	pngDir = "png";
	xres = 800;
	yres = 600;

	switch(argc){
	
		case 1: 
			cout<<"\nParameter missing : filename\nExiting ";
			return false;
			
		case 2:
			cout << "\nSlicing " << argv[1] << " with default slice size of " << sliceSize
				<< " and default png directory (" << pngDir << " )";
			break;		
	
		case 3: 

			is_slice_size_sane ( argv[2], sliceSize);

			cout << "\nSlicing " <<argv[1] << " with supplied slice size of " << sliceSize
				<< " and default png directory ( " << pngDir <<" ) ";
			break;
		case 4: 

			is_slice_size_sane ( argv [2], sliceSize);
				
			pngDir = argv[3];

			cout << "\nSlicing " <<argv[1] << " with supplied slice size of " << sliceSize
				<< " and supplied png directory ( " << pngDir <<" ) ";


			break;

		case 6:
			is_slice_size_sane ( argv [2], sliceSize);

			pngDir = argv[3];

			xres = atoi ( argv[4] );
			yres = atoi ( argv[5] );

			cout << "\nSlicing " <<argv[1] << " with supplied slice size of " << sliceSize
				<< " and supplied png directory ( " << argv[3] <<" ) ";
			
			cout << "\nUsing resolutions of "<<xres<<" and "<<yres;
			break;

		default: 
			cout << "\nExtraneous parameters supplied, exiting. ";
			return false;
	}
	
	return true;

}

void writeSCAD(stlMesh mesh, char* filename, vector<vec3> supportPoints){

	//Calculate values to recenter shape by
	float xcentre = (abs(mesh.getMinX())+mesh.getMaxX()) * -1;
	float ycentre = (abs(mesh.getMinY())+mesh.getMaxY()) * -1;
	float zcentre = 0;
	
	float h = mesh.getMaxZ() - mesh.getMinZ(), r = 2;
	float x,y,z;	x = y = z = 0;
	
	fstream outfile; 	
	
	outfile.open("/home/nikki/bolt/bolt_code/output.scad", ios::out | ios::trunc);
	if(outfile.is_open()) {

		outfile<<"union() { \n";
		outfile<<"translate(["<<xcentre<<","<<ycentre<<","<<zcentre<<"]) ";	//recenter shape
		outfile<<"import(\""<<filename<<"\");\n";
		
		//For loop
		for(auto it = supportPoints.begin(); it!=supportPoints.end(); it++) {	
			x = it->x; y = it->y; z = it->z;
			outfile<<"translate(["<<x<<","<<y<<","<<z<<"]) ";
			outfile<<"cylinder("<<h<<","<<r<<","<<r<<");\n";
		}
		outfile<<"}\n";	
		cout<<"\nWrote to file\n"; 
	}
	
	else
		cout<<"ERROR: COULD NOT OPEN FILE\n";
	outfile.close();
	
}
