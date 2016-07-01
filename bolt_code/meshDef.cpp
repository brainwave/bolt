#ifndef MESHDEF_CPP
#define MESHDEF_CPP

#include "meshDef.hpp"
#include <fstream>
#include <time.h>
#include <algorithm>
float plane::distanceFromPoint (vec3 point) {
	normal = glm::normalize (normal);
	return dot(point, normal) - distance;
}	

int stlMesh::readStlFile ( const char *filename ) {
			
	bool isASCII=false; //false -binary, true -ASCII

		triangle t;
		vec3 vertex[4];

		//detect if the file is ASCII or not
		int parser;
		
		FILE *file = fopen(filename, "r");
			//Using C style file IO over C++ style streams, as streams cause data corruption.
			//May be fixable, need to check advantages of both approaches
			
		do{
			parser=fgetc(file);
		}while((parser!=EOF) && parser<=127 );
			//Check if file is ASCII. If true, every character read from file should be <=127
			//Need to check for robustness of this approach.
			
		fclose(file);

		if(parser==EOF)
			isASCII=true;

		if(!isASCII) {
			//block to read binary stl file
			
				char modelName[80], discarder[2];
				long long facetNo;

				cout<<"\nFile is not ASCII, using binary format";

				file = fopen(filename, "rb");
				
				if(!file) {
					cout<<"\nfile opening failed,  with error ";
					perror("Error");
					return 1;
				}
				
				fread(modelName, 80, 1, file);
				fread((void *)&facetNo, 4, 1, file);
				
				cout<<"\nNo. Of Triangles is : "<<facetNo;

				while(!feof(file)){
					for(int i=0;i<4;i++){

						fread((void *)&vertex[i].x, 4, 1 , file);
						fread((void *)&vertex[i].y, 4, 1 , file);
						fread((void *)&vertex[i].z, 4, 1 , file);
				}
						
				vertex>>t;

					mesh.push_back(t);

					fread((void *)discarder, 2, 1, file);
						//discard 2 bytes end of each loop
				}
			return 0;
		}

		else{
		//block for ASCII
		
		cout<<"\nUsing ASCII format";
		
		file = fopen(filename, "r");
		
		char header[6], modelName[256];
		fscanf(file,"%s",header);
			//Again, choosing C style formatted read of C++ style stream.
			//Check advantages of each approach, change if beneficial.
			
		if(!strcmp(header,"solid")){

			fgets(modelName,255,file);
			cout<<"\nModel Name is : "<<modelName;

			while (!feof(file)){

				char string0[11], string1[11];
				
				fscanf(file,"%s", string0);

				if(!strcmp(string0,"endsolid")) {

					cout<<"\nEnd of file reached";
					break;
				}

				fscanf(file,"%s", string1);

				if( !strcmp(string0, "facet") && !strcmp(string1,"normal")) {

					fscanf(file, "%f", &vertex[0].x);
					fscanf(file, "%f", &vertex[0].y);
					fscanf(file, "%f", &vertex[0].z);

					fscanf(file, "%s", string0);
					fscanf(file, "%s", string1);
						
					if( !strcmp(string0, "outer") && !strcmp(string1,"loop")){
					

						for(int i=1; i<=3; i++){

								fscanf(file, "%s", string0);
								
								if( !strcmp(string0, "vertex")){

									fscanf(file, "%f", &vertex[i].x);
									fscanf(file, "%f", &vertex[i].y);
									fscanf(file, "%f", &vertex[i].z);			

								}			
						}
						fscanf(file, "%s", string0);
						if( strcmp(string0, "endloop"))
						{
							cout<<"\nERROR - Expected 'endloop', found "<<string0<<" instead";
							return 1;
						}

						fscanf(file, "%s", string0);

						if( strcmp(string0, "endfacet")) {

							cout<<"ERROR - expected 'endfacet', found "<<string1<<" instead";
							return 1;
						}
					}
				}

				vertex>>t;
				mesh.push_back(t);

			}
		}
	
		else
			cout<<"\nHeader Mismatch, incorrect ASCII file";

		return 0;
	}
		return 1;
}

float stlMesh::getMinX(){ return min_x; }
float stlMesh::getMaxX(){ return max_x; }
float stlMesh::getMinY(){ return min_y; }
float stlMesh::getMaxY(){ return max_y; }
float stlMesh::getMinZ(){ return min_z; }
float stlMesh::getMaxZ(){ return max_z; }

