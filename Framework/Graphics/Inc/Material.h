#pragma once

#include "Color.h"

namespace SumEngine::Graphics
{
	struct Material
	{
		Color emissive = Colors::Black; // color without any light 4x
		Color ambient = Colors::White; // color with no light contact 4x
		Color diffuse = Colors::White; // base color 4x
		Color specular = Colors::White; // hightlight color 4x
		float shininess = 10.0f; // size of the spot when it shines 1x

		float padding[3] = { 0.0f }; // 3x

	};
}																												