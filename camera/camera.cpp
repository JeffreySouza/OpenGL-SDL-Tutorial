#include <string>
#include <fstream>

#include <glm/fwd.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#ifdef USING_WINDOWS
	#include <SDL.h>
	#include <glew.h>
	#include <SOIL.h>
#else
	#include <SDL2/SDL.h>
	#include <GL/glew.h>
	#include <SOIL/SOIL.h>
#endif

// ========== CONSTANTS ==========

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

// ========== GLOBALS ==========

SDL_Window* gWINDOW = NULL;
SDL_GLContext gGLCONTEXT = NULL;

// ========== FUNCTION HEADERS ==========

bool init();

GLuint createShader( std::string filename, GLenum shaderType );


void close();

// ========== MAIN ==========
#ifdef main
# undef main
#endif
int main() {
	if ( !init() ) {
		printf( "Failed to initialize!\n" );
		return 0;
	}

	// ===== create camera =====
	glm::vec3 cameraPos = glm::vec3( 0.0f, 0.0f, 3.0f );
	glm::vec3 cameraFront = glm::vec3( 0.0f, 0.0f, -1.0f );
	glm::vec3 cameraUp = glm::vec3( 0.0f, 1.0f, 0.0f );

	glm::mat4 view;
	GLfloat cameraSpeed = 0.05f;
			
	// ===== create textures =====
	int width, height;
	unsigned char* boximage = SOIL_load_image( "img/container.jpg", &width, &height, 0, SOIL_LOAD_RGB );
	unsigned char* smileimage = SOIL_load_image( "img/smile.png", &width, &height, 0, SOIL_LOAD_RGB );

	GLuint texture[2];
	glGenTextures( 2, texture );

	glBindTexture( GL_TEXTURE_2D, texture[0] );

	glTexImage2D( GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, boximage );
	glGenerateMipmap( GL_TEXTURE_2D );
	
	SOIL_free_image_data( boximage );

	glBindTexture( GL_TEXTURE_2D, texture[1] );

	glTexImage2D( GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, smileimage );
	glGenerateMipmap( GL_TEXTURE_2D );
	
	SOIL_free_image_data( smileimage );

	glBindTexture( GL_TEXTURE_2D, 0 );

	// ===== get shaders =====
	GLuint vertexShader = createShader( "camera/camera.vert", GL_VERTEX_SHADER );
	GLuint fragmentShader = createShader( "camera/camera.frag", GL_FRAGMENT_SHADER );
	
	// ===== create shader program =====
	GLuint shaderProgram;
	shaderProgram = glCreateProgram();

	glAttachShader( shaderProgram, vertexShader );
	glAttachShader( shaderProgram, fragmentShader );
	glLinkProgram( shaderProgram );

	GLint success;
	glGetProgramiv( shaderProgram, GL_LINK_STATUS, &success );
	if ( !success ) {
		GLchar infoLog[512];
	    glGetProgramInfoLog( shaderProgram, 512, NULL, infoLog );
		printf( "ERROR: SHADER PROGRAM LINKING FAILED. ERROR: %s\n", infoLog );
	}

	glDeleteShader( vertexShader );
	glDeleteShader( fragmentShader );
	
	GLfloat vertices[] = {
		// Positions          	  // Texture Coords
		-0.5f, -0.5f, -0.5f,	  0.0f, 0.0f,
		 0.5f, -0.5f, -0.5f,	  1.0f, 0.0f,
		 0.5f,  0.5f, -0.5f,	  1.0f, 1.0f,
		 0.5f,  0.5f, -0.5f,	  1.0f, 1.0f,
		-0.5f,  0.5f, -0.5f,	  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,	  0.0f, 0.0f,

		-0.5f, -0.5f,  0.5f,	  0.0f, 0.0f,
		 0.5f, -0.5f,  0.5f,	  1.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,	  1.0f, 1.0f,
		 0.5f,  0.5f,  0.5f,	  1.0f, 1.0f,
		-0.5f,  0.5f,  0.5f,	  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,	  0.0f, 0.0f,

		-0.5f,  0.5f,  0.5f,	  1.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,	  1.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,	  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,	  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,	  0.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,	  1.0f, 0.0f,

		 0.5f,  0.5f,  0.5f,	  1.0f, 0.0f,
		 0.5f,  0.5f, -0.5f,	  1.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,	  0.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,	  0.0f, 1.0f,
		 0.5f, -0.5f,  0.5f,	  0.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,	  1.0f, 0.0f,

		-0.5f, -0.5f, -0.5f,	  0.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,	  1.0f, 1.0f,
		 0.5f, -0.5f,  0.5f,	  1.0f, 0.0f,
		 0.5f, -0.5f,  0.5f,	  1.0f, 0.0f,
		-0.5f, -0.5f,  0.5f,	  0.0f, 0.0f,
		-0.5f, -0.5f, -0.5f,	  0.0f, 1.0f,

		-0.5f,  0.5f, -0.5f,	  0.0f, 1.0f,
		 0.5f,  0.5f, -0.5f,	  1.0f, 1.0f,
		 0.5f,  0.5f,  0.5f,	  1.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,	  1.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,	  0.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,	  0.0f, 1.0f
	};

	glm::vec3 cubePositions[] = {
		glm::vec3(  0.0f,  0.0f,  0.0f), 
		glm::vec3(  2.0f,  5.0f, -15.0f), 
		glm::vec3( -1.5f, -2.2f, -2.5f),  
		glm::vec3( -3.8f, -2.0f, -12.3f),  
		glm::vec3(  2.4f, -0.4f, -3.5f),  
		glm::vec3( -1.7f,  3.0f, -7.5f),  
		glm::vec3(  1.3f, -2.0f, -2.5f),  
		glm::vec3(  1.5f,  2.0f, -2.5f), 
		glm::vec3(  1.5f,  0.2f, -1.5f), 
		glm::vec3( -1.3f,  1.0f, -1.5f)  
	};

	GLuint VBO;
	glGenBuffers( 1, &VBO );

	GLuint VAO;
	glGenVertexArrays( 1, &VAO );
	glBindVertexArray( VAO );
		glBindBuffer( GL_ARRAY_BUFFER, VBO );
		glBufferData( GL_ARRAY_BUFFER, sizeof( vertices ), vertices, GL_STATIC_DRAW );
	
		// Poisiton attributes
		glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof( GLfloat ), (GLvoid*)0 );
		glEnableVertexAttribArray( 0 );

		// Texture coordinate attributes
		glVertexAttribPointer( 1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof( GLfloat ), (GLvoid*)(3 * sizeof( GLfloat )) );
		glEnableVertexAttribArray( 1 );

	glBindVertexArray( NULL );

	// Declare matrix used for rotation
	//glm::mat4 trans;

	// Orthographic projection matrix
	glm::ortho(0.0f, 800.0f, 0.0f, 600.0f, 0.1f, 100.0f);

	// Prospective projection matrix
	glm::mat4 proj = glm::perspective( glm::radians( 45.0f ), (float)width/(float)height, 0.1f, 100.0f);

	// Model matrix
	glm::mat4 model;
	model = glm::rotate( model, glm::radians( -55.0f ), glm::vec3( 1.0f, 0.0f, 0.0f ) );

	// Projection matrix
	glm::mat4 projection;
	projection = glm::perspective( glm::radians( 45.0f ), (GLfloat)SCREEN_WIDTH / (GLfloat)SCREEN_HEIGHT, 0.1f, 100.0f );  

	// ===== set loop flags =====
	bool quit = false;
	SDL_Event e;

	while ( !quit ) {
		// ===== handle events =====
		while ( SDL_PollEvent( &e ) != 0 ) {
			if ( e.type == SDL_QUIT ) {
				quit = true;
			} else if ( e.type == SDL_KEYDOWN ) {
				// select surfaces based on key press
				switch ( e.key.keysym.sym ) {

					case SDLK_UP:
						cameraPos += cameraSpeed * cameraFront;
						break;
					case SDLK_DOWN:
						cameraPos -= cameraSpeed * cameraFront;
						break;
					case SDLK_LEFT:
						cameraPos -= glm::normalize( glm::cross( cameraFront, cameraUp ) ) * cameraSpeed;
						break;
					case SDLK_RIGHT:
						cameraPos += glm::normalize( glm::cross( cameraFront, cameraUp ) ) * cameraSpeed;
						break;
					default:
						break;

				}
			}
		}
		
		// ===== render =====
	
		// == set state ==
		glEnable( GL_DEPTH_TEST );
		glClearColor( 0.2f, 0.3f, 0.3f, 1.0f );
		
		// == use state ==
		glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

		glUseProgram( shaderProgram );

		// Move camera
		/*
		GLfloat radius = 8.0f;
		GLfloat camX = sin( glm::radians( (GLfloat)SDL_GetTicks() / 20 ) ) * radius;
		GLfloat camZ = cos( glm::radians( (GLfloat)SDL_GetTicks() / 20 ) ) * radius;
		view = glm::lookAt( glm::vec3( camX, 0.0, camZ ), glm::vec3( 0.0, 0.0, 0.0 ), glm::vec3( 0.0, 1.0, 0.0 ) );
		*/
		view = glm::lookAt( cameraPos, cameraPos + cameraFront, cameraUp );

		// == ROTATE ==
		glm::mat4 trans;
		GLfloat ticks = SDL_GetTicks();

		//trans = glm::translate( trans, glm::vec3( 0.2f, -0.2f, 0.0f ) ); 
		trans = glm::rotate( trans, ticks * glm::radians( 0.1f ), glm::vec3( 0.0f, 0.0f, 1.0f ) ); 

		//
		GLuint transformLoc = glGetUniformLocation( shaderProgram, "transform" );
		glUniformMatrix4fv( transformLoc, 1, GL_FALSE, glm::value_ptr( trans ) );
	
		//
		GLuint modelLoc = glGetUniformLocation( shaderProgram, "model" );
		glUniformMatrix4fv( modelLoc, 1, GL_FALSE, glm::value_ptr( model ) );

		//
		GLuint viewLoc = glGetUniformLocation( shaderProgram, "view" );
		glUniformMatrix4fv( viewLoc, 1, GL_FALSE, glm::value_ptr( view ) );

		//
		GLuint projectionLoc = glGetUniformLocation( shaderProgram, "projection" );
		glUniformMatrix4fv( projectionLoc, 1, GL_FALSE, glm::value_ptr( projection ) );

		glActiveTexture( GL_TEXTURE0 );
		glBindTexture( GL_TEXTURE_2D, texture[0] );
		glUniform1i( glGetUniformLocation( shaderProgram, "ourTexture1"), 0 );
	
		glActiveTexture( GL_TEXTURE1 );
		glBindTexture( GL_TEXTURE_2D, texture[1] );
		glUniform1i( glGetUniformLocation( shaderProgram, "ourTexture2"), 1 );

		glBindVertexArray( VAO );

		for ( GLuint i = 0; i < 10; i++ ) {
			glm::mat4 model;
			model = glm::translate( model, cubePositions[i] );
			GLfloat angle = glm::radians( 20.0f * i ); 
			model = glm::rotate( model, angle, glm::vec3( 1.0f, 0.3f, 0.5f ) );
			glUniformMatrix4fv( modelLoc, 1, GL_FALSE, glm::value_ptr( model ) );

			glDrawArrays( GL_TRIANGLES, 0, 36 );
		}		

		glBindVertexArray( NULL );

		// ===== swap buffers =====
		SDL_GL_SwapWindow( gWINDOW );
	}
	
	close();
	
	return 0;
}

