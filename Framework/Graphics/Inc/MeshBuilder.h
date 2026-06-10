#pragma once

#include "MeshTypes.h"

namespace SumEngine::Graphics
{
	class MeshBuilder
	{
	public:
		// Cube
		static Mesh CreateCube(float size);
		static MeshPC CreateCubePC(float size);
		static MeshPX CreateCubePX(float size);

		// SkyStuff
		static MeshPX CreateSkyboxPX(float size);
		static MeshPX CreateSkySpherePX(int slices, int rings, float radius);

		// Rectangle
		static MeshPC CreateRectPC(float width, float length, float height);
	
		// Plane
		static MeshPC CreatePlanePC(int numRows, int numCols, float spacing);
		static MeshPX CreatePlanePX(int numRows, int numCols, float spacing);
		static Mesh CreateGroundPlane(int numRows, int numCols, float spacing);

		// Cylinder
		static MeshPC CreateCylinderPC(int slices, int rings);

		// Sphere
		static MeshPC CreateSpherePC(int slices, int rings, float radius);
		static MeshPX CreateSpherePX(int slices, int rings, float radius);
		static Mesh CreateSphere(int slices, int rings, float radius);

		// Screen Quad
		static MeshPX CreateScreenQuad();

		// Sprite Quad
		static MeshPX CreateSpriteQuad(float width, float height);
	};
}