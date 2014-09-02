#include <GL/glew.h> // glew must be included before the main gl libs
#include <GL/freeglut.h> // doing otherwise causes compiler shouting
#include <iostream>
#include <chrono>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp> //Makes passing matrices to shaders easier

#include "ShaderLoader.h"
#include "ModelLoader.h"
#include <vector>


//--Data types
//This object will define the attributes of a vertex(position, color, etc...)
/*struct Vertex
{
    GLfloat position[3];
    GLfloat color[3];
};*/

//--Evil Global variables
//Just for this example!
int w = 640, h = 480;// Window size
GLuint program;// The GLSL program handle
GLuint vbo_geometry;// VBO handle for our geometry
bool isRotate = true;
bool isReverse = false;
bool isReverseOrbit = false;
int numOfVertices = 0;
char *filename;
float zoomScale = -16;
//uniform locations
GLint loc_mvpmat;// Location of the modelviewprojection matrix in the shader

//attribute locations
GLint loc_position;
GLint loc_color;

//transform matrices
glm::mat4 model;//obj->world each object should have its own model matrix
glm::mat4 moon; //moon model
glm::mat4 view;//world->eye
glm::mat4 projection;//eye->clip
glm::mat4 mvp;//premultiplied modelviewprojection

ModelLoader loadModel;

//--GLUT Callbacks
void render();
void update();
void reshape(int n_w, int n_h);
void keyboard(unsigned char key, int x_pos, int y_pos);
void arrowKeyboard(int key, int x, int y);
void mouse(int button, int state, int x, int y);
void menu(int id);




//--Resource management
bool initialize();
void cleanUp();


//--Random time things
float getDT();
void renderString(const char *text);
std::chrono::time_point<std::chrono::high_resolution_clock> t1,t2;


//--Main
int main(int argc, char **argv)
{
    // Initialize glut
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_DEPTH);
    glutInitWindowSize(w, h);

	
	filename = argv[1];
	// create menu with values
	/*glutCreateMenu(menu);
	glutAddMenuEntry("Quit", 1);
	glutAddMenuEntry("Start Rotation", 2);
	glutAddMenuEntry("Stop Rotation", 3);
	*/

    // Name and create the Window
    glutCreateWindow("Matrix Example");

    // Now that the window is created the GL context is fully set up
    // Because of that we can now initialize GLEW to prepare work with shaders
    GLenum status = glewInit();
    if( status != GLEW_OK)
    {
        std::cerr << "[F] GLEW NOT INITIALIZED: ";
        std::cerr << glewGetErrorString(status) << std::endl;
        return -1;
    }

    // Set all of the callbacks to GLUT that we need
    glutDisplayFunc(render);// Called when its time to display
    glutReshapeFunc(reshape);// Called if the window is resized
    glutIdleFunc(update);// Called if there is nothing else to do
    glutKeyboardFunc(keyboard);// Called if there is keyboard input
	glutSpecialFunc(arrowKeyboard);
	glutMouseFunc(mouse);// Called if there is mouse input
	glutAttachMenu(GLUT_RIGHT_BUTTON);

    // Initialize all of our resources(shaders, geometry)
    bool init = initialize();
    if(init)
    {
        t1 = std::chrono::high_resolution_clock::now();
        glutMainLoop();
    }

	

    // Clean up after ourselves
    cleanUp();
    return 0;
}

//--Implementations
void render()
{
    //--Render the scene

    //clear the screen
    glClearColor(0.0, 0.0, 0.2, 1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    //premultiply the matrix for this example
    mvp = projection * view * model;

    //enable the shader program
    glUseProgram(program);

    //upload the matrix to the shader
    glUniformMatrix4fv(loc_mvpmat, 1, GL_FALSE, glm::value_ptr(mvp));

    //set up the Vertex Buffer Object so it can be drawn
    glEnableVertexAttribArray(loc_position);
    glEnableVertexAttribArray(loc_color);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_geometry);
    //set pointers into the vbo for each of the attributes(position and color)
    glVertexAttribPointer( loc_position,//location of attribute
                           3,//number of elements
                           GL_FLOAT,//type
                           GL_FALSE,//normalized?
                           sizeof(Vertex),//stride
                           0);//offset

    glVertexAttribPointer( loc_color,
                           3,
                           GL_FLOAT,
                           GL_FALSE,
                           sizeof(Vertex),
                           (void*)offsetof(Vertex,color));

    glDrawArrays(GL_TRIANGLES, 0, numOfVertices*3);//mode, starting index, count



	/*//moon set up
	mvp = projection * view * moon;
	glUniformMatrix4fv(loc_mvpmat, 1, GL_FALSE, glm::value_ptr(mvp));
    glDrawArrays(GL_TRIANGLES, 0, 36);//mode, starting index, count

	// draw text
	if(isReverse)
	{
		renderString("Planet Rotation: clockwise");
	}
	else
	{
		renderString("Planet Rotation: counter-clockwise");
	}*/
    //clean up
    glDisableVertexAttribArray(loc_position);
    glDisableVertexAttribArray(loc_color);
                      

     
    //swap the buffers
    glutSwapBuffers();
}

