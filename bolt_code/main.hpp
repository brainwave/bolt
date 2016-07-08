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

	
bool checkArguments(int argc, char *argv[], string &fileName, float &sliceSize, string &pngDir, int &xres, int &yres, int &hollow){
	
	fileName = "";
	sliceSize = 0.1;
	pngDir = "png";
	xres = 800;
	yres = 600;
	hollow = 0;

	/*
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
	*/	

	
	for(int i=0; i<argc; i++){
	
		if(strcmp(argv[i],"-f") == 0){ // file name switch
		
			fileName = argv[i+1];
		}
		else if(strcmp(argv[i],"-s") == 0){ // slice size switch
	
			is_slice_size_sane ( argv[i+1], sliceSize);
		}
		else if(strcmp(argv[i],"-o") == 0){ // png directory switch
			
			pngDir = argv[i+1];
		}
		else if(strcmp(argv[i],"-r") == 0){ // resolution switch
	
			xres = atoi ( argv[i+1] );
			yres = atoi ( argv[i+2] );
		}
		else if(strcmp(argv[i],"-h") == 0){ // hollow swtich
			
			hollow = atoi(argv[i+1]);
		}
	}

	if(fileName==""){
	
		cout<<"\n Please specify a file to be sliced.";
		return false;
	}
		
	cout<<"\n File Name       : "<<fileName;
	cout<<"\n Slice Size      : "<<sliceSize;
	cout<<"\n Resolution      : "<<xres<<"*"<<yres;
	cout<<"\n Hollowing       : ";if(hollow) cout<<"Yes"; else cout<<"No";
	cout<<"\n Output Directory: "<<pngDir;
	cout<<"\n";	
	
	return true;

}
