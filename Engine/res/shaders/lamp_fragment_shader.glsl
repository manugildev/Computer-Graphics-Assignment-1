#version 330

out vec4 frag_color;

uniform vec3 object_color;

void main(){
	frag_color = vec4(object_color, 1.0);
}