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

#include "piecewiselinearfunction.h"
#include "color.h"
#include "timestamp.h"

namespace ExposureRender
{

class EXPOSURE_RENDER_DLL ColorNode
{
public:
	HOST ColorNode()
	{
	}

	HOST ColorNode(const ColorNode& Other)
	{
		*this = Other;
	}

	HOST ColorNode& operator = (const ColorNode& Other)
	{
		for (int i = 0; i < 3; i++)
			this->ScalarNodes[i] = Other.ScalarNodes[i];

		return *this;
	}
	
	HOST static ColorNode FromRGB(const float& Position, const ColorRGBf& RGB)
	{
		ColorXYZf XYZ = ColorXYZf::FromRGBf(RGB.D);

		XYZ.Clamp(0.0f, 1.0f);

		return ColorNode::FromXYZ(Position, XYZ);
	}
	
	HOST static ColorNode FromXYZ(const float& Position, const ColorXYZf& XYZ)
	{
		ColorNode Result;

		Result.ScalarNodes[0].Position	= Position;
		Result.ScalarNodes[0].Value		= XYZ[0];
		Result.ScalarNodes[1].Position	= Position;
		Result.ScalarNodes[1].Value		= XYZ[1];
		Result.ScalarNodes[2].Position	= Position;
		Result.ScalarNodes[2].Value		= XYZ[2];

		return Result;
	}

	ScalarNode	ScalarNodes[3];
};

class EXPOSURE_RENDER_DLL ColorTransferFunction1D
{
public:
	HOST ColorTransferFunction1D() :
		Texture()
	{
	}

	HOST ColorTransferFunction1D(const ColorTransferFunction1D& Other)
	{
		*this = Other;
	}

	HOST ColorTransferFunction1D& operator = (const ColorTransferFunction1D& Other)
	{
		if (*this != Other)
		{
			Buffer1D<ColorXYZf> Samples("Samples", Enums::Device);

			this->Discretize(512, Samples.GetData());

			printf("Rebuilding color transfer function\n");
		}

		

		for (int i = 0; i < 3; i++)
			this->PLF[i] = Other.PLF[i];
		
		return *this;
	}

	HOST void AddNode(const ColorNode& Node)
	{
		for (int i = 0; i < 3; i++)
			this->PLF[i].AddNode(Node.ScalarNodes[i].Position, Node.ScalarNodes[i].Value);
	}

	HOST void Reset()
	{
		this->PLF[0].Reset();
		this->PLF[1].Reset();
		this->PLF[2].Reset();
	}

	HOST_DEVICE ColorXYZf Evaluate(const float& Intensity) const
	{
		return ColorXYZf(this->PLF[0].Evaluate(Intensity), this->PLF[1].Evaluate(Intensity), this->PLF[2].Evaluate(Intensity));
	}

	HOST void Discretize(const int& NoSamples, ColorXYZf* Samples)
	{
		if (NoSamples <= 0)
			throw (Exception(Enums::Error, "Can't discretize scalar transfer function with zero samples!"));
	
		Samples = new ColorXYZf[NoSamples];

		for (int i = 0; i < NoSamples; ++i)
		{
		}

		delete[] Samples;
	}

	PiecewiseLinearFunction<MAX_NO_TF_NODES>	PLF[3];
	CudaTexture1D<ColorXYZf>					Texture;
};

}