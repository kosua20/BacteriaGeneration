#version 330

// Input: UV coordinates
in INTERFACE {
	vec2 uv;
} In ;

// Uniforms: the texture, inverse of the screen size, FXAA flag.
uniform sampler2D screenTexture;
uniform vec3 globalColor;
uniform bool invert;
// Output: the fragment color
out vec3 fragColor;


// Classic one-line low quality random noise.
float rand(vec2 co) {
	return fract(sin(dot(co.xy, vec2(12.9898, 78.233))) * 137.5453);
}

void main(){
	vec2 uv = In.uv;
	float radius = length(uv - vec2(0.5));
	
	// External circle of the microscope, w/ small variations.
	if(radius > 0.45-0.003*rand(uv)){
		discard;
	}
	// Read in scene framebuffer.
	vec3 finalColor = texture(screenTexture,uv).rgb;
	
	// Chromatic aberration near the edges.
	if(radius > 0.15){
		// Read the green and blue components of neighboring pixels.
		float green = textureOffset(screenTexture,uv, ivec2(3,2)).g;
		float blue = textureOffset(screenTexture,uv, ivec2(-2,-3)).b;
		// Chromatic aberration is stronger near the edge.
		float factor = (radius-0.15)/0.3;
		// Mix components for smooth variation of the effect intensity.
		finalColor.g = mix(finalColor.g, green, factor);
		finalColor.b = mix(finalColor.b, blue, factor);
	}
	// Invert color if needed.
	if(invert){
		finalColor = 1.0 - finalColor;
	}
	
	// Add a secondary circle close to the microscope edges, w/ small variations.
	if(radius > 0.444-0.004*rand(uv)){
		finalColor *= 0.5;
	}
	// Attenuate color based on distance from image center (brighter at the center, dimmer on the edges.
	float attenuation = clamp(-2.333*radius+1.6,0.0,1.0);
	// Apply attenuation and global tint.
	fragColor = finalColor*attenuation*globalColor;
	// Desaturate a bit by mixing the color and the grey of same intensity.
	fragColor = mix(fragColor, vec3(dot(fragColor, vec3(0.299, 0.587, 0.114))), 0.25);
}
