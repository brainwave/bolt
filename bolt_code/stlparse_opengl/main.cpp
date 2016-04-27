#include "meshDef.cpp"
#include "graphics.cpp"
#include <stdlib.h>
#include <array>
#include <unistd.h>

int main ( int argc, char *argv[] ) {

	cout << "\nProgram Starts, file name is : ";
	cout<<argv[1];

	float sliceSize = 0.1;
	float min_z, max_z;
	stlMesh mesh;

	if ( mesh.readStlFile(argv[1]) ) {
		cout << "\nProgram Failed" ;
		return 1;
	}
	else {
		mesh.set_min_max_var_z(min_z, max_z);

		//Add one extra length for the last element, if it falls on max_z
		int arr_len= (int)(((max_z-min_z)/sliceSize))+1;
		static int slice_counter=0; 

		cout << "\n(Diagnostic Msg) Min and max Z are : "<<min_z<<" "<<max_z;

		plane *p = new plane[arr_len];
		slice *s = new slice[arr_len];

				cout << "\n(Diagnostic Msg) Successfully created plane and slice arrays ";
		
		for( float i = min_z; i <= max_z-sliceSize; i+=sliceSize )	{
		
			string filename = "slice_";

			p->create_plane( vec3(0,0,1), i ) ;
			mesh.slice_mesh(p, s);
			s->store_slice(filename, slice_counter); 
			
		
			if(slice_counter<=arr_len) {
			p++; s++; slice_counter++;
			}

		}	

			GLFWwindow* window = glInit(slice_counter);
			//filename, extension, slice_counter);

			showSlice("slice_",".dat", 0 );
			showWindow(window);

			glfwDestroyWindow(window);	
			glfwTerminate();

		}
}


