#include "main.hpp"



int main ( int argc, char *argv[] ) {

	float sliceSize;
	float pixels_per_mm;	

	// time calculation
	clock_t time, startTime = clock();

	if(!checkArguments(argc, argv, sliceSize, pixels_per_mm))
		return 0;
	
	//ranges, min and max z values, and O(verall)scale_x, y and z
	float xrange, yrange, zrange, min_z, max_z, Oscale_x=1.0f, Oscale_y=1.0f, Oscale_z=1.0f;

	stlMesh mesh;

	time = clock();
	if ( mesh.readStlFile(argv[1]) ) {
		cout << "\nProgram Failed" ;
		return 1;
	}
	else {
		cout<<"\nreadStlFile : "<<(double)(clock() - time)/CLOCKS_PER_SEC;


		mesh.recenter(xrange, yrange, zrange, max_z, min_z);

		cout<<"\nRanges are "<<" "<<xrange<<" "<<yrange<<" "<<zrange<<" , Z max is at "<<max_z;

		int arr_len= (int)(zrange/sliceSize)+1;

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
	
		//SUPPORT GENERATION
		mesh.supportGenerator(s, arr_len, 15);	//skipAmnt = 15 slices	

		for( float i = min_z; i <= max_z-sliceSize; i+=sliceSize )
			if(slice_counter<arr_len) 
			 slice_counter++;
			
		ofstream file;
		file.open("last_run_parameters.txt");
		file<<"SliceCount"<<"\n"<<slice_counter<<"\n";
		
		GLFWwindow* window = glInit(slice_counter, xrange*Oscale_x, yrange*Oscale_y, zrange*Oscale_z, pixels_per_mm);

		showSlice (s++, xrange, yrange, zrange);

		file<<"xScale\n"<<xscale<<"\nyScale"<<"\n"<<yscale<<"\nzScale"<<"\n"<<zscale<<"\n";
		file.close();
		
		showWindow(s, window, Oscale_x, Oscale_y, Oscale_z);

		glfwDestroyWindow(window);	
		glfwTerminate();

		cout<<"\nTotal Program time : "<<(double)(clock() - startTime)/CLOCKS_PER_SEC;

		}
}