void stlMesh::recenter() {

	float xrange, yrange, zrange;

	for ( auto meshIterator = mesh.begin(); meshIterator != mesh.end(); meshIterator++ ) 
		for ( int i = 0; i < 3; i++ ) {
			
			min_z = ( min_z > meshIterator -> vertex[i].z ) ? meshIterator -> vertex[i].z : min_z;
			max_z = ( max_z < meshIterator -> vertex[i].z ) ? meshIterator -> vertex[i].z : max_z;

			min_x = ( min_x > meshIterator -> vertex[i].x ) ? meshIterator -> vertex[i].x : min_x;
			max_x = ( max_x < meshIterator -> vertex[i].x ) ? meshIterator -> vertex[i].x : max_x;

			min_y = ( min_y > meshIterator -> vertex[i].y ) ? meshIterator -> vertex[i].y : min_y;
			max_y = ( max_y < meshIterator -> vertex[i].y ) ? meshIterator -> vertex[i].y : max_y;

	}

	xrange = max_x - min_x; yrange = max_y - min_y; zrange = max_z - min_z;

	float xcenter = min_x + xrange/2, ycenter = min_y + yrange/2, zcenter = min_z + zrange/2;

		for ( auto meshIterator = mesh.begin(); meshIterator != mesh.end(); meshIterator++) {
			for( int i =0; i <3; i++ ) {

				meshIterator->vertex[i].x -= xcenter;
				meshIterator->vertex[i].y -= ycenter;
				meshIterator->vertex[i].z -= zcenter;
			}
		}

	max_x -= xcenter; min_x -= xcenter;
	max_y -= ycenter; min_y -= ycenter;
	max_z -= zcenter; min_z -= zcenter;	
}

// For all triangles in the mesh, consider only the relevant planes and push the intersection line segments into appropriate slice
void stlMesh::sliceMesh(plane *pstart, slice *sstart, float sliceSize, int arr_len){

	plane *p;
	slice *s;

	int sliceCounter;

	float triangle_min_z, triangle_max_z;

	for(auto t = mesh.begin(); t!=mesh.end(); t++){
		
		p=pstart;
		s=sstart;
		sliceCounter=0;
		
		triangle_min_z=t->vertex[0].z;
		triangle_max_z=t->vertex[0].z;
	
		for (int j=1; j<3; j++)	{

			if(t->vertex[j].z<triangle_min_z) triangle_min_z=t->vertex[j].z;
			if(t->vertex[j].z>triangle_max_z) triangle_max_z=t->vertex[j].z;
		}

		// move to the bottom-most plane that cuts the triangle
		while(p->distance<=triangle_min_z && sliceCounter<arr_len){

				p++;
				s++;
				sliceCounter++;

 		}

		// possibly overshot the min-z value, so move back one plane
		if(p!=pstart && sliceCounter>0){
			p--;
			s--;
			sliceCounter--;

		}

		// move through planes till the plane is above the triangle
		while(p->distance<=triangle_max_z && sliceCounter<arr_len && sliceCounter >=0){

			vector<vec3> intersections;

			for (int i=0; i < 3; i++) {

				float dp1 = p->distanceFromPoint(t->vertex[i]);
				float dp2 = p->distanceFromPoint(t->vertex[(i+1)%3]);

				if( dp1*dp2 < 0){ 
					intersections.push_back (t->vertex[i] + ((t->vertex[(i+1)%3] - t->vertex[i]) * 
							 (dp1/(dp1-dp2))) );
				}	

			}
			
			if(intersections.size()==2) 
					 s->slice.push_back( linesegment(intersections[1], intersections[0]));		
				
			p++;
			s++;
			sliceCounter++;
			
 		}
	}
}

