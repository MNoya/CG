varying vec4 Color;

uniform int especular;

void main(void)
{
   gl_Position = gl_ModelViewMatrix * gl_Vertex;
   vec3 L = normalize(gl_LightSource[0].position.xyz - gl_Position.xyz);
   vec3 N = normalize(gl_NormalMatrix*gl_Normal);
   vec3 R = normalize(reflect(L,N));
   vec3 V = normalize(gl_Position.xyz);
   Color = gl_FrontLightProduct[0].diffuse * max(0, dot(L,N));
   if(especular == 1)
     Color+= gl_FrontLightProduct[0].specular * pow(max(0,dot(R,V)),gl_FrontMaterial.shininess);
   gl_TexCoord[0]=gl_MultiTexCoord0;
   gl_Position = gl_ProjectionMatrix * gl_Position;

}
