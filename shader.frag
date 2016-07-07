#version 330 core
//in vec4 vertexColor;

in vec3 ourColor;
out vec4 color;

//uniform vec4 ourColor;

void main() {
	//color = vertexColor;
	color = vec4( ourColor, 1.0f );
}

