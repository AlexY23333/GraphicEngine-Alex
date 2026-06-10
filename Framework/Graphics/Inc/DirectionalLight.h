#pragma once

#include "Color.h"

namespace SumEngine::Graphics
{
	struct DirectionalLight
	{
		Color ambient = Colors::White;	// any light color without light contact
		Color diffuse = Colors::White;	// base light color
		Color specular = Colors::White;	// highlight light color
		Math::Vector3 direction = Math::Vector3::ZAxis;
		float padding = 0.0f;	// padding to make it 16 bytes aligned
	};
}