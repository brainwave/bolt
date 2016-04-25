#include "graphics.hpp"
#include "libs/glm/glm.hpp"
#include "meshDef.hpp"

static void error_callback(int error, const char* description)
{
    cerr<<"\n(Diagnostic Msg) Error No"<<error;
    cerr<<"\n(Diagnostic Msg) Error Desc"<<description;
}

void testgl() {

	GLfloat data[]= {
		-1.0f, -1.0f, 0.0f,
		1.f, -1.f, 0.f,
		0.0f, 1.f, 0.f
	};

	glfwSetErrorCallback(error_callback);
	
	if(!glfwInit())
		exit(EXIT_FAILURE);

	int major, minor, revision;
	glfwGetVersion(&major, &minor, &revision);

	GLFWwindow* window;
	window = glfwCreateWindow ( 640, 480, "Visualizer", NULL, NULL );

	if (!window) {

	cout<<"\nWindow Creation Failed";
	cin.get();
	glfwTerminate();
	
	exit(EXIT_FAILURE);
	}

	glfwMakeContextCurrent(window);

	glewExperimental=GL_TRUE;
	glewInit();

	glfwSwapInterval(1);
	
	glfwSetInputMode ( window, GLFW_STICKY_KEYS, GL_TRUE );
	
	GLuint vao, vbo;

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);	
	
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(data), data, GL_STATIC_DRAW);
	

	do {

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);

		glDrawArrays(GL_TRIANGLES,0,12);

		glDisableVertexAttribArray(0);
		
		glfwSwapBuffers(window);

		glfwPollEvents();
			
	} while( glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS ); 

	glEnd();

	glfwTerminate();
	exit(EXIT_SUCCESS);

}
