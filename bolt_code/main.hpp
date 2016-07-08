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

/**
	\brief Checks if the specified slice size is acceptable.
	
*/
bool is_slice_size_sane ( char* sliceArgument , float& sliceSize ){

		
	sliceSize = atof(sliceArgument);

	if(sliceSize>1 || sliceSize<0.001 || sliceSize==NAN){

		cout<<"\nPlease specify a slice size between 0.001 and 1 ";
		return false;
	}

	else {
		return true;
	}
}

/**
	\brief 	Checks if the supplied command line arguments are valid and assigns them to variables in main. 
	
	@param argc Number of command line arguments.
	@param argv Command line arguments.
	@param fileName Name of the file to be sliced.
	@param sliceSize Distance between two slices.	
	@param pndDir Directory where the generated PNGs will be stored.
	@param xres X Resolution
	@param yres Y Resolution
	@param hollow Set to 0 if hollowing is not required.
	@param thickness The thickness of the wall if hollowing is used.
	 
*/	
bool checkArguments(int argc, char *argv[], string &fileName, float &sliceSize, string &pngDir, int &xres, int &yres, int &hollow, float &thickness){
	
	fileName = "";
	sliceSize = 0.1;
	pngDir = "png";
	xres = 800;
	yres = 600;
	hollow = 0;
	thickness = 0.1;
	
	if(argc>=15){
	
		cout<<"\n Extraneous parameters supplied! Exiting!";
		return false;
	}

	for(int i=0; i<argc; i++){
	
		if(strcmp(argv[i],"-f") == 0){ // file name switch
		
			fileName = argv[i+1];
		}
		else if(strcmp(argv[i],"-s") == 0){ // slice size switch
	
			if(!is_slice_size_sane ( argv[i+1], sliceSize))
				return false;
		}
		else if(strcmp(argv[i],"-o") == 0){ // png output directory switch
			
			pngDir = argv[i+1];
		}
		else if(strcmp(argv[i],"-r") == 0){ // resolution switch
	
			xres = atoi ( argv[i+1] );
			yres = atoi ( argv[i+2] );

			if(xres <=0 || yres<=0){
			
				cout<<"\n Invalid resolution specified.";
				return false;
			}
		}
		else if(strcmp(argv[i],"-h") == 0){ // hollow switch
			
			hollow = atoi(argv[i+1]);
		}
		else if(strcmp(argv[i],"-t") == 0){ // wall thickness
		
			thickness = atof(argv[i+1]);
			
			if(thickness<=0 || thickness == NAN){
	
				cout<<"\n Invalid thickness!";
				return false;
			}
		}
	}

	if(fileName==""){
	
		cout<<"\n Please specify a file to be sliced.";
		return false;
	}
		
	cout<<"\n File Name       : "<<fileName;
	cout<<"\n Slice Size      : "<<sliceSize;
	cout<<"\n Resolution      : "<<xres<<"*"<<yres;
	cout<<"\n Hollowing       : ";
	if(hollow) {
		
		cout<<"Yes";
		cout<<"\n Wall Thickness  : "<<thickness;
	}
	else
		cout<<"No";
	cout<<"\n Output Directory: "<<pngDir;
	cout<<"\n";	
	
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
