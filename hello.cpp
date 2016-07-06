
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
		// get rid of previous color
		glClearColor( 0.2f, 0.3f, 0.3f, 1.0f );
		
		// == use state ==
		// fill entire color buffer with color from glClearColor
		glClear( GL_COLOR_BUFFER_BIT );
		
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

void close() {
	SDL_GL_DeleteContext( gGLCONTEXT );
	gGLCONTEXT = NULL;

	SDL_DestroyWindow( gWINDOW );
	gWINDOW = NULL;

	SDL_Quit();
} // close

