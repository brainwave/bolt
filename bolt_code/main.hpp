/**
* @file main.hpp
* \brief Header file for main.cpp
*
* Includes all other header files and programs. 
* Also defines helper methods for main.
*/

#include "meshDef.cpp"
#include "graphics.cpp"
#include "slicer.cpp"
#include "pngsupport.cpp"
#include <stdlib.h>
#include <array>
#include <unistd.h>
#include <time.h>
#include <chrono>

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

	
bool checkArguments(int argc, char *argv[], float &sliceSize, string &pngDir){
	
	sliceSize = 0.1;
	pngDir = "png";

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

			is_slice_size_sane ( argv[2], sliceSize);
				
			cout << "\nSlicing " <<argv[1] << " with supplied slice size of " << sliceSize
				<< " and supplied png directory ( " << argv[3] <<" ) ";

			pngDir = argv[3];

			break;

		default: 
			cout << "\nExtraneous parameters supplied, exiting. ";
			return false;
	}
	
	return true;

}
