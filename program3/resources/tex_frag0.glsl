#version 330 core
uniform sampler2D Texture0;
uniform vec3 lightColor;

in vec2 vTexCoord;
in vec3 fragNor;
in vec3 lightDir;
in vec3 EPos;
out vec4 Outcolor;

void main() {
  	vec4 texColor0 = texture(Texture0, vTexCoord);

	vec3 normal = normalize(fragNor);
	//if (normal.z < 0) {
	//	normal = -normal;
	//}
	vec3 light = normalize(lightDir);

	float dC = max(dot(normal, light), 0.0);
	vec3 diffuse_color = dC * texColor0.xyz;

	vec3 halfway = normalize(-EPos);
	float sC = max(dot(halfway, normal), 0.0);
	vec3 spec_color = sC * texColor0.xyz;

	Outcolor = vec4(diffuse_color + spec_color, 1.0);
 	//to set the out color as the texture color 
	// Outcolor = texColor0;
  	//to set the outcolor as the texture coordinate (for debugging)
	//Outcolor = vec4(vTexCoord.s, vTexCoord.t, 0, 1);
}

