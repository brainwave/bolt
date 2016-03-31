#include<iostream>
#include <vector>
#include <math.h>
using namespace std;


struct vec3{

	float x, y, z;

	float dot (const vec3 &vector) {
		return x*vector.x + y*vector.y + z*vector.z;
	}

	void normalize () {
		 float magnitude = sqrt(x*x+y*y+z*z);
		 x=x/magnitude;
		 y=y/magnitude;
		 z=z/magnitude;
	}
		
};

struct plane{
	
	vec3 normal_vec;	
	float distance;

	float distance_to_plane (vec3 point) {
		return normal_vec.dot(point) - distance;
	}

};

int main()
{
	vec3 point1, point2;
	plane plane1;

	cout<<"\n Enter components of vector 1 : ";
	cin>>point1.x>>point1.y>>point1.z;

	cout<<"\n Enter components of vector 2 : ";
	cin>>point2.x>>point2.y>>point2.z;


	cout<<"\n Enter normal vector and distance of plane : ";
	cin>>plane1.normal_vec.x>>plane1.normal_vec.y>>plane1.normal_vec.z>>plane1.distance;			
	
	float d1 = plane1.distance_to_plane(point1);
	float d2 = plane1.distance_to_plane(point2);

	cout<<"\n"<<d1*d2;

	return 0;
}
