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

	
bool checkArguments(int argc, char *argv[], float &sliceSize, string &pngDir, int &xres, int &yres, int &hollow){
	
	sliceSize = 0.1;
	pngDir = "png";
	xres = 800;
	yres = 600;
	hollow = 0;

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

		case 7:
			is_slice_size_sane ( argv [2], sliceSize);

			pngDir = argv[3];

			xres = atoi ( argv[4] );
			yres = atoi ( argv[5] );

			hollow = atoi( argv[6] );

			cout << "\nSlicing " <<argv[1] << " with supplied slice size of " << sliceSize
				<< " and supplied png directory ( " << argv[3] <<" ) ";
			
			cout << "\nUsing resolutions of "<<xres<<" and "<<yres;
			
			if(hollow)
				cout << "\n Hollowing";
			
			break;

		default: 
			cout << "\nExtraneous parameters supplied, exiting. ";
			return false;
	}
	
	return true;

}
