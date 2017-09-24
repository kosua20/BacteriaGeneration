#version 330

in INTERFACE {
    vec3 n;
	vec2 uv;
} In ;

uniform vec3 color;
uniform int id;

// Output: the fragment color
layout (location = 0) out vec4 fragColor;

// Classic one-line low quality random noise.
float rand(vec2 co) {
	return fract(sin(dot(co.xy, vec2(12.9898, 78.233))) * 137.5453);
}

void main(){
	
	vec3 n = normalize(In.n);
	
	// Rim effect, stronger on the edges, where the normal is ortogonal to the z axis.
	float rim = abs(n.z);
	// Apply rim and color, set transparency.
	fragColor.rgb = rim*color;
	fragColor.a = 0.5;
	
	// Add a nucleus of radius 0.5 based on the UVs, for bacteria w/ odd IDs.
	float mixi = mod(id,2) == 0 ? 1.0 : smoothstep(0.4, 0.5, length(In.uv));
	// Make the nucleus darker.
	fragColor.rgb *= mixi*0.7+0.3;
	
	// Inside the nucleus of half of the organisms, and in half of the other too, add some noise (trnasparency).
	if((mixi == 0.0 && mod(id,4) == 1) || mod(id,4) == 0){
		fragColor.a = rand(In.uv);
	}
	
}
