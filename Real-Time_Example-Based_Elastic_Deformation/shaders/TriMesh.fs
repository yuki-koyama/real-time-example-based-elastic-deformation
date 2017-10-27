// #define COLOR_ADJUSTMENT

float diffuseReflectance = 5.0;

#ifdef COLOR_ADJUSTMENT
uniform float contrast;
uniform float lightness;
#endif

varying vec3 normal;
varying vec3 pos;

void main()
{
    vec3 n = normalize(normal);

    // Get emissive
//  vec4 emissive = gl_FrontMaterial.emissive;
	vec4 emissive = vec4(0.0, 0.0, 0.0, 0.0);
    
    vec4 color = gl_Color;

	// Get ambient.
//	vec4 ambient = gl_FrontMaterial.ambient * gl_LightSource[0].ambient;
    vec4 ambient = color * 0.2;

	// Get diffuse.
    vec3 lightVec = gl_LightSource[0].position.xyz - pos;
    float lightDistance = length(lightVec);
	vec3 lightVecNormal = normalize(lightVec);
	float diffuseLight = max(dot(n, lightVecNormal), 0.0) * diffuseReflectance;
//	vec4 diffuse = gl_FrontMaterial.diffuse * gl_LightSource[0].diffuse * diffuseLight;
    vec4 diffuse = gl_LightSource[0].diffuse * diffuseLight / lightDistance;

	// Get specular.
	vec3 eyeVec = normalize(vec3(0.0, 0.0, 0.0) - pos);
    vec3 halfVec = normalize(lightVec + eyeVec);
//	float shininess = max(gl_FrontMaterial.shininess, 0.0);
	float shininess = 50.0;
	float specularLight = pow(max(dot(n, halfVec), 0.0), shininess);

	// If there's no diffuse light, turn off the specular
	if(diffuseLight <= 0.0) specularLight = 0.0;
    vec4 specularColor = vec4(0.6, 0.6, 0.6, 1.0);
//	vec4 specular = gl_FrontMaterial.specular * specularLight;
    vec4 specular = specularColor * specularLight;
	    
	// Final lighting color.
    vec4 color2 = (emissive + ambient + diffuse) * color + specular;
#ifdef COLOR_ADJUSTMENT
    gl_FragColor = (color2 - lightness) * contrast + 0.5;
#else
    gl_FragColor = color2;
#endif
	gl_FragColor.w = 1.0;
}