// ========== FUNCTION DEFINITIONSs ==========

bool init() {
	// init sdl
	if ( SDL_Init( SDL_INIT_VIDEO ) < 0 ) {
		printf( "SDL could not initialize! SDL_Error: %s\n", SDL_GetError() );
		return false;
	}
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
	
	// init window
	gWINDOW = SDL_CreateWindow( "OpenGL Tutorial", SDL_WINDOWPOS_CENTERED,
								SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT,
								SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL );
	if ( gWINDOW == NULL ) {
		printf( "Window could not be created! SDL_Error: %s\n", SDL_GetError() );
		return false;
	}
	
	// init glcontext
	gGLCONTEXT = SDL_GL_CreateContext( gWINDOW );
	if ( gGLCONTEXT == NULL ) {
		printf( "GLContext could not be created! SDL_Error: %s\n", SDL_GetError() );
		return false;
	}
	
	// Initialize GLEW (load OpenGL functionality)
	glewExperimental = GL_TRUE; // allow modern OpenGL functionality to run
	GLenum err = glewInit();
	if ( err != GLEW_OK ) {
		printf( "GLEW failed to load! GLEW error: %s\n", glewGetErrorString( err ) );
		return false;
	}
	
	// tell OpenGl the size of the window
	// f( lower left x, lower left y, width, height )
	glViewport( 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT );
	
	return true;
} // init

