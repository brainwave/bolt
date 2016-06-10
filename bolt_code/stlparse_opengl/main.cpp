#include "meshDef.cpp"
#include "graphics.cpp"
#include <stdlib.h>
#include <array>
#include <unistd.h>

int main ( int argc, char *argv[] ) {

	float sliceSize = 0.1;
		//replace with switch case
	if(argc ==1 ) {
			cout<<"\nParameter missing : filename\nExiting ";
			return 1;
	}

    else if(argc == 2) {
			cout<<"\nSlicing "<<argv[1]<<" with default slice size of "<<sliceSize;
	}

	else if(argc ==3) {
			sliceSize = atof(argv[2]);
		cout<<"\nSlicing "<<argv[1]<<" with supplied slice size of "<<sliceSize;
	}

	else if(argc >3) {
			cout << "\nExtraneous parameters supplied, exiting. ";
			return 1;
	}

	//clean up all computations, very shabby right now
	float min_z, max_z,min_x, max_x, min_y, max_y, xrange, yrange, zrange, xcenter, ycenter, zcenter, xscale=1.f, yscale=1.f, zscale=1.f;

	stlMesh mesh;

	if ( mesh.readStlFile(argv[1]) ) {
		cout << "\nProgram Failed" ;
		return 1;
	}
	else {
		mesh.set_min_max_var_z(min_z, max_z, min_x, max_x, min_y, max_y);
	
		//restructure using labmda later
		xrange = (max_x - min_x)/2;
		yrange = (max_y - min_y)/2;
		zrange = (max_z - min_z)/2;

		cout<<"\nRanges are: "<<xrange<<" "<<yrange<<" "<<zrange;

		xcenter = min_x + xrange;
		ycenter = min_y + yrange;
		zcenter = min_z + zrange;

		//update new coordinates
		min_z = min_z - zcenter; max_z -= zcenter;
		min_y = min_y - ycenter; max_y -= ycenter;
		min_x = min_x - xcenter; max_x -= xcenter;

		//Center Mesh, using extremums
		mesh.recenter(xcenter, ycenter, zcenter);

		//Add one extra length for the last element, if it falls on max_z
//		cout<<"\n(Diagnostic Msg) Model centered around point "<<xcenter<<" "<<ycenter<<" "<<zcenter<<" Units";

		int arr_len= (int)(((max_z-min_z)/sliceSize))+1;
		static int slice_counter=0; 

		plane *p = new plane[arr_len];
		slice *s = new slice[arr_len];

		cout << "\n(Diagnostic Msg) Successfully created plane and slice arrays ";
		
		
		// save the address of the first place
		plane *pstart = p;

		int j=0;

		// initialize the planes - from min-z to one below max_z		
		for(float i = min_z; i<=max_z-sliceSize && j<arr_len; i+=sliceSize,j++,p++){

			p->create_plane( vec3(0,0,1), i ) ;
		}
	
		// restore first place to p
		p=pstart;
		
		mesh.sliceByTriangle(p,s,sliceSize);
		
		// store the slices 
		for( float i = min_z; i <= max_z-sliceSize; i+=sliceSize ){

			string filename = ".slice_";	
			s->store_slice(filename, slice_counter); 
			if(slice_counter<=arr_len) {
			 s++; slice_counter++;
			}
		}

		printf("\n Number of slices: %d",slice_counter);


			ofstream file;
			file.open("last_run_parameters.txt");
			file<<"SliceCount"<<"\n"<<slice_counter<<"\n";
					GLFWwindow* window = glInit(slice_counter, xrange, yrange, zrange);
			//filename, extension, slice_counter);

			showSlice(".slice_",".dat", 0, xscale, yscale, zscale);
			file<<"xScale"<<"\n"<<xscale<<"\n";
			file<<"yScale"<<"\n"<<yscale<<"\n";
			file<<"zScale"<<"\n"<<zscale<<"\n";
			file.close();


			showWindow(window,xscale, yscale, zscale);

			glfwDestroyWindow(window);	
			glfwTerminate();

		}
}


