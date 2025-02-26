#version 330 core 

out vec4 color;

uniform vec3 MatAmb;
uniform vec3 MatDif;
uniform vec3 MatSpec;
uniform float MatShine;
uniform vec3 lightColor;

//interpolated normal and light vector in camera space
in vec3 fragNor;
in vec3 lightDir;
//position of the vertex in camera space
in vec3 EPos;

void main()
{
	//you will need to work with these for lighting
	vec3 normal = normalize(fragNor);
	//if (normal.z < 0) {
	//	normal = -normal;
	//}
	vec3 light = normalize(lightDir);

	float dC = max(dot(normal, light), 0.0);
	vec3 diffuse_color = dC * MatDif * lightColor;

	vec3 halfway = normalize(-EPos);
	float sC = max(pow(dot(halfway, normal), MatShine), 0.0);
	vec3 spec_color = sC * MatSpec * lightColor;


	//color = vec4(MatAmb + diffuse_color , 1.0);
	color = vec4(MatAmb + diffuse_color + spec_color, 1.0);
}
