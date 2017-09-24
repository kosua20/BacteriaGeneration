#version 330

// Attributes
layout(location = 0) in vec3 v;
layout(location = 1) in vec3 n;
layout(location = 2) in vec2 uv;

uniform mat4 mvp;
uniform float time;
uniform int id;

out INTERFACE {
    vec3 n;
	vec2 uv;
} Out ;



void main(){
	
	// Pass normal for rim effect.
	Out.n = normalize(n);
	
	// Compute an offset along the normal, w/ random tweaks.
	vec3 offset = Out.n * (sin(time*2.0+v.x+v.x*v.y)*0.5+0.4);
	vec3 finalV = v+offset;
	
	// Rotate the bacterium in 2D.
	float theta = time*(1.12)*(mod(id, 10)/5.0)-float(id)*13.17;
	theta *= (mod(id, 3) == 0 ? -1.0 : 1.0);
	float ctheta = cos(theta);
	float stheta = sin(theta);
	float newX = ctheta*finalV.x + stheta*finalV.y;
	float newY = ctheta*finalV.y - stheta*finalV.x;
	
	// Apply the rotation.
	finalV.x = newX;
	finalV.y = newY;
	
	// We multiply the coordinates by the MVP matrix, and ouput the result.
	gl_Position =  mvp * vec4(finalV, 1.0);

	// Apply the same effect to the UV but with a lower intensity (for nucleus).
	Out.uv = v.xy+0.5*offset.yx;
	
}
