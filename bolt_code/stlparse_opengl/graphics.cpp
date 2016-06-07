#include <iostream>
#include <cassert>
#include <vector>
#include <fstream>
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

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);


	window = glfwCreateWindow(1440, 900, "Slice Viewer", NULL, NULL);
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

int showSlice(string filename, string extension, int counter, GLfloat &x_scale, GLfloat &y_scale, GLfloat &z_scale) {

	vector<glm::vec3> vertices ;

	filename = filename+to_string(counter)+extension;
	FILE* file = fopen(filename.c_str(),"r");
	if(!file) {
			cout<<"\n(Diagnostic Msg) Couldn't Open File";
			return 1;
	}
	
	while(!feof(file)) {
	glm::vec3 temp_vertex;

	fscanf(file, "%f %f %f", &temp_vertex.x, &temp_vertex.y, &temp_vertex.z);
	
	vertices.push_back(temp_vertex);
	}
	vertexCount=0;

	cout<<"\n(Diagnostic Msg)Printing Vertices:\n";	
	for ( auto it = vertices.begin(); it != vertices.end(); it++) 
	{		
			cout<<(*it).x<<" "<<(*it).y<<" "<<(*it).z<<"\n";
			vertexCount++;
	}
	
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

	glUniformMatrix4fv(tm, 1, GL_FALSE, glm::value_ptr(glm_tm));

	glBufferData(GL_ARRAY_BUFFER, 3*sizeof(GLfloat)*vertices.size(), &vertices[0].x, GL_STATIC_DRAW);

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