void update()
{
    //total time
    static float angle = 0.0, rotAngle = 0.0;
    float dt = getDT();// if you have anything moving, use dt.

   		angle += dt * M_PI/2; //move through 90 degrees a second


	//moonOrbit += 2*(dt * M_PI/2);
   
	
	//model = glm::translate( glm::mat4(1.0f), glm::vec3(4.0 * sin(angle), 0.0, 4.0 * cos(angle)));
	
	//moon rotate around model
	//moon = glm::translate( model, glm::vec3(4.0 * sin(moonOrbit), 0.0, 4.0 * cos(moonOrbit)));
	
	//rotate moon



	rotAngle = (dt * M_PI/2 *180/M_PI)/2;
	model = glm::rotate(model, rotAngle, glm::vec3(0.0f, 1.0f, 0.0f));



    // Update the state of the scene
    glutPostRedisplay();//call the display callback
}


void reshape(int n_w, int n_h)
{
    w = n_w;
    h = n_h;
    //Change the viewport to be correct
    glViewport( 0, 0, w, h);
    //Update the projection matrix as well
    //See the init function for an explaination
    projection = glm::perspective(45.0f, float(w)/float(h), 0.01f, 100.0f);

}

void keyboard(unsigned char key, int x_pos, int y_pos)
{
    // Handle keyboard input
	switch(key)
	{
		case 27: //ESC
			exit(0);
			break;
		case 32: // Space Bar to start and stop rotate
			if(isRotate)
			{
				isRotate = false;
			}
			else
			{
				isRotate = true;
			}
			break;

		case 114: // 'r' for reverse
			if(!isReverse)
			{
				isReverse = true;
			}
			else
			{
				isReverse = false;
			}
			break;
	}
}

void arrowKeyboard(int key, int x, int y)
{
	    // Handle keyboard input
	switch(key)
	{
		case GLUT_KEY_LEFT: // left arrow
			isReverseOrbit = true;
			break;
		case GLUT_KEY_RIGHT: // right arrow
			isReverseOrbit = false;
			break;
		case GLUT_KEY_UP:
			zoomScale+= 2;
			if(zoomScale == 0)
				zoomScale+=2;
			view = glm::lookAt( glm::vec3(0.0, 8.0, zoomScale), //Eye Position
	                    glm::vec3(0.0, 0.0, 0.0), //Focus point
	                    glm::vec3(0.0, 1.0, 0.0));
			break;
		case GLUT_KEY_DOWN:
			zoomScale-= 2;
			if(zoomScale == 0)
				zoomScale-=2;
			view = glm::lookAt( glm::vec3(0.0, 8.0, zoomScale), //Eye Position
	                    glm::vec3(0.0, 0.0, 0.0), //Focus point
	                    glm::vec3(0.0, 1.0, 0.0));
			break;
	}
}

