#include "main.hpp"

int main ( int argc, char *argv[] ) {

	float sliceSize = 0.1;
	float pixels_per_mm = 0.5;	

	// time calculation
	clock_t begin = clock();
	clock_t end;
	double time_spent;
	

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
	float min_z, max_z,min_x, max_x, min_y, max_y, xrange, yrange, zrange, xcenter, ycenter, zcenter, xscale=1.0f, yscale=1.0f, zscale=1.0f;

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
//			cout<<"\nCreated Plane "<<p->distance;
		}
	
		// restore first place to p
		p=pstart;
	
		// slicing 
		clock_t beginSliceTime, endSliceTime;	

		beginSliceTime = clock();

		mesh.sliceByTriangle(p,s,sliceSize,arr_len);

		endSliceTime = clock();

		time_spent = (double) (endSliceTime - beginSliceTime) / CLOCKS_PER_SEC;

		printf("\nTotal slicing time: %lf", time_spent);	
		clock_t startTime = clock();
		
		// store the slices 
		for( float i = min_z; i <= max_z-sliceSize; i+=sliceSize ){
			if(slice_counter<arr_len) {
			 slice_counter++;
			}
		}
		
		
			ofstream file;
			file.open("last_run_parameters.txt");
			file<<"SliceCount"<<"\n"<<slice_counter<<"\n";
			
			GLFWwindow* window = glInit(slice_counter, xrange/xscale, yrange/xscale, zrange/xscale, pixels_per_mm);
			//filename, extension, slice_counter);

//			showSlice("dat/slice_",".dat", 0, xscale, yscale, zscale);
//
			showSlice (s++, xscale, yscale, zscale);

			file<<"xScale"<<"\n"<<xscale<<"\n";
			file<<"yScale"<<"\n"<<yscale<<"\n";
			file<<"zScale"<<"\n"<<zscale<<"\n";
			file.close();
			
			end = clock();
			time_spent = (double) (end - begin) / CLOCKS_PER_SEC;
			printf("\n Total Time Spent: %lf ", time_spent);
			
			showWindow(s, window,xscale, yscale, zscale);

			glfwDestroyWindow(window);	
			glfwTerminate();


		}
}


