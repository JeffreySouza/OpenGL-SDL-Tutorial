#ifndef SHADER_H
#define SHADER_H

#include <string>
#include <fstream>
#include <sstream>
  
#include <GL/glew.h>; // Include glew to get all the required OpenGL headers

class Shader {
public:
  	// The program ID
	GLuint Program;
  	// Use the program
  	void Use() { glUseProgram( Program ); }

	// Constructor reads and builds the shader
	Shader( const GLchar* vertexPath, const Glchar* fragmentPath ) {
		// Retrieve shader source code from filePath
		std::string vertexCode;
		std::string fragmentCode;

		std::ifstream vShaderFile;
		std::ifstream fShaderFile;
		
		// ensures ifstream objects can throw exceptions
		vShaderFile.exceptions( std::ifstream::badbit );
		fShaderFile.exceptions( std::ifstream::badbit );

		try {
			// Open files
			vShaderFile.open( vertexPath );
			fShaderFile.open( fragmentPath );

			// Read file contents
			vShaderStream << vSahderFile.rdbuf();
			fShaderStream << fSahderFile.rdbuf();

			// close file handlers
			vShaderFile.close();
			fShaderFile.close();

			// convert stream into GLchar array
			vertexCode = vShaderStream.str();
			fragmentCode = fShaderStream.str();
		}
		catch ( std::ifstream::failure e ) {
			printf( "ERROR::SHADER::FILE_NOT_SUCCESSFULLY_READ\n" ); 
		}
		
		const GLchar* vShaderCode = vertexCode.c_str();
		const GLchar* fShaderCode = fragmentCode.c_str();

		// Compile shaders
		GLuint vertex, fragment;
		GLint success;
		GLchar infoLog[512];

		// Vertex Shader
		vertex = glCreateShader( GL_VERTEX_SHADER );
		glShaderSource( vertex, 1, &vShaderCode, NULL );
		glCompileShader( vertex );
		
		glGetShaderiv( vertex, GL_COMPILE_STATUS, &success );
		if ( !success ) {
			glGetShaderInfoLog( vertex, 512, NULL, infoLog );
			printf( "ERROR::SHADER::VERTEX::COMPILATION_FAILED:\n%s\n", infoLog ); 
		}

		// Fragment Shader
		fragment = glCreateShader( GL_FRAGMENT_SHADER );
		glShaderSource( fragment, 1, &fShaderCode, NULL );
		glCompileShader( fragment );
		
		glGetShaderiv( fragment, GL_COMPILE_STATUS, &success );
		if ( !success ) {
			glGetShaderInfoLog( fragment, 512, NULL, infoLog );
			printf( "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED:\n%s\n", infoLog ); 
		}

		// Shader Program
		Program = glCreateProgram();
		glAttachShader( Program, vertex );
		glAttachShader( Program, fragment );
		glLinkProgram( Program );

		glGetProgramiv( Program, GL_LINK_STATUS, &success );
		if ( !success ) {
			glGetProgramInfoLog(this->Program, 512, NULL, infoLog);
			printf( "ERROR::SHADER::PROGRAM::LINKING_FAILED:\n%s\n", infoLog ); 
		}

		// Delete shaders
		glDeleteShader( vertex );
		glDeleteShader( fragment );
	}
};
  
#endif
