/******************************************************************
    This file is part of DomeSimulator.

    DomeSimulator is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    DomeSimulator is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with DomeSimulator.  If not, see <http://www.gnu.org/licenses/>.

    DomeSimulator is free for non-commercial use. If you want to use it 
    commercially, you should contact the author 
    Michael Winkelmann aka Wilston Oreo by mail:
    me@wilstonoreo.net
**************************************************************************/

uniform sampler2D image; // Texture image
uniform sampler2D texcoord_map;
uniform vec3 offset;
uniform vec3 stretch;

void main()
{
  gl_FragColor = vec4(0.0,0.0,0.0,1.0);
  vec2 uv = gl_TexCoord[0].st*vec2(1.0,0.5);
  vec3 texCoords = offset + stretch*texture2D(texcoord_map,uv).xyz*2.0-1.0 + texture2D(texcoord_map,uv + vec2(0.0,0.5)).xyz*2.0/255.0 - 1.0 / 255.0;

  texCoords = fract(texCoords);
  gl_FragColor = texture2D(image, texCoords); 
}

