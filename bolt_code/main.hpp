#include "meshDef.cpp"
#include "graphics.cpp"
#include <stdlib.h>
#include <array>
#include <unistd.h>
#include <time.h>
#include <chrono>


bool checkArguments(int argc, char *argv[], float &sliceSize, float &pixels_per_mm){
	
	sliceSize = 0.1;
	pixels_per_mm = 0.5;

	switch(argc){
	
		case 1: 
			cout<<"\nParameter missing : filename\nExiting ";
			return false;
			
		case 2:
			cout<<"\nSlicing "<<argv[1]<<" with default slice size of "<<sliceSize<<" and default pixels/mm as "<<pixels_per_mm;
			break;		
	
		case 3: 
			sliceSize = atof(argv[2]);	
			if(sliceSize>1 || sliceSize<0.001 || sliceSize==NAN){
			
				cout<<"\nPlease specify a slice size between 0.001 and 1 ";
				return false;
			}
			cout<<"\nSlicing "<<argv[1]<<" with supplied slice size of "<<sliceSize<<" and default pixels/mm as "<<pixels_per_mm;
			break;

		case 4:
			sliceSize = atof(argv[2]);
			if(sliceSize>1 || sliceSize<0.001 || sliceSize==NAN){
	
				cout<<"\nPlease specify a slice size between 0.001 and 1 ";
				return false;
			} 

			pixels_per_mm = atof(argv[3]);
			if(pixels_per_mm <= 0 || pixels_per_mm==NAN){
				
				cout<<"\nPlease specify a positive pixel-per-mm value";
				return false;
			}
			cout<<"\n Slicing "<<argv[1]<<" with supplied slice size of "<<sliceSize<<" and supplied pixels/mm as "<<pixels_per_mm;
			break;


		default: 
			cout << "\nExtraneous parameters supplied, exiting. ";
			return false;
	}
	
	return true;

}
