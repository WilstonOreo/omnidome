////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// FFGLLib.h
//
// FreeFrame is an open-source cross-platform real-time video effects plugin system.
// It provides a framework for developing video effects plugins and hosts on Windows,
// Linux and Mac OSX.
//
// Copyright (c) 2006 www.freeframe.org
// All rights reserved.
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// FFGLLib.h by Trey Harrison
// www.harrisondigitalmedia.com
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// Redistribution and use in source and binary forms, with or without modification,
//	are permitted provided that the following conditions are met:
//
//  * Redistributions of source code must retain the above copyright
//    notice, this list of conditions and the following disclaimer.
//  * Redistributions in binary form must reproduce the above copyright
//    notice, this list of conditions and the following disclaimer in
//    the documentation and/or other materials provided with the
//    distribution.
//  * Neither the name of FreeFrame nor the names of its
//    contributors may be used to endorse or promote products derived
//    from this software without specific prior written permission.
//
//
//	THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
//	ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
//	WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
//	IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT,
//	INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
//	BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
//	DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY
//	OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE
//	OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED
//	OF THE POSSIBILITY OF SUCH DAMAGE.
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef __FFGLLIB_H__
#define __FFGLLIB_H__

//FFGLTexCoords
typedef struct FFGLTexCoordsTag
{
  GLdouble s,t;
} FFGLTexCoords;

//helper function to return the s,t,r coordinate
//that cooresponds to the width,height,depth of the used
//portion of the texture
inline FFGLTexCoords GetMaxGLTexCoords(FFGLTextureStruct t)
{
  FFGLTexCoords texCoords;

  //the texture may only occupy a portion
  //of the allocated hardware texture memory

  //normalized (0..1) S and T coords
  texCoords.s = ((GLdouble)t.Width) / (GLdouble)t.HardwareWidth;
  texCoords.t = ((GLdouble)t.Height) / (GLdouble)t.HardwareHeight;

  return texCoords;
}

#endif

