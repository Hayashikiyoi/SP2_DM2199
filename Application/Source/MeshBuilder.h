#ifndef MESH_BUILDER_H
#define MESH_BUILDER_H

#include "Mesh.h"
#include "Vertex.h"
#include "LoadOBJ.h"

/******************************************************************************/
/*!
		Class MeshBuilder:
\brief	Provides methods to generate mesh of different shapes
*/
/******************************************************************************/
class MeshBuilder
{
public:
	static Mesh* GenerateAxes(const std::string &meshName, float lengthX, float lengthY, float lengthZ);
	static Mesh* GenerateQuad(const std::string &meshName, Color color, float lengthX = 10.f, float lengthY = 10.f);
	static Mesh* GenerateTriangle(const std::string &meshName, Color color, float lengthX, float lengthY);
	static Mesh* GenerateCube(const std::string &meshName, Color color, float lengthX, float lengthY, float lengthZ);

	static Mesh* GenerateCircle(const std::string &meshName, Color color, unsigned numSlices = 10, float radius = 1.f);
	static Mesh* GenerateRing(const std::string &meshName, Color color, unsigned numSlices = 10, float radius = 1.f, float innerRadius = 1.f);
	static Mesh* GenerateHemiSphere(const std::string &meshName, Color color, unsigned numSlices = 10, unsigned numStack = 10, float radius = 1.f);
	static Mesh* GenerateSphere(const std::string &meshName, Color color, unsigned numSlice = 10, unsigned numStack = 10, float radius = 1.f);
	static Mesh* GenerateCylinder(const std::string &meshName, Color color, unsigned numSlice = 10, unsigned height = 10, float radius = 1.f);
	static Mesh* GenerateStarCuboid(const std::string &meshName, Color color, unsigned numSlice = 10, unsigned height = 10, float radius = 1.f);
	static Mesh* GenerateHalfCylinder(const std::string &meshName, Color color, unsigned numSlice = 10, unsigned height = 10, float radius = 1.f);
	static Mesh* GenerateTorus(const std::string &meshName, Color color, unsigned numSlices = 12, unsigned numStacks = 12, float outerR = 10.f, float innerR = .5f);
	static Mesh* GenerateCone(const std::string &meshName, Color color, unsigned numSlices = 10, float radius = 1.0f, float Height = 10);
	static Mesh* GenerateSTorus(const std::string &meshName, Color color,Color color2, unsigned numSlices = 12, unsigned numStacks = 12, float outerR = 10.f, float innerR = .5f);
	static Mesh* GenerateOBJ(const std::string &meshName, const std::string &file_path);
	static Mesh* MeshBuilder::GenerateText(const std::string &meshName, unsigned numRow, unsigned numCol);
};

#endif