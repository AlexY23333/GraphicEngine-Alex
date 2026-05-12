#pragma once

namespace SumEngine
{
	enum class ComponentId
	{
		Invalid,		// default value, not valid
		Transform,		// Transform component for location data
		Camera,			// contains the camera for viewing
		FPSCamera,		// move the camera with fps controls
		Mesh,			// adds a mesh shape to the game object
		Count			// last value, chain ids in custom components
	};

	enum class ServiceId
	{
		Invalid,		// default value, not valid
		Camera,			// manages the cameras in the world
		Render,			// renders renderobjects to the world
		Count			// last value, chain ids in custom services
	};
}

#define SET_TYPE_ID(id)\
	static uint32_t StaticGetTypeId() { return static_cast<uint32_t>(id); }\
	uint32_t GetTypeId() const override { return StaticGetTypeId(); }