void slice::fillSlice() {

	if(isFilled)
		return;

	vector <glm::vec3> vertices, intersections;
	int boundaryVertexCount = 0;

	for ( auto it = slice.begin(); it != slice.end(); it++ ) {
	
		vertices.push_back(it->startpoint);
		vertices.push_back(it->endpoint);
		boundaryVertexCount+=2;
	}

	if(boundaryVertexCount == 0){
	
		isFilled = true;
		return;
	}

	float yMin = vertices[0].y, yMax = vertices[0].y, xMin = vertices[0].x, xMax = vertices[0].x, step;
	float zCoord = vertices[0].z;	

	bool first = true;
	bool odd = true;

	
	// find ymax and ymin
	for(auto it = vertices.begin(); it!=vertices.end(); it++){

		yMin = (it->y < yMin) ? it->y : yMin; yMax = (it->y > yMax) ? it->y : yMax;
		xMin = (it->x < xMin) ? it->x : xMin; xMax = (it->x > xMax) ? it->x : xMax;
	}

	step =(yMax - yMin)/600;
	
	// EDGE TABLE ALGORITHM

	glm::vec3 vertex1, vertex2;
	
	vector <EdgeTableRecord> global, active;

	// creation of the global edge table
	for(auto it = vertices.begin(); it!=vertices.end(); it++){
	
		if(first){
		
			vertex1 = *it;
			
			first = false;
		}

		else{
			
			vertex2 = *it;
	
			first = true;			

			float y_min, y_max, x_y_min,x_y_max, inverse_slope;
			
			if(vertex1.y<vertex2.y){
	
				y_min = vertex1.y;
				y_max = vertex2.y;
				x_y_min = vertex1.x;
				x_y_max = vertex2.x;
			}
			else if(vertex1.y == vertex2.y){
			
				y_min = y_max = vertex1.y;
				if(vertex1.x<vertex2.x){
					
					x_y_min = vertex1.x;
					x_y_max = vertex2.x;
				}
				else{
			
					x_y_min = vertex2.x;
					x_y_max = vertex1.x;
				}
			}
			else{
	
				y_min = vertex2.y;
				y_max = vertex1.y;
				x_y_min = vertex2.x;
				x_y_max = vertex1.x;
			}


			if(y_max == y_min)
				inverse_slope = numeric_limits<float>::max();
			else
				inverse_slope = (vertex1.x - vertex2.x)/(vertex1.y - vertex2.y);
			
			EdgeTableRecord edge;

			edge.y_min = y_min;
			edge.y_max = y_max;
			edge.x_y_min = x_y_min;
			edge.x_y_max = x_y_max;
			edge.inverse_slope = inverse_slope;
			edge.checked = false;
				
			global.push_back(edge);
		}
	}

	// sort based on y_min, then x_y_min, then y_max	
	sort(global.begin(),global.end(),globalEdgeTableComparision);

	for(float y = yMin; y<=yMax; y+=step){

		// remove active edges with y_max < y
		for(auto it = active.begin(); it!=active.end(); ){

			if(it->y_max<y){
				active.erase(it);
			}
			else{
				it++;
			}
		}
	
		// add edges with y_min = y to the active edge list
		for(auto it = global.begin(); it!=global.end() && it->y_min<=y; ) {

			active.push_back(*it);
			global.erase(it);
		}
	
		// sort active edge table records in ascending order of x_y_min
		sort(active.begin(),active.end(),activeEdgeTableComparision);

		odd = true;

		intersections.clear();

		for(auto it = active.begin(); it!=active.end(); it++){
	
			vertex1.x = it->x_y_min;
			vertex1.y = y;
			vertex1.z = zCoord;
				
			if(y>it->y_min && y<it->y_max) {
	
				intersections.push_back(vertex1);
			}
			else if(y==it->y_min && y!=it->y_max){	
			
				for(auto iter = active.begin(); iter!=active.end(); iter++){
	
					if(iter->y_max==y){
						
						if(!(find(intersections.begin(),intersections.end(),vertex1)!=intersections.end())){
	
							intersections.push_back(vertex1);	
						}
					}
				}
			}
			else if(y==it->y_max && y!=it->y_min){
	
				for(auto iter = active.begin(); iter!=active.end(); iter++){		
			
					if(iter->y_min==y){
					
						if(!(find(intersections.begin(),intersections.end(),vertex1)!=intersections.end()))
	
							intersections.push_back(vertex1);
					}
				}
			}

		}
		
		for(auto it = active.begin(); it!=active.end(); it++){	
			
			if(it->y_max != it->y_min)	
				it->x_y_min = it->x_y_min + step*it->inverse_slope;
		}

		if(intersections.size() % 2 == 1)
			intersections.erase(intersections.end() - 1);

		for(auto x = intersections.begin(); x!=intersections.end();x++){
		
			vertices.push_back(*x);
		}		

	}

	for(auto it = vertices.begin(); it!=vertices.end(); it+=2){
	
		slice.push_back(linesegment(*it,*(it+1)));
	}

	isFilled = true;
}

#endif
