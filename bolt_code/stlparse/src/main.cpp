#include "read_stl.h"

int main(int argc, char *argv[]){

	cout<<"\nProgram Starts, file name is : ";
	cout<<argv[1];
	
	triangleMesh mesh;

	if(readStlFile(argv[1],  &mesh))
		cout<<"\nProgram Failed";
	else
		cout<<"\nProgram Sucess";

}