void mouse(int button, int state, int x, int y)
{
	if(button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN)
	{
		menu(button);
	}
	

	if(button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
	{
		menu(4); // case 4 reverse
	}

	
}

void menu(int id)
{
	switch(id)
	{
		case 1:
			exit(0);
			break;
		case 2:
			isRotate = true;
			break;
		case 3:
			isRotate = false;
			break;
		case 4:
			isReverse = !isReverse;
			break;
	}
	glutPostRedisplay();
}

bool initialize()
{
	GLint shader_status;
    // Initialize basic geometry and shaders for this example

    //this defines a cube, this is why a model loader is nice
    //you can also do this with a draw elements and indices, try to get that working



	
	std::vector<Vertex> tmpVect;
	tmpVect = loadModel.loadObj(filename, numOfVertices);
	
	Vertex *geometry = new Vertex[tmpVect.size()];


	for(unsigned int i =0; i<tmpVect.size(); i++)
		geometry[i] = tmpVect[i];

    // Create a Vertex Buffer object to store this vertex info on the GPU
    glGenBuffers(1, &vbo_geometry);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_geometry);
    glBufferData(GL_ARRAY_BUFFER, sizeof(*geometry)*tmpVect.size(), geometry, GL_STATIC_DRAW);

    //--Geometry done

   ShaderLoader vertShader(GL_VERTEX_SHADER);
	ShaderLoader fragShader(GL_FRAGMENT_SHADER);

    //Shader Sources
    // Put these into files and write a loader in the future
    // Note the added uniform!
    

	if(!vertShader.loadShader("../bin/shaders/vertShader.txt") || !
	fragShader.loadShader("../bin/shaders/fragShader.txt")){
		std::cout<<"ERROR: Load Shader Issue" << std::endl;
		return false;
	}
	

    //Now we link the 2 shader objects into a program
    //This program is what is run on the GPU
    program = glCreateProgram();
    glAttachShader(program, vertShader.getShader());
    glAttachShader(program, fragShader.getShader());
    glLinkProgram(program);
    //check if everything linked ok
    glGetProgramiv(program, GL_LINK_STATUS, &shader_status);
    if(!shader_status)
    {
        std::cerr << "[F] THE SHADER PROGRAM FAILED TO LINK" << std::endl;
        return false;
    }

    //Now we set the locations of the attributes and uniforms
    //this allows us to access them easily while rendering
    loc_position = glGetAttribLocation(program,
                    const_cast<const char*>("v_position"));
    if(loc_position == -1)
    {
        std::cerr << "[F] POSITION NOT FOUND" << std::endl;
        return false;
    }

    loc_color = glGetAttribLocation(program,
                    const_cast<const char*>("v_color"));
    if(loc_color == -1)
    {
        std::cerr << "[F] V_COLOR NOT FOUND" << std::endl;
        return false;
    }

    loc_mvpmat = glGetUniformLocation(program,
                    const_cast<const char*>("mvpMatrix"));
    if(loc_mvpmat == -1)
    {
        std::cerr << "[F] MVPMATRIX NOT FOUND" << std::endl;
        return false;
    }
    
    //--Init the view and projection matrices
    //  if you will be having a moving camera the view matrix will need to more dynamic
    //  ...Like you should update it before you render more dynamic 
    //  for this project having them static will be fine
    view = glm::lookAt( glm::vec3(0.0, 8.0, zoomScale), //Eye Position
                        glm::vec3(0.0, 0.0, 0.0), //Focus point
                        glm::vec3(0.0, 1.0, 0.0)); //Positive Y is up

    projection = glm::perspective( 45.0f, //the FoV typically 90 degrees is good which is what this is set to
                                   float(w)/float(h), //Aspect Ratio, so Circles stay Circular
                                   0.01f, //Distance to the near plane, normally a small value like this
                                   100.0f); //Distance to the far plane, 

    //enable depth testing
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
	glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );

    //and its done
    return true;
}

void cleanUp()
{
    // Clean up, Clean up
    glDeleteProgram(program);
    glDeleteBuffers(1, &vbo_geometry);
}

//returns the time delta
float getDT()
{
    float ret;
    t2 = std::chrono::high_resolution_clock::now();
    ret = std::chrono::duration_cast< std::chrono::duration<float> >(t2-t1).count();
    t1 = std::chrono::high_resolution_clock::now();
    return ret;
}

void renderString(const char *text)
{
	// disable shaders
	glUseProgram(0);

	glColor3f(1.0f, 1.0f, 1.0f);
	glRasterPos2f(-0.4f,-0.8f);
	// src = http://linux.die.net/man/3/glutbitmapstring
	glutBitmapString(GLUT_BITMAP_9_BY_15, (const unsigned char *)text);


	// enable shaders
	glUseProgram(program);


}
