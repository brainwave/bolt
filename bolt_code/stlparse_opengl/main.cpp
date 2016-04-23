#include "meshDef.cpp"

int main ( int argc, char *argv[] ) {

	cout<<"\nProgram Starts, file name is : ";
	cout<<argv[1];

	stlMesh mesh;

	if ( mesh.readStlFile(argv[1]) )
		cout << "\nProgram Failed" ;
	
	else
		mesh.set_min_max_var_z();
		mesh.slice_mesh();
}
