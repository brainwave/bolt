#include <iostream>
#include <cassert>
#include <vector>
#include <fstream>
#include <algorithm>
using namespace std;

#include <stdio.h>
#include <unistd.h>

#include <GL/glew.h>

#include <GLFW/glfw3.h>

#include "glm/glm.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "glm/gtc/matrix_transform.hpp"


// Shader sources

const GLchar* vshader =
    "#version 150 \n"
    "in vec3 position;"
    "uniform mat4 transform;"
	"void main()"
    "{"
    "    gl_Position  = transform*vec4(position, 1.0);"
    "}";

const GLchar* fshader =
    "#version 150 \n"
    "out vec4 fragColor;"
    "void main()"
    "{"
    "    fragColor = vec4(0.0, 0.0, 1.0, 1.0);"
    "}";

//Global Variables
GLuint vao, vbo,shaderProgram ;
GLfloat  xscale, yscale, zscale;
int showSlice(string filename, string extension, int counter);
int max_slice_no=0,cur_slice_no=0, vertexCount=0;
int width=0, height=0;

float minimum (GLfloat x, GLfloat y, GLfloat z) {
	float min = x; /* assume x is the largest */
	if (y < min) 
		min = y;
	if (z < min ) 
		min = z;

	return min ; /* max is the largest value */
} 

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
			glfwSetWindowShouldClose(window,GL_TRUE);
	
//	if (key == GLFW_KEY_SPACE && action == GLFW_PRESS && cur_slice_no<max_slice_no)

}

GLFWwindow* glInit(int slicecounter, const GLfloat xshift, const GLfloat yshift, const GLfloat zshift) {
	//store globally relevant info
	max_slice_no=slicecounter;

	xscale = 1/(2*xshift);
	yscale = 1/(2*yshift);
	zscale = 1/(2*zshift);

	GLFWwindow* window;
	if (!glfwInit())
	    exit(EXIT_FAILURE);

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);


	window = glfwCreateWindow(800, 600, "Slice Viewer", NULL, NULL);
if (!window) {
	    glfwTerminate();
	    exit(EXIT_FAILURE);
	}

	glfwMakeContextCurrent(window);
	glfwSetKeyCallback(window, key_callback);
	glfwSwapInterval(1);

	glfwGetFramebufferSize (window, &width, &height);

	glewExperimental = GL_TRUE;
	if(glewInit()!=GLEW_OK) {
		cout<<"\n(Diagnostic Msg) Error: Couldn't initiazlie glew.";
		exit(EXIT_FAILURE);
	}

	glGetError();
	
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	glEnableVertexAttribArray(0);
	
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);

	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource (vertexShader, 1, &vshader, NULL);
	glCompileShader(vertexShader);

	//check vertex shader compilation
	GLint status=0;
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &status);
	if(status==GL_FALSE) {
		GLint logLen=0;
		glGetShaderiv(vertexShader, GL_INFO_LOG_LENGTH, &logLen);
		GLchar* logStr = new GLchar[logLen];
		glGetShaderInfoLog(vertexShader, logLen, 0, logStr);

		cout<<"\nVertex Shader Error "<<logStr;
	}
	
	//frag shader
	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);	
	glShaderSource (fragmentShader, 1, &fshader, NULL);	
	glCompileShader(fragmentShader);
	status=0;
	//check fragshader compilation
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &status);
	if(status==GL_FALSE) {
		GLint logLen=0;
		glGetShaderiv(fragmentShader, GL_INFO_LOG_LENGTH, &logLen);
		GLchar* logStr = new GLchar[logLen];
		glGetShaderInfoLog(fragmentShader, logLen, 0, logStr);

		cout<<"\nFragment Shader Error "<<logStr;
	}
	
	//shader program
	shaderProgram = glCreateProgram();
	
	glAttachShader(shaderProgram, fragmentShader);
	glAttachShader( shaderProgram, vertexShader);

	glLinkProgram(shaderProgram);

	status=0;
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &status);
	
	//check linking
	if(status==GL_FALSE) {
		GLint logLen=0;
		glGetProgramiv(shaderProgram, GL_INFO_LOG_LENGTH, &logLen);
		cout<<"\nLog Length: "<<logLen<<"\n";

		GLchar* logStr = new GLchar[logLen];
		glGetProgramInfoLog(shaderProgram, logLen, 0, logStr);

		cout<<"\nProgram Linking Error "<<logStr;
	}
	
	glUseProgram(shaderProgram);
	
	return window;
}

int boundaryVertexCount=0;

bool xCoordinateComparision(const glm::vec3 &a, const glm::vec3 &b){

	return a.x<b.x;
}



