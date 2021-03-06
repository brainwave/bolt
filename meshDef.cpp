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
		
		FILE *file;
	
		file = fopen(filename, "r");
		//Using C style file IO over C++ style streams, as streams cause data corruption.
		//May be fixable, need to check advantages of both approaches
	
		if(file==NULL)
			throw Exception("FILE_NOT_FOUND");
		
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

void stlMesh::getMinMax() {

	for ( auto meshIterator = mesh.begin(); meshIterator != mesh.end(); meshIterator++ ) 
		for ( int i = 0; i < 3; i++ ) {
			
			min_z = ( min_z > meshIterator -> vertex[i].z ) ? meshIterator -> vertex[i].z : min_z;
			max_z = ( max_z < meshIterator -> vertex[i].z ) ? meshIterator -> vertex[i].z : max_z;

			min_x = ( min_x > meshIterator -> vertex[i].x ) ? meshIterator -> vertex[i].x : min_x;
			max_x = ( max_x < meshIterator -> vertex[i].x ) ? meshIterator -> vertex[i].x : max_x;

			min_y = ( min_y > meshIterator -> vertex[i].y ) ? meshIterator -> vertex[i].y : min_y;
			max_y = ( max_y < meshIterator -> vertex[i].y ) ? meshIterator -> vertex[i].y : max_y;

	}

}

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
/*
float sign (vec3 p1, vec3 p2, vec3 p3)
{
    return (p1.x - p3.x) * (p2.y - p3.y) - (p2.x - p3.x) * (p1.y - p3.y);
}*/
float area (vec3 p1, vec3 p2, vec3 p3)
{
   return abs((p1.x*(p2.y-p3.y) + p2.x*(p3.y-p1.y)+ p3.x*(p1.y-p2.y))/2.0);
}

/*
// Check if a given point lies within a triangle (2D)
bool enclosed(vec3 point, triangle *t) {

	//Ignore vertical faces
	if(t->vertex[0].x==t->vertex[1].x && t->vertex[0].x==t->vertex[2].x && t->vertex[1].x==t->vertex[2].x || 
	t->vertex[0].y==t->vertex[1].y && t->vertex[0].y==t->vertex[2].y && t->vertex[1].y==t->vertex[2].y )
		return false;

	//Ignore faces close to the edge

	float a = area(t->vertex[0], t->vertex[1], t->vertex[2]);
	float a1 = area(point, t->vertex[1], t->vertex[2]);
	float a2 = area(t->vertex[0], point, t->vertex[2]);
	float a3 = area(t->vertex[0], t->vertex[1], point);
	return (a==a1+a2+a3);
}
	triangle temp = *t;
	//Project triangle to 2D
	for(int i=0; i<3; i++)
		temp.vertex[i].z=0;
	bool b1, b2, b3;
	b1 = sign(point, temp.vertex[0], temp.vertex[1]) < 0.0f;
	b2 = sign(point, temp.vertex[1], temp.vertex[2]) < 0.0f;
	b3 = sign(point, temp.vertex[2], temp.vertex[0]) < 0.0f;
	return ((b1 == b2) && (b2 == b3));
*/

//Substitute a point p onto a line ab to see which side it lies on
float equality(vec3 p, vec3 a, vec3 b) {
	float m = (b.y-a.y)/(b.x-a.x);
	return a.y-p.y - m*(a.x-p.x);
}
// Check if a given point lies within a triangle (2D)
bool enclosed(vec3 point, triangle *t) {
	vec3 centroid;
	centroid.x = (t->vertex[0].x + t->vertex[1].x + t->vertex[2].x)/3.0;
	centroid.y = (t->vertex[0].y + t->vertex[1].y + t->vertex[2].y)/3.0;

	int sameSide = 0;
	for(int i=0; i<3; i++) {
		float e1 = equality(centroid, t->vertex[i%3], t->vertex[(i+1)%3]);
		float e2 = equality(point, t->vertex[i%3], t->vertex[(i+1)%3]);
		if(e1*e2>0 || e2==0)
			sameSide++;
		
		//Ignore points at vertices of the triangle - in the RARE case that this ever happens
		if(point.x == t->vertex[i].x && point.y == t->vertex[i].y && point.z == t->vertex[i].z)
		{	cout<<"\nAt vertex\n"; return false;	}
	}

	//Ignore vertical faces
//	if(t->vertex[0].x==t->vertex[1].x && t->vertex[0].x==t->vertex[2].x && t->vertex[1].x==t->vertex[2].x || 
//	t->vertex[0].y==t->vertex[1].y && t->vertex[0].y==t->vertex[2].y && t->vertex[1].y==t->vertex[2].y ) 
	if(t->normal.z == 0)	
		return false;


	return (sameSide == 3);
}

float maxZ(float a, float b, float c) {
	float max_ab = std::max(a,b);
	return std::max(max_ab, c);
}

float findZ(vec3 point, triangle *t) {
	float a,b,c,d;
	a = t->normal.x; b = t->normal.y; c = t->normal.z;

	float x,y,z,x1,y1,z1;	
	x1 = t->vertex[0].x; y1 = t->vertex[0].y; z1 = t->vertex[0].z;
	x = point.x; y = point.y;

	d = a*x1 + b*y1 + c*z1;
	z = (d-(a*x+b*y))/c;

}

void stlMesh::boundBox(int x_interval, int y_interval)
{
	//Generate a series of points in the box to potentially draw supports from

	vector<float> z_list;
	z_list.push_back(0);

	//If shape falls below z=0, push it to z=0
	if(min_z < 0) 
		for(auto t = mesh.begin(); t != mesh.end(); t++) 
			for(int i=0; i<3; i++)
				t->vertex[i].z += abs(min_z);

	for(int i=min_x; i<=max_x; i+=x_interval) {
		for(int j=min_y; j<=max_y; j+=y_interval) {
			bool xyFlag = false;
			vec3 point; point.x = i; point.y = j;
			for(auto t = mesh.begin(); t != mesh.end(); t++) {		//need to find smaller set of triangles
				if(enclosed(point,&*t)) {
					//float z = maxZ(t->vertex[0].z,t->vertex[1].z,t->vertex[2].z); 
					float z = findZ(point, &*t); 
					if(find(z_list.begin(), z_list.end(), z) != z_list.end() && z!=0) //second case fixes floating shapes and on-ground shapes
						cout<<"Double Z\n";
					else 
						z_list.push_back(z);
					xyFlag=true;
				}
			}
			//Sort z-list in ascending order and link with the xy point
			if(xyFlag == true) {
				sort(z_list.begin(), z_list.end());
				supports.push_back(support(point,z_list));
			}
			z_list.clear();
			z_list.push_back(0);
				
		}	
	} 
}


#endif
