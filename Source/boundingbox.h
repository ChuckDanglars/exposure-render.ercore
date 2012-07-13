/*
*	@file
*	@author  Thomas Kroes <t.kroes at tudelft.nl>
*	@version 1.0
*	
*	@section LICENSE
*	
*	Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:
*	
*	Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.
*	Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.
*	Neither the name of the TU Delft nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.
*
*	THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#pragma once

#include "vector.h"

namespace ExposureRender
{

class EXPOSURE_RENDER_DLL BoundingBox
{
public:
#ifndef NO_CONSTRUCTORS
	HOST_DEVICE BoundingBox() :
		MinP(FLT_MAX),
		MaxP(FLT_MIN),
		Size(0.0f),
		InvSize(0.0f)
	{
	}

	HOST_DEVICE BoundingBox(const Vec3f& MinP, const Vec3f& MaxP) :
		MinP(MinP),
		MaxP(MaxP),
		Size(MaxP - MinP),
		InvSize(1.0f / Size)
	{
	}
#endif
	
	HOST_DEVICE BoundingBox& BoundingBox::operator = (const BoundingBox& Other)
	{
		this->MinP		= Other.MinP;	
		this->MaxP		= Other.MaxP;
		this->Size		= Other.Size;
		this->InvSize	= Other.InvSize;

		return *this;
	}

	HOST_DEVICE void SetMinP(const Vec3f& MinP)
	{
		this->MinP = MinP;
		this->Update();
	}

	HOST_DEVICE void SetMaxP(const Vec3f& MaxP)
	{
		this->MaxP = MaxP;
		this->Update();
	}

	HOST_DEVICE void Update()
	{
		this->Size		= this->MaxP - this->MinP,
		this->InvSize	= 1.0f / Size;
	}

	HOST_DEVICE bool Intersect(const Ray& R, float& T0, float& T1) const
	{
		const Vec3f InvR		= Vec3f(1.0f, 1.0f, 1.0f) / R.D;
		const Vec3f BottomT		= InvR * (this->MinP - R.O);
		const Vec3f TopT		= InvR * (this->MaxP - R.O);
		const Vec3f MinT		= TopT.Min(BottomT);
		const Vec3f MaxT		= TopT.Max(BottomT);
		const float LargestMinT = max(max(MinT[0], MinT[1]), max(MinT[0], MinT[2]));
		const float LargestMaxT = min(min(MaxT[0], MaxT[1]), min(MaxT[0], MaxT[2]));

		if (LargestMaxT < LargestMinT)
			return false;

		T0 = LargestMinT > 0.0f ? LargestMinT : 0.0f;
		T1 = LargestMaxT;

		T0 = max(T0, R.MinT);
		T1 = min(T1, R.MaxT);

		return true;
	}

	Vec3f	MinP;
	Vec3f	MaxP;
	Vec3f	Size;
	Vec3f	InvSize;
};

}