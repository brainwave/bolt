#ifndef MESHDEF_CPP
#define MESHDEF_CPP

#include "meshDef.hpp"
#include <fstream>
#include <time.h>
#include <algorithm>

vector<vec3> supportPoints;

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
					 s->boundary.push_back( linesegment(intersections[1], intersections[0]));		
				
			p++;
			s++;
			sliceCounter++;
			
 		}
	}
}

float sign (vec3 p1, vec3 p2, vec3 p3)
{
    return (p1.x - p3.x) * (p2.y - p3.y) - (p2.x - p3.x) * (p1.y - p3.y);
}

// Check if a given point lies within a triangle (2D)
bool enclosed(vec3 point, triangle *t)
{
	bool b1, b2, b3;
	
	b1 = sign(point, t->vertex[0], t->vertex[1]) < 0.0f;
	b2 = sign(point, t->vertex[1], t->vertex[2]) < 0.0f;
	b3 = sign(point, t->vertex[2], t->vertex[0]) < 0.0f;

	return ((b1 == b2) && (b2 == b3));
}
void stlMesh::boundBox()
{
	vector<triangle> intersections;
	vec3 corners[4];
	corners[0].x = getMinX(); 	corners[0].y = getMinY();
	corners[1].x = getMinX(); 	corners[1].y = getMaxY();
	corners[2].x = getMaxX(); 	corners[2].y = getMaxY();
	corners[3].x = getMaxX();	corners[3].y = getMinY();

	//Generate a series of points in the box to potentially draw supports from
	vector<vec3> points;   
	float x_interval = abs( (max_x - min_x)/10 );
	float y_interval = abs( (max_y - min_y)/10 );
	
	cout<<"\nIntervals:"<<x_interval<<" "<<y_interval<<endl;

	for(int i=min_x; i<max_x; i+=x_interval) {
		for(int j=min_y; j<max_y; j+=y_interval) {
			vec3 point; point.x = i; point.y = j;
			points.push_back(point);
		}	
	} 

	//Iterate through all triangles and find points of intersection for all the points 
	for(auto p = points.begin(); p != points.end(); p++) {
		for(auto t = mesh.begin(); t != mesh.end(); t++) {		//need to find smaller set of triangles
			if(enclosed(*p, &*t)) { 							//&*t converts iterator of triangles to a pointer to a triangle
				intersections.push_back(*t);
				cout<<"Support generated at "<<p->x<<","<<p->y<<","<<p->z<<"\n";   
				projectTriangle(&*t);
			}
		}
	}
}

void stlMesh::projectTriangle(triangle *triA)
{
	vec3 vertex[3];
	triangle triB;

        for(int i=0; i<3; i++) {
                vertex[i].x = triA->vertex[i].x;
                vertex[i].y = triA->vertex[i].y;
                vertex[i].z = getMinZ();
        }
        triB.setVertices(vertex[0], vertex[1], vertex[2]);
	supportMesh.push_back(triB);



        //Manually connecting vertices
        triangle dummy;
//      dummy.setVertices(triA.vertex[0], triB.vertex[0], triA.vertex[1]);      supportMesh.push_back(dummy);
        dummy.setVertices(triA->vertex[1], triB.vertex[1], triA->vertex[2]);    supportMesh.push_back(dummy);
        dummy.setVertices(triA->vertex[2], triB.vertex[2], triA->vertex[0]);    supportMesh.push_back(dummy);

//      dummy.setVertices(triB.vertex[1], triA.vertex[1], triB.vertex[0]);      supportMesh.push_back(dummy);
        dummy.setVertices(triB.vertex[0], triA->vertex[0], triB.vertex[2]);     supportMesh.push_back(dummy);
        dummy.setVertices(triB.vertex[2], triA->vertex[2], triB.vertex[1]);     supportMesh.push_back(dummy);

}

/*
//if contact area between 2 adjacent slices is small, generate support
void stlMesh::supportGenerator( slice *sstart, int arr_len, int skipAmnt )
{
//  cout<<"\nBEFORE: "<<s->slice.size()<<endl;
  slice *top, *next;	//s is current, next is slice below it
  top = sstart + arr_len-2;
  vector<linesegment> points;
  int z;
  //iterate through all slices
  //for(int i=arr_len; i >=0; i--) {	cout<<"\nLOOP STARTED\n";
  for(int i=0; i<arr_len-1; i++, top--) {

	next = top-1;
	if(enclosed(next, top)) {			//if bottom layer is smaller than top layer
		//iterate through each edge in the top slice
	        for(int j=0; j < top->slice.size(); j++) {         
		        //get the endpoints of each edge 
			int x1,y1, x2, y2, l=20;
			x1 = top->slice[j].startpoint.x; y1 = top->slice[j].startpoint.y;	
			x2 = top->slice[j].endpoint.x; y2 = top->slice[j].endpoint.y;	
			vec3 point[4]; 
			point[0].x = x-l; point[0].y = y-l; point[1].x = x+l; point[1].y = y-l;
			point[2].x = x-l; point[2].y = y+l; point[3].x = x+l; point[3].y = y+l;
			//tiny square of length 10 mm centred at x, y
			for(int k=0; k<4; k++)
				points.push_back(linesegment(point[k], point[k+1]));
			points.push_back(linesegment(point[3], point[0]));

			vec3 midpoint; midpoint.x=(x1+x2)/2; midpoint.y=(y2-y1)/2;
			vec3 point; point.x = midpoint.x+l; point.y = midpoint.y;
			points.push_back(linesegment(midpoint, point));

		}

		//add the linesegments in point to the array of slices for all z till 0
		z = top->slice[0].endpoint.z;	//current z value
		slice *temp = top;
		cout<<"\nZ: "<<z<<endl;
		while(z>=0 || temp!=sstart)
	        {
		        cout<<"Added support point at z = "<<z<<endl;
		 	for(int p = 0; p < points.size(); p++)
				temp->slice.push_back(points[i]); 
			z--;
			temp--;
 		}
	}
	i += skipAmnt;
	top -= skipAmnt;
	if( i >= arr_len) break;
  	//break; //only generate one support
  }

//  cout<<"\nAFTER: "<<s->slice.size()<<endl;

}
*/
#endif
