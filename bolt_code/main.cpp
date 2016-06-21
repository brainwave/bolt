#include "main.hpp"

int main ( int argc, char *argv[] ) {

	float sliceSize = 0.1;
	float pixels_per_mm = 0.5;	

	// time calculation
	clock_t time, startTime = clock();

	//replace with switch case
	if(argc ==1 ) {
			cout<<"\nParameter missing : filename\nExiting ";
			return 1;
	}

    else if(argc == 2) {
			cout<<"\nSlicing "<<argv[1]<<" with default slice size of "<<sliceSize<<" and default pixels/mm as "<<pixels_per_mm;
	}

	else if(argc ==3) {
			sliceSize = atof(argv[2]);
		cout<<"\nSlicing "<<argv[1]<<" with supplied slice size of "<<sliceSize<<" and default pixels/mm as "<<pixels_per_mm;
	}

	else if (argc == 4) {
		sliceSize = atof(argv[2]);
		cout<<"\n Slicing "<<argv[1]<<" with supplied slice size of "<<sliceSize<<" and supplied pixels/mm as "<<pixels_per_mm;
	}


	else if(argc >4) {
			cout << "\nExtraneous parameters supplied, exiting. ";
			return 1;
	}

	//clean up all computations, very shabby right now
	float min_z, max_z,min_x, max_x, min_y, max_y, xrange, yrange, zrange, xcenter, ycenter, zcenter, xscale=2.0f, yscale=2.0f, zscale=2.0f;

	stlMesh mesh;

	time = clock();
	if ( mesh.readStlFile(argv[1]) ) {
		cout << "\nProgram Failed" ;
		return 1;
	}
	else {
		cout<<"\nreadStlFile : "<<(double)(clock() - time)/CLOCKS_PER_SEC;

		mesh.set_min_max_var_z(min_z, max_z, min_x, max_x, min_y, max_y);
	
		//restructure using labmda later
		xrange = (max_x - min_x); 	yrange = (max_y - min_y);	zrange = (max_z - min_z);

		cout<<"\nRanges are: "<<xrange<<" "<<yrange<<" "<<zrange;

		xcenter = min_x + xrange; ycenter = min_y + yrange; zcenter = min_z + zrange;

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
	
		// slicing 

		time = clock();
		mesh.sliceByTriangle(p,s,sliceSize,arr_len);

		cout<<"Time spent in slicing "<<(double) (clock() - time) / CLOCKS_PER_SEC;
		
		for( float i = min_z; i <= max_z-sliceSize; i+=sliceSize )
			if(slice_counter<arr_len) 
			 slice_counter++;
			
		ofstream file;
		file.open("last_run_parameters.txt");
		file<<"SliceCount"<<"\n"<<slice_counter<<"\n";
		
		GLFWwindow* window = glInit(slice_counter, xrange/xscale, yrange/xscale, zrange/xscale, pixels_per_mm);

		showSlice (s++, xscale, yscale, zscale);

		file<<"xScale\n"<<xscale<<"\nyScale"<<"\n"<<yscale<<"\nzScale"<<"\n"<<zscale<<"\n";
		file.close();
		
		showWindow(s, window,xscale, yscale, zscale);

		glfwDestroyWindow(window);	
		glfwTerminate();

		cout<<"\nTotal Program time : "<<(double)(clock() - startTime)/CLOCKS_PER_SEC;

		}
}


