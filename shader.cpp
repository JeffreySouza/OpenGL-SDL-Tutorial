#include <string>
#include <fstream>

#ifdef USING_WINDOWS
	#include <SDL.h>
	#include <glew.h>
#else
	#include <SDL2/SDL.h>
	#include <GL/glew.h>
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
	
	// ===== get shaders =====
	GLuint vertexShader = createShader( "shader.vert", GL_VERTEX_SHADER );
	GLuint fragmentShader = createShader( "shader.frag", GL_FRAGMENT_SHADER );
	
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
		// Positions	      // Colors
		-0.5f, -0.5f, 0.0f,   1.0f, 0.0f, 0.0f,		// Bottom right
		0.5f, -0.5f, 0.0f,    0.0f, 1.0f, 0.0f,		// Bottom left
		0.0f,  0.5f, 0.0f,    0.0f, 0.0f, 1.0f		// Top
	};
	
	GLuint VBO;
	glGenBuffers( 1, &VBO );
		
	GLuint VAO;
	glGenVertexArrays( 1, &VAO );
	glBindVertexArray( VAO );
		glBindBuffer( GL_ARRAY_BUFFER, VBO );
		
		glBufferData( GL_ARRAY_BUFFER, sizeof( vertices ), vertices, GL_STATIC_DRAW );
	
		// Poisiton attributes
		glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof( GLfloat ), (GLvoid*)0 );
		glEnableVertexAttribArray( 0 );

		// Color attributes
		glVertexAttribPointer( 1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof( GLfloat ), (GLvoid*)(3 * sizeof( GLfloat )) );
		glEnableVertexAttribArray( 1 );

	glBindVertexArray( NULL );

	// ===== set loop flags =====
	bool quit = false;
	SDL_Event e;

	while ( !quit ) {
		// ===== handle events =====
		while ( SDL_PollEvent( &e ) != 0 ) {
			if ( e.type == SDL_QUIT ) {
				quit = true;
			}
		}
		
		// ===== render =====
	
		// == set state ==
		glClearColor( 0.2f, 0.3f, 0.3f, 1.0f );
		
		// == use state ==
		glClear( GL_COLOR_BUFFER_BIT );

		glUseProgram( shaderProgram );

		// Change color of uniform variable for shader
		GLfloat timeValue = SDL_GetTicks() / 1000.f;
		GLfloat greenValue = ( sin( timeValue ) / 2 ) + 0.5;
		GLint vertexColorLocation = glGetUniformLocation( shaderProgram, "ourColor" );
		glUniform4f( vertexColorLocation, 0.0f, greenValue, 0.0f, 1.0f );

		// Draw triangle
		glBindVertexArray( VAO );
			glDrawArrays( GL_TRIANGLES, 0, 3 );	
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

