#include "main.hpp"



int main ( int argc, char *argv[] ) {

	float sliceSize;
	float pixels_per_mm;	

	// time calculation
	clock_t time, startTime = clock();

	if(!checkArguments(argc, argv, sliceSize, pixels_per_mm))
		return 0;
	
	//ranges, min and max z values, and O(verall)scale_x, y and z
	float xrange, yrange, zrange, min_z, max_z, max_x, min_x, max_y, min_y, Oscale_x=0.5f, Oscale_y=0.5f, Oscale_z=0.5f;

	stlMesh mesh;

	time = clock();
	if ( mesh.readStlFile(argv[1]) ) {
		cout << "\nProgram Failed" ;
		return 1;
	}
	else {
		cout<<"\nreadStlFile : "<<(double)(clock() - time)/CLOCKS_PER_SEC;


		mesh.recenter();

		max_x = mesh.getMaxX();
		min_x = mesh.getMinX();
		
		max_y = mesh.getMaxY();
		min_y = mesh.getMinY();
	
		min_z = mesh.getMinZ();
		max_z = mesh.getMaxZ();

		xrange = max_x - min_x;
		yrange = max_y - min_y;
		zrange = max_z - min_z;

		cout<<"\nRanges are "<<" "<<xrange<<" "<<yrange<<" "<<zrange<<" , Z max is at "<<max_z;

		int arr_len= (int)(zrange/sliceSize)+1;

		static int slice_counter=0; 

		plane *p = new plane[arr_len];
		slice *s = new slice[arr_len];

		cout << "\n(Diagnostic Msg) Successfully created plane and slice arrays ";
		
		
		// save the address of the first place
		plane *pstart = p;

		//int j=0;

		// initialize the planes - from min-z to one below max_z		
		for(float i = min_z; i<=max_z-sliceSize && slice_counter<arr_len; i+=sliceSize,slice_counter++,p++){

			p->create_plane( vec3(0,0,1), i ) ;
		}
	
		// restore first place to p
		p=pstart;
	
		// slicing 

		time = clock();
		mesh.sliceMesh(p, s, sliceSize, arr_len);

		cout<<"Time spent in slicing "<<(double) (clock() - time) / CLOCKS_PER_SEC;
		
	//	for( float i = min_z; i <= max_z-sliceSize; i+=sliceSize )
	//		if(slice_counter<arr_len) 
	//		 slice_counter++;
		
		showWindow(s, slice_counter, max_x, min_x, max_y, min_y);

		cout<<"\nTotal Program time : "<<(double)(clock() - startTime)/CLOCKS_PER_SEC;

	}
}

