#include <Python.h>
#include "logic.hpp"

int logic ( int argc, const char* argv[] ) {

	string fileName, pngDir;
	float sliceSize, thickness;

	int xres, yres;

	bool hollow, support;

	// time calculation
	clock_t startTime = clock();
	clock_t time = startTime;

	if(!checkArguments(argc, argv, fileName, sliceSize, pngDir, xres, yres, hollow, thickness, support))
		return 0;
	
	//ranges, min and max z values, and O(verall)scale_x, y and z
	float xrange, yrange, zrange, min_z, max_z, max_x, min_x, max_y, min_y;

	stlMesh mesh;
	stlMesh supportMesh;

	time = clock();


	// perform hollowing routine if needed	
	if(hollow){
	
		string hollowingCommand = "openscad -o "+pngDir+"/hollow.stl -D 'model=\""+fileName+"\"' -D 'thickness="+to_string(thickness)+"' hollow.scad";

		system(hollowingCommand.c_str());	

		fileName = pngDir+"/hollow.stl";
	}
	
	if(support){
	if ( supportMesh.readStlFile( fileName.c_str())) {
		cout << "\nProgram Failed" ;
		return 1;
	}
	else {
		//writing SCAD file
		writeSCAD(supportMesh, fileName.c_str(), fileName);
	}
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


		cout<<"\nTime spent in slicing "<<(double) (clock() - time) / CLOCKS_PER_SEC;

		initPNG ( xres, yres, pngDir );
		
		boost::threadpool::pool tp(boost::thread::hardware_concurrency());

		time = clock();
		
		// filling and png generation
		for(slice_counter=0;slice_counter<max_slice_no;slice_counter++){
	
	
			tp.schedule(boost::bind(&thread_function, *s, slice_counter, min_x, max_x, min_y, max_y, xres, yres));

	//		s->fillSlice();
	//		generatePNG(*s, slice_counter, min_x, max_x, min_y, max_y);
			s++;
		}
		cout<<"\nTime spent in storing "<<(double) ( clock() - time ) / CLOCKS_PER_SEC;
		cout<<"\nTotal Program Execution Time "<<(double)(clock() - startTime)/CLOCKS_PER_SEC;

	}

	return 0;
}

static PyObject *
main_logic (PyObject *self, PyObject *args) {

/*
	char* fileName, pngDir;
	
	float sliceSize;

	int xres, yres, hollow, thickness, support;
*/
	const char* argv[16]; 

	argv[0] = "Slicer Program";
	argv[1] = "-f";
	argv[3] = "-s";
	argv[5] = "-o";
	argv[7] = "-r";
	argv[10] = "-h";
	argv[12] = "-t";
	argv[14] = "-sg";
	

	if( !PyArg_ParseTuple ( args, "ssssssss", &argv[2], &argv[4], &argv[6], &argv[8], &argv[9], &argv[11], &argv[13], &argv[15] )) {

		return NULL;
	}
	

	int isSuccessful = logic( 16, argv );

	return PyLong_FromLong ( isSuccessful ) ;
}

static PyMethodDef methods[] = {

	{"logic", main_logic, METH_VARARGS, "Run the main logic"},
	{NULL, NULL, 0, NULL}
};

static struct PyModuleDef module = {
	PyModuleDef_HEAD_INIT,
	"logic",
	NULL,
	-1,
	methods
};

PyMODINIT_FUNC PyInit_slicer(void) {
	return PyModule_Create(&module);
}

