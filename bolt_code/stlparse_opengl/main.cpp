#include "meshDef.hpp"

int main ( int argc, char *argv[] ) {

	cout<<"\nProgram Starts, file name is : ";
	cout<<argv[1];

	triangleMesh mesh;

	if ( mesh.readStlFile(argv[1]) )
		cout << "\nProgram Failed" ;
	
	else
		mesh.display_all_elements();
}
