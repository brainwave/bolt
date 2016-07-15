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
#include <unistd.h>
#include <time.h>
#include <chrono>
#include <fstream>
#include <math.h>

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
bool checkArguments(int argc, char *argv[], string &fileName, float &sliceSize, string &pngDir, int &xres, int &yres, bool &hollow, float &thickness, bool &support){
	
	fileName = "";
	sliceSize = 0.1;
	pngDir = "png";
	xres = 800;
	yres = 600;
	hollow = false;
	support = false;
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
			
			hollow = true;
		}
		else if(strcmp(argv[i],"-t") == 0){ // wall thickness
		
			thickness = atof(argv[i+1]);
			
			if(thickness<=0 || thickness == NAN){
	
				cout<<"\n Invalid thickness!";
				return false;
			}
		}
		else if(strcmp(argv[i],"-sg") == 0){ //support generation
			support = true;
		}	
	}

	if(fileName==""){
	
		cout<<"\n Please specify a file to be sliced.";
		return false;
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
	
	return true;

}

void writeSCAD(stlMesh mesh, string in_filename, string &out_filename, float thickness = 0.75, int divisions = 8, float tip_height = 10){
	
	mesh.getMinMax();
	float x_interval = (mesh.getMaxX() - mesh.getMinX())/divisions;
	float y_interval = (mesh.getMaxY() - mesh.getMinY())/divisions;
	mesh.boundBox(x_interval,y_interval);

	float x,y,z,h,r;
	r = thickness;
	fstream outfile;
	outfile.open("/home/nikki/bolt/bolt_code/support.scad", ios::out | ios::trunc);

	if(outfile.is_open()) {
		
		outfile<<"union() { \n";
		outfile<<"import(\""<<in_filename<<"\");\n";
	
		//For loop
		for(auto it = mesh.supports.begin(); it!=mesh.supports.end(); it++) {
			x = it->xy_point.x; y = it->xy_point.y;

			if(it->z_vector.size() % 2 == 0) {	//even
				for(auto jt = it->z_vector.begin(); jt!= it->z_vector.end(); jt+=2) {

					z = *jt;
					h = *(jt+1);	tip_height = h/10;

					//Bottom cone
					if(z!=0) {
						outfile<<"translate(["<<x<<","<<y<<","<<z<<"]) ";
						outfile<<"cylinder("<<tip_height<<","<<r/5<<","<<r<<");\n";
					}
					else {
						outfile<<"translate(["<<x<<","<<y<<","<<z<<"]) ";
						outfile<<"cylinder("<<tip_height<<","<<r<<","<<r<<");\n";
					}

					//Main support
					z = z+tip_height;
					h = h-tip_height-z;
					outfile<<"translate(["<<x<<","<<y<<","<<z<<"]) ";
					outfile<<"cylinder("<<h<<","<<r<<","<<r<<");\n";

					//Top cone
					z = z + h;
					outfile<<"translate(["<<x<<","<<y<<","<<z<<"])";
					outfile<<"cylinder("<<tip_height<<","<<r<<","<<r/5<<");\n";

						
				}
			}
			else {					//odd
				for(auto jt = it->z_vector.begin(); jt!= it->z_vector.end()-1; jt+=2) {

					z = *jt;
					h = *(jt+1);	tip_height = h/10;

					//Bottom cone
					if(z!=0) {
						outfile<<"translate(["<<x<<","<<y<<","<<z<<"]) ";
						outfile<<"cylinder("<<tip_height<<","<<r/5<<","<<r<<");\n";
					}
					else {
						outfile<<"translate(["<<x<<","<<y<<","<<z<<"]) ";
						outfile<<"cylinder("<<tip_height<<","<<r<<","<<r<<");\n";
					}

					//Main support
					z = z+tip_height;
					h = h-tip_height-z;
					outfile<<"translate(["<<x<<","<<y<<","<<z<<"]) ";
					outfile<<"cylinder("<<h<<","<<r<<","<<r<<");\n";

					//Top cone
					z = z + h;
					outfile<<"translate(["<<x<<","<<y<<","<<z<<"])";
					outfile<<"cylinder("<<tip_height<<","<<r<<","<<r/5<<");\n";
				}
			}
					
		
		}
	
		outfile<<"}\n";	
		cout<<"\nWrote to file\n"; 
	}
	else
		cout<<"ERROR: COULD NOT OPEN FILE\n";
	outfile.close();

	string cmd ="openscad -o support.stl support.scad";
	out_filename = "support.stl";
	system(cmd.c_str());
}
	

/*
	OLD CODE FOR PASSING VALUES INTO OPENSCAD PROGRAMS

	string cmd ="openscad -o support.stl -D 'min_x ="+to_string(mesh.getMinX())+"' -D 'min_y = "+to_string(mesh.getMinY());
	cmd += "' -D 'max_x = "+to_string(mesh.getMaxX())+"' -D 'max_y = "+to_string(mesh.getMaxY())+"' -D 'height = "+to_string(mesh.getMaxZ());
	cmd += "' -D 'thickness = "+to_string(thickness)+"' -D 'x_interval ="+to_string(x_interval)+"' -D 'y_interval ="+to_string(y_interval)+"' -D 'infile = \""+in_filename+"\"'";

	cmd += " -D 'list_x = ["+to_string(mesh.supportPoints.at(0).x);
	for(int i=1; i<mesh.supportPoints.size(); i++)
		cmd += ","+to_string(mesh.supportPoints[i].x);
	cmd += "]' ";

	cmd += " -D 'list_y = ["+to_string(mesh.supportPoints.at(0).y);
	for(int i=1; i<mesh.supportPoints.size(); i++)
		cmd += ","+to_string(mesh.supportPoints[i].y);
	cmd += "]' ";

	cmd += " -D 'list_z = ["+to_string(mesh.supportPoints.at(0).z);
	for(int i=1; i<20; i++)
		cmd += ","+to_string(mesh.supportPoints[i].z);
	cmd += "]' ";


	//Convert the z-list to a 2D array for instant lookup (instead of iteration)
	vector<vector<float>> z_array( mesh.supportPoints.size(), vector<float>(mesh.supportPoints.size())); 
	int k=0;
	for(int i=0; i<mesh.supportPoints.size(); i++, k++){
		for(int j=0; j<mesh.supportPoints.size(); j++){
			z_array[i][j] = mesh.supportPoints.at(k).z;
		}
	}
	cmd += " -D 'list_z = [ ";
	for(int i=0; i<z_array.size(); i++){
		for(int j=0; j<z_array.at(i).size(); j++){
			if(j==0)
				cmd += "["+to_string(z_array[i][j]);
			else
				cmd += ","+to_string(z_array[i][j]);
		}
		if(i==z_array.size()-1)
			cmd += "] ";
		else
			cmd += "],";
	}
	cmd += "]' ";

	cmd += " support.scad"; 
	out_filename = "support.stl";
	system(cmd.c_str());
}
*/

