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
#include <string>
#include <time.h>
#include <fstream>
#include "boost/threadpool.hpp"

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


bool isswitch(char *a){
	
	if( 	strcmp(a,"-f") == 0 || strcmp(a,"-s") == 0 || strcmp(a,"-h") == 0 || strcmp(a,"-o") == 0 || strcmp(a,"-sg") == 0
		|| strcmp(a,"-r") == 0 || strcmp(a,"-t") == 0 )
		return true;
	else
		return false;
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
void setArguments(int argc, char *argv[], string &fileName, float &sliceSize, string &pngDir, int &xres, int &yres, bool &hollow, float &thickness, bool &support){
	
	fileName = "";
	sliceSize = 0.1;
	pngDir = "png";
	xres = 800;
	yres = 600;
	hollow = false;
	support = false;
	thickness = 0.1;
	
	if(argc>=15){
	
		//cout<<"\n Extraneous parameters supplied! Exiting!";
		//return false;
	
		throw Exception("EXTRANEOUS_PARAMETERS");
	}

	for(int i=0; i<argc; i++){
	
		if(strcmp(argv[i],"-f") == 0){ // file name switch
		
			if(i==argc-1 || (i!=argc-1 && isswitch(argv[i+1]))){
	
				throw Exception("MISSING_FILE_NAME");
			}
			fileName = argv[i+1];
		}
		else if(strcmp(argv[i],"-s") == 0){ // slice size switch
	
			if(i==argc-1 || (i!=argc-1 && isswitch(argv[i+1]))){
	
				throw Exception("MISSING_SLICE_SIZE");
			}
			if(!is_slice_size_sane ( argv[i+1], sliceSize)){
				
				throw Exception("BAD_SLICE_SIZE");
			}
		}
		else if(strcmp(argv[i],"-o") == 0){ // png output directory switch
			
			if(i==argc-1 || (i!=argc-1 && isswitch(argv[i+1]))){
	
				throw Exception("MISSING_OUTPUT_DIRECTORY");
			}
			
			pngDir = argv[i+1];
		}
		else if(strcmp(argv[i],"-r") == 0){ // resolution switch
	
			if(i>=argc-2 || (i<argc-2 && (isswitch(argv[i+1]) || isswitch(argv[i+2]) ))){
	
				throw Exception("MISSING_RESOLUTION");
			}

			xres = atoi ( argv[i+1] );
			yres = atoi ( argv[i+2] );

			if(xres <=0 || yres<=0){
			
				//cout<<"\n Invalid resolution specified.";
				//return false;
				throw Exception ("BAD_RESOLUTION");
			}
		}
		else if(strcmp(argv[i],"-h") == 0){ // hollow switch
			
			hollow = true;
		}
		else if(strcmp(argv[i],"-t") == 0){ // wall thickness
		
			if(i==argc-1 || (i!=argc-1 && isswitch(argv[i+1]))){
	
				throw Exception("MISSING_THICKNESS");
			}
			

			thickness = atof(argv[i+1]);
			
			if(thickness<=0 || thickness == NAN){
	
				//cout<<"\n Invalid thickness!";
				//return false;
				throw Exception ("BAD_THICKNESS");
			}
		}
		else if(strcmp(argv[i],"-sg") == 0){ //support generation

			support = true;
		}	
	}

	cout<<"\n File Name             : "<<fileName;
	cout<<"\n Slice Size            : "<<sliceSize;
	cout<<"\n Resolution            : "<<xres<<"*"<<yres;
	cout<<"\n Hollowing             : ";
	if(hollow) {
		
		cout<<"Yes";
		cout<<"\n Wall Thickness        : "<<thickness;
	}
	else
		cout<<"No";

	cout<<"\n Output Directory      : "<<pngDir;
	cout<<"\n Support Generation    : ";
	if(support) 
		cout<<"Yes";
	else
		cout<<"No";
	cout<<"\n";	

}

void writeSCAD(stlMesh mesh, string in_filename, string &out_filename, float thickness = 2.0, float interval = 15.0){
	
	mesh.getMinMax();
	string cmd ="openscad -o support.stl -D 'min_x ="+to_string(mesh.getMinX())+"' -D 'min_y = "+to_string(mesh.getMinY());
	cmd += "' -D 'max_x = "+to_string(mesh.getMaxX())+"' -D 'max_y = "+to_string(mesh.getMaxY())+"' -D 'height = "+to_string(mesh.getMaxZ()-mesh.getMinZ());
	cmd += "' -D 'thickness = "+to_string(thickness)+"' -D 'interval ="+to_string(interval)+"' -D 'infile = \""+in_filename+"\"' output.scad"; 
	out_filename = "support.stl";
	system(cmd.c_str());

}


/**
	\brief Function that is multi-threaded.
	
	Fills a slice and generates a PNG for it.
*/
void thread_function(slice s, int slice_counter, float min_x, float max_x, float min_y, float max_y, int xres, int yres){

	s.fillSlice(xres, yres);
	generatePNG(s,slice_counter,min_x,max_x,min_y,max_y,xres,yres);
}

	

