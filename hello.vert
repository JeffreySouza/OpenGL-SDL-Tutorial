#version 330 core

// vertex shader

layout (location = 0) in vec3 position;

void main() {
	// simply forward data to shader output
	// usually input data is not already in normalized device coords
	// so it needs to be transformed into coords within OpenGL's visible region
	
	// gl_Position is a vec4, and its value at the end of main is used as
	// the output of the vertex shader
	// last value is w-component, used for perspective division
	gl_Position = vec4( position.x, position.y, position.z, 1.0f );
}

