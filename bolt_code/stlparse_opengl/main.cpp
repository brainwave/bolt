#include "meshDef.cpp"
#include <array>

int main ( int argc, char *argv[] ) {


	cout<<"\nProgram Starts, file name is : ";
	cout<<argv[1];

	float sliceSize = 0.1;
	float min_z, max_z;
	stlMesh mesh;



	if ( mesh.readStlFile(argv[1]) )
		cout << "\nProgram Failed" ;
	
	else {
		mesh.set_min_max_var_z(min_z, max_z);
		
	
		int arr_len= (int)(((max_z-min_z)/sliceSize));
		static int slice_counter=0; 

		plane *p = new plane[arr_len];
		slice *s = new slice[arr_len];

		cout<<"\n(Diagnostic Msg) Min and max Z are : "<<min_z<<" "<<max_z;
	
		float sliceSize = 0.1f;

		cout<<"\n(Diagnostic Msg) Successfully created plane and slice arrays ";
		
		for( float i = min_z; i <= max_z; i+=sliceSize ) {
		
			p->create_plane( vec3(0,0,1), i ) ;

			mesh.slice_mesh(p, s);
			
			p++;
			s++;


			slice_counter++;
		}	
				
	}
}
