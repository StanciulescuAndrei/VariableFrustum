#version 130

uniform mat4 projection, modelview;
uniform mat3 normalMatrix;

in vec3 position;

void main()
{
	// Transform position from pixel coordinates to clipping coordinates
	gl_Position = projection * modelview * vec4(position, 1.0);
}

