
uniform sampler2D texture; // Texture image

void main()
{
  gl_FragColor = texture2D(texture,gl_TexCoord[0].st);
}