vector <glm::vec3>  lineFill(vector <glm::vec3> vertices){

	if(boundaryVertexCount == 0)
		return vertices;

	vector <glm::vec3> addedVertices,intersections;

	float ymin = vertices[0].y;
	float ymax = vertices[0].y;
//	float step = 0.0010f;		
	float step = 0.01f;
//	float step = 0.0225f;	

	bool first = true;
	bool odd = true;

	
	// find ymax and ymin
	for(auto it = vertices.begin(); it!=vertices.end(); it++){

		ymin = (it->y<ymin)?it->y:ymin;
		ymax = (it->y>ymax)?it->y:ymax;
	}


	
	for (float i=ymin; i<=ymax; i+=step){

		glm::vec3 vertex1, vertex2;
		glm::vec3 intersectionPoint;
			

		float dp1, dp2;

		odd = true;
		first = true;

		intersections.clear();
		
		for(auto it = vertices.begin();it != vertices.end(); it++){
	
			if(first){

				vertex1 = *it;
				dp1 = vertex1.y - i;
				first = false;
			}
						
			else{

				first = true;
				vertex2 = *it;
				dp2 = vertex2.y - i;
			
				
				if( dp1*dp2 < 0){
					
					intersectionPoint = vertex1 + (vertex2 - vertex1)*(dp1/(dp1-dp2));
					intersections.push_back(intersectionPoint);
				}

				else if (dp1 == 0){
					
					printf("\n DP1 is 0");
					
					vector<glm::vec3> endPoints;

					int counter = 0;

					for (auto iter = vertices.begin(); iter!=vertices.end(); iter++, counter++){
						
						if(iter->x==vertex1.x && iter->y==vertex1.y){
						
							if(counter%2==0)								
								endPoints.push_back(*(iter+1));
							else
								endPoints.push_back(*(iter-1));
						}
					}
						
					if(endPoints.size() == 2){ // 2 point case
					
						if((endPoints[0].y - vertex1.y > 0 && endPoints[1].y - vertex1.y > 0) 
							|| (endPoints[0].y - vertex1.y < 0 && endPoints[1].y - vertex1.y < 0) )
							intersections.push_back(vertex1);
							
					} 			
	
				}
				else if (dp2 == 0){

					printf("\n DP2 is 0");

					vector<glm::vec3> endPoints;

					int counter = 0;

					for (auto iter = vertices.begin(); iter!=vertices.end(); iter++, counter++){
						
						if(iter->x==vertex2.x && iter->y==vertex2.y){
						
							if(counter%2==0)								
								endPoints.push_back(*(iter+1));
							else
								endPoints.push_back(*(iter-1));
						}
					}
						
					if(endPoints.size() == 2){ // 2 point case
					
						if((endPoints[0].y - vertex2.y > 0 && endPoints[1].y - vertex2.y > 0) 
							|| (endPoints[0].y - vertex2.y < 0 && endPoints[1].y - vertex2.y < 0) )
							intersections.push_back(vertex2);
							
					} 
				}
			}

		}

		// sort intersection points  and push them into addedVertices
                sort(intersections.begin(), intersections.end(),xCoordinateComparision);

                for(auto x = intersections.begin(); x!=intersections.end(); x++){

			addedVertices.push_back(*x);
                }
	}

	// push all added vertices into vertices
	for(auto it = addedVertices.begin(); it!=addedVertices.end(); it++){

		vertices.push_back(*it);
	}
	
	return vertices;
}


int showSlice(string filename, string extension, int counter, GLfloat &x_scale, GLfloat &y_scale, GLfloat &z_scale) {

	vector<glm::vec3> vertices ;

	filename = filename+to_string(counter)+extension;
	FILE* file = fopen(filename.c_str(),"r");
	if(!file) {
			cout<<"\n(Diagnostic Msg) Couldn't Open File";
			return 1;
	}

	boundaryVertexCount = 0;
	vertexCount = 0;	
	glm::vec3 temp_vertex;
	
	while(fscanf(file,"%f %f %f", &temp_vertex.x, &temp_vertex.y, &temp_vertex.z)!=EOF) {
	
		vertices.push_back(temp_vertex);
		
		boundaryVertexCount++;
	}

	printf("\n Initial read count = %d",boundaryVertexCount);


	fclose(file);

	vertices = lineFill(vertices);

//	cout<<"\n(Diagnostic Msg)Printing Vertices:\n";	

	GLfloat aspectratio = width/(float)height;
	
	yscale = minimum(xscale,yscale,zscale);
	cout<<"\nY scale currently is: "<<yscale;
	xscale = yscale;
	zscale = yscale;
	yscale*= aspectratio;	

	x_scale=xscale;
	y_scale=yscale;
	z_scale=zscale;

	glm::mat4 glm_tm = glm::scale(glm::mat4(1.0),glm::vec3(xscale,yscale,zscale));

//	glm::mat4 glm_tm = glm::scale(glm::mat4(1.0),glm::vec3(1,aspectratio,1));
	//get transformation matrix location
	GLint tm = glGetUniformLocation(shaderProgram, "transform");
	if(tm ==-1 ) {
			cout<<"\nError, couldn't bind transform ";
			return 1; 
	}



	vertexCount = 0;
	for ( auto it = vertices.begin(); it != vertices.end(); it++) 
	{		
//			cout<<(*it).x<<" "<<(*it).y<<" "<<(*it).z<<"\n";
			vertexCount++;
	}
	
	glUniformMatrix4fv(tm, 1, GL_FALSE, glm::value_ptr(glm_tm));

	glBufferData(GL_ARRAY_BUFFER, 3*sizeof(GLfloat)*vertices.size(), &vertices[0].x, GL_STATIC_DRAW);

//	cin.get();

	return 0;
}

int showWindow(GLFWwindow* window,GLfloat x_scale, GLfloat y_scale, GLfloat z_scale) {
		//vertex shader
	while(!glfwWindowShouldClose(window)) {
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
	glDrawArrays(GL_LINES, 0, vertexCount);

	if(cur_slice_no<max_slice_no)
	showSlice(".slice_",".dat",cur_slice_no++,x_scale, y_scale, z_scale);	

	glfwPollEvents();
	glfwSwapBuffers(window);
	}
	return 0;
}
