varying vec4 Color;
uniform sampler2D tex;

void main (void)
{
   gl_FragColor = Color * texture2D(tex, gl_TexCoord[0].st);
}
