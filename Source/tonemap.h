/*
	Copyright (c) 2011, T. Kroes <t.kroes@tudelft.nl>
	All rights reserved.

	Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

	- Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.
	- Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.
	- Neither the name of the TU Delft nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.
	
	THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#pragma once

#include "geometry.h"

namespace ExposureRender
{

HOST_DEVICE ColorRGBAuc ToneMap(const ColorXYZAf& XYZA)
{
	ColorRGBf RGB = ColorRGBf::FromXYZAf(XYZA.D);

	RGB[0] = 1.0f - expf(-(RGB[0] / gpTracer->Camera.Exposure));
	RGB[1] = 1.0f - expf(-(RGB[1] / gpTracer->Camera.Exposure));
	RGB[2] = 1.0f - expf(-(RGB[2] / gpTracer->Camera.Exposure));

	RGB.Clamp(0.0f, 1.0f);

	ColorRGBAuc Result;

	Result[0] = (unsigned char)(255.0f * powf(RGB[0], gpTracer->Camera.InvGamma));
	Result[1] = (unsigned char)(255.0f * powf(RGB[1], gpTracer->Camera.InvGamma));
	Result[2] = (unsigned char)(255.0f * powf(RGB[2], gpTracer->Camera.InvGamma));
	Result[3] = (unsigned char)Clamp((int)(XYZA[3] * 255.0f), 0, 255);

	return Result;
}

}
