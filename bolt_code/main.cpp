#include "main.hpp"


int main ( int argc, char *argv[] ) {

	float sliceSize;
	
	string pngDir,fileName;
	
	int xres, yres;
	
	int hollow;

	float thickness;
	
	// time calculation
	clock_t time, startTime = clock();

	if(!checkArguments(argc, argv, fileName, sliceSize, pngDir, xres, yres, hollow, thickness))
		return 0;
	
	//ranges, min and max z values, and O(verall)scale_x, y and z
	float xrange, yrange, zrange, min_z, max_z, max_x, min_x, max_y, min_y;

	stlMesh mesh;

	time = clock();

	// perform hollowing routine if needed	
	if(hollow){
	
		string hollowingCommand = "openscad -o "+pngDir+"/hollow.stl -D 'model=\""+fileName+"\"' -D 'thickness="+to_string(thickness)+"' hollow.scad";

		system(hollowingCommand.c_str());	

		fileName = pngDir+"/hollow.stl";
	}

	if ( mesh.readStlFile(fileName.c_str())) {
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

		int slice_counter=0,max_slice_no=0; 

		plane *p = new plane[arr_len];
		slice *s = new slice[arr_len];

		cout << "\n(Diagnostic Msg) Successfully created plane and slice arrays ";
		
		// save the address of the first place
		plane *pstart = p;

		// initialize the planes - from min-z to one below max_z		
		for(float i = min_z; i<=max_z-sliceSize && max_slice_no<arr_len; i+=sliceSize,max_slice_no++,p++){

			p->create_plane( vec3(0,0,1), i ) ;
		}
	
		// restore first place to p
		p=pstart;
	
		// slicing 

		time = clock();
		mesh.sliceMesh(p, s, sliceSize, arr_len);

		cout<<"Time spent in slicing "<<(double) (clock() - time) / CLOCKS_PER_SEC;

		initPNG ( xres, yres, pngDir );
		
		// filling and png generation
		for(slice_counter=0;slice_counter<max_slice_no;slice_counter++){
	
			s->fillSlice();
			generatePNG(*s,slice_counter,min_x,max_x,min_y,max_y);
			s++;
		}

		cout<<"\nTotal Program time : "<<(double)(clock() - startTime)/CLOCKS_PER_SEC;
	}
}

