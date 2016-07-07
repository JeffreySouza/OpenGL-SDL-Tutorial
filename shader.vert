#version 330 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 color;

//out vec4 vertexColor;
out vec3 ourColor; // output color for fragment shader

void main() {
	gl_Position = vec4( position.xyz, 1.0 );
	//vertexColor = vec4( 0.5f, 0.0f, 0.0f, 1.0f );
	ourColor = color; // Set ourColor to the input color we got from the vertex data
}

