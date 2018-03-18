#version 450

in vec2 texture_coords;
out vec4 out_color;
uniform sampler2D gui_texture;

void make_kernel(inout vec4 n[9], sampler2D tex, vec2 coord) {
	float w = 1.0 / 940;
	float h = 1.0 / 560;

	n[0] = texture2D(tex, coord + vec2( -w, -h));
	n[1] = texture2D(tex, coord + vec2(0.0, -h));
	n[2] = texture2D(tex, coord + vec2(  w, -h));
	n[3] = texture2D(tex, coord + vec2( -w, 0.0));
	n[4] = texture2D(tex, coord);
	n[5] = texture2D(tex, coord + vec2(  w, 0.0));
	n[6] = texture2D(tex, coord + vec2( -w, h));
	n[7] = texture2D(tex, coord + vec2(0.0, h));
	n[8] = texture2D(tex, coord + vec2(  w, h));
}

void main(void) {
	vec4 n[9];
	make_kernel(n, gui_texture, texture_coords);

	vec4 sobel_edge_h = n[2] + (2.0*n[5]) + n[8] - (n[0] + (2.0*n[3]) + n[6]);
  	vec4 sobel_edge_v = n[0] + (2.0*n[1]) + n[2] - (n[6] + (2.0*n[7]) + n[8]);
	vec4 sobel = sqrt((sobel_edge_h * sobel_edge_h) + (sobel_edge_v * sobel_edge_v));
	vec3 c = 1.0 - sobel.rgb;

	float grayScale = dot(c, vec3(0.19, 0.97, 0.09));
	
	float a = grayScale > 0.999 ? 0 : 1;
	grayScale = min(grayScale, 0.2);
	//a = min(a, 1- texture2D(gui_texture, texture_coords).a);
	out_color = vec4(grayScale, grayScale, grayScale, a);
}