GLuint createShader( std::string filename, GLenum shaderType ) {
	// read in source of shader
	std::ifstream shaderFile;
	shaderFile.open( filename.c_str() );
	
	std::string tmp, shaderSource;
	
	while ( getline(shaderFile,tmp) ) {
		shaderSource += tmp;
		shaderSource += '\n';
	}
	
	shaderFile.close();
	
	const char *sfCStr = shaderSource.c_str();
	const int sfCStrLength = shaderSource.length();
	
	// create and compile shader
	GLuint shaderID;
	shaderID = glCreateShader(shaderType);
	glShaderSource( shaderID, 1, &sfCStr, &sfCStrLength );
	glCompileShader( shaderID );
	
	// check errors
	GLint success;
	glGetShaderiv( shaderID, GL_COMPILE_STATUS, &success );
	if ( !success ) {
		GLchar infoLog[512];
		glGetShaderInfoLog( shaderID, 512, NULL, infoLog );
		printf( "ERROR: SHADER COMPILATION FAILED. ERROR: %s\n", infoLog );
	}
	
	return shaderID;
} // createShader

void close() {
	SDL_GL_DeleteContext( gGLCONTEXT );
	gGLCONTEXT = NULL;

	SDL_DestroyWindow( gWINDOW );
	gWINDOW = NULL;

	SDL_Quit();
} // close

