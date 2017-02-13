#include "MeshBuilder.h"
#include <GL\glew.h>
#include <vector>
#include "MyMath.h"


/******************************************************************************/
/*!
\brief
Generate the vertices of a reference Axes; Use red for x-axis, green for y-axis, blue for z-axis
Then generate the VBO/IBO and store them in Mesh object

\param meshName - name of mesh
\param lengthX - x-axis should start at -lengthX / 2 and end at lengthX / 2
\param lengthY - y-axis should start at -lengthY / 2 and end at lengthY / 2
\param lengthZ - z-axis should start at -lengthZ / 2 and end at lengthZ / 2

\return Pointer to mesh storing VBO/IBO of reference axes
*/
/******************************************************************************/
Mesh* MeshBuilder::GenerateAxes(const std::string &meshName, float lengthX, float lengthY, float lengthZ)
{
	Vertex v;
	std::vector<Vertex>  vertex_buffer_data;
	std::vector<unsigned>  index_buffer_data;
	v.pos.Set(-1000, 0, 0); v.color.Set(1, 0, 0); vertex_buffer_data.push_back(v);
	v.pos.Set(1000, 0, 0); v.color.Set(1, 0, 0); vertex_buffer_data.push_back(v);

	v.pos.Set(0, -1000, 0); v.color.Set(0, 1, 0); vertex_buffer_data.push_back(v);
	v.pos.Set(0, 1000, 0); v.color.Set(0, 1, 0); vertex_buffer_data.push_back(v);

	v.pos.Set(0, 0, -1000); v.color.Set(0, 0, 1); vertex_buffer_data.push_back(v);
	v.pos.Set(0, 0, 1000); v.color.Set(0, 0, 1); vertex_buffer_data.push_back(v);

	Mesh *mesh = new Mesh(meshName);

	for (unsigned i = 0; i < 6; ++i)
		index_buffer_data.push_back(i);

	glBindBuffer(GL_ARRAY_BUFFER, mesh->vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, vertex_buffer_data.size()*sizeof(Vertex), &vertex_buffer_data[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->indexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, index_buffer_data.size()*sizeof(unsigned), &index_buffer_data[0], GL_STATIC_DRAW);

	mesh->indexSize = index_buffer_data.size();
	mesh->mode = Mesh::DRAW_LINES;
	return mesh;
}

/******************************************************************************/
/*!
\brief
Generate the vertices of a quad; Use random color for each vertex
Then generate the VBO/IBO and store them in Mesh object

\param meshName - name of mesh
\param lengthX - width of quad
\param lengthY - height of quad

\return Pointer to mesh storing VBO/IBO of quad
*/
/******************************************************************************/
Mesh* MeshBuilder::GenerateQuad(const std::string &meshName, Color color, float lengthX, float lengthY)
{
	Vertex v;
	std::vector<Vertex>  vertex_buffer_data;
	std::vector<unsigned>  index_buffer_data;

	v.pos.Set(lengthX* 0.5f, lengthY*  0.5f, 0); v.color = color; v.normal.Set(0, 0, -1); v.texCoord.Set(1, 1); vertex_buffer_data.push_back(v);
	v.pos.Set(lengthX* -0.5f, lengthY* 0.5f, 0); v.color = color; color; v.normal.Set(0, 0, -1); v.texCoord.Set(0, 1);  vertex_buffer_data.push_back(v);
	v.pos.Set(lengthX* 0.5f, lengthY* -0.5f, 0); v.color = color; color; v.normal.Set(0, 0, -1); v.texCoord.Set(1, 0);  vertex_buffer_data.push_back(v);
	v.pos.Set(lengthX* -0.5f, lengthY* -0.5f, 0); v.color = color; color; v.normal.Set(0, 0, -1); v.texCoord.Set(0, 0);  vertex_buffer_data.push_back(v);
	
	for (unsigned i = 0; i < 4; ++i)
		index_buffer_data.push_back(i);

	Mesh *mesh = new Mesh(meshName);

	glBindBuffer(GL_ARRAY_BUFFER, mesh->vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, vertex_buffer_data.size()*sizeof(Vertex), &vertex_buffer_data[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->indexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, index_buffer_data.size()*sizeof(unsigned), &index_buffer_data[0], GL_STATIC_DRAW);

	mesh->indexSize = index_buffer_data.size();
	mesh->mode = Mesh::DRAW_TRIANGLE_STRIP;

	return mesh;
}

Mesh* MeshBuilder::GenerateTriangle(const std::string &meshName, Color color, float lengthX, float lengthY)
{
	Vertex v;
	std::vector<Vertex>  vertex_buffer_data;
	std::vector<unsigned>  index_buffer_data;

	v.pos.Set(lengthX * 0.5f, lengthY * -0.5f, 0); v.color = color; color; v.normal.Set(0, 0, 1);  vertex_buffer_data.push_back(v);
	v.pos.Set(lengthX * -0.5f, lengthY * -0.5f, 0); v.color = color; color; v.normal.Set(0, 0, 1);  vertex_buffer_data.push_back(v);
	v.pos.Set(0.0f, lengthY * 0.5f, 0); v.color = color; v.normal.Set(0, 0, 1); vertex_buffer_data.push_back(v);

	for (unsigned i = 0; i < 3; ++i)
		index_buffer_data.push_back(i);

	Mesh *mesh = new Mesh(meshName);

	glBindBuffer(GL_ARRAY_BUFFER, mesh->vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, vertex_buffer_data.size()*sizeof(Vertex), &vertex_buffer_data[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->indexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, index_buffer_data.size()*sizeof(unsigned), &index_buffer_data[0], GL_STATIC_DRAW);

	mesh->indexSize = index_buffer_data.size();
	mesh->mode = Mesh::DRAW_TRIANGLE_STRIP;

	return mesh;
}

/******************************************************************************/
/*!
\brief
Generate the vertices of a cube; Use random color for each vertex
Then generate the VBO/IBO and store them in Mesh object

\param meshName - name of mesh
\param lengthX - width of cube
\param lengthY - height of cube
\param lengthZ - depth of cube

\return Pointer to mesh storing VBO/IBO of cube
*/
/******************************************************************************/
Mesh* MeshBuilder::GenerateCube(const std::string &meshName, Color color, float lengthX, float lengthY, float lengthZ)
{
	Vertex v;
	Vertex x;
	std::vector<Vertex>  vertex_buffer_data;
	std::vector<unsigned>  index_buffer_data;

	x.normal.Set(0, 0, -1);
	//Front Square
	v.pos.Set(-lengthX / 2, lengthY / 2, lengthZ / 2); v.color = color; v.normal = x.normal; vertex_buffer_data.push_back(v);
	v.pos.Set(-lengthX / 2, -lengthY / 2, lengthZ / 2); v.color = color; v.normal = x.normal; vertex_buffer_data.push_back(v);
	v.pos.Set(lengthX / 2, lengthY / 2, lengthZ / 2); v.color = color; v.normal = x.normal; vertex_buffer_data.push_back(v);
	v.pos.Set(lengthX / 2, lengthY / 2, lengthZ / 2); v.color = color; v.normal = x.normal; vertex_buffer_data.push_back(v);
	v.pos.Set(-lengthX / 2, -lengthY / 2, lengthZ / 2); v.color = color; v.normal = x.normal; vertex_buffer_data.push_back(v);
	v.pos.Set(lengthX / 2, -lengthY / 2, lengthZ / 2); v.color = color; v.normal = x.normal; vertex_buffer_data.push_back(v);

	x.normal.Set(-1, 0, 0);
	//Right square
	v.pos.Set(lengthX / 2, lengthY / 2, lengthZ / 2); v.color = color; v.normal = x.normal; vertex_buffer_data.push_back(v);
	v.pos.Set(lengthX / 2, -lengthY / 2, lengthZ / 2); v.color = color; v.normal = x.normal; vertex_buffer_data.push_back(v);
	v.pos.Set(lengthX / 2, lengthY / 2, -lengthZ / 2); v.color = color; v.normal = x.normal; vertex_buffer_data.push_back(v);
	v.pos.Set(lengthX / 2, lengthY / 2, -lengthZ / 2); v.color = color; v.normal = x.normal; vertex_buffer_data.push_back(v);
	v.pos.Set(lengthX / 2, -lengthY / 2, lengthZ / 2); v.color = color; v.normal = x.normal; vertex_buffer_data.push_back(v);
	v.pos.Set(lengthX / 2, -lengthY / 2, -lengthZ / 2); v.color = color; v.normal = x.normal; vertex_buffer_data.push_back(v);

	x.normal.Set(0, -1, 0);
	//top square
	v.pos.Set(-lengthX / 2, lengthY / 2, -lengthZ / 2); v.color = color; v.normal = x.normal; vertex_buffer_data.push_back(v);
	v.pos.Set(-lengthX / 2, lengthY / 2, lengthZ / 2); v.color = color; v.normal = x.normal; vertex_buffer_data.push_back(v);
	v.pos.Set(lengthX / 2, lengthY / 2, -lengthZ / 2); v.color = color; v.normal = x.normal; vertex_buffer_data.push_back(v);
	v.pos.Set(lengthX / 2, lengthY / 2, -lengthZ / 2); v.color = color; v.normal = x.normal; vertex_buffer_data.push_back(v);
	v.pos.Set(-lengthX / 2, lengthY / 2, lengthZ / 2); v.color = color; v.normal = x.normal; vertex_buffer_data.push_back(v);
	v.pos.Set(lengthX / 2, lengthY / 2, lengthZ / 2); v.color = color; v.normal = x.normal; vertex_buffer_data.push_back(v);

	x.normal.Set(0, 1, 0);
	//Bottom square
	v.pos.Set(lengthX / 2, -lengthY / 2, lengthZ / 2); v.color = color; v.normal = x.normal; vertex_buffer_data.push_back(v);
	v.pos.Set(-lengthX / 2, -lengthY / 2, lengthZ / 2); v.color = color; v.normal = x.normal; vertex_buffer_data.push_back(v);
	v.pos.Set(-lengthX / 2, -lengthY / 2, -lengthZ / 2); v.color = color; v.normal = x.normal; vertex_buffer_data.push_back(v);
	v.pos.Set(lengthX / 2, -lengthY / 2, lengthZ / 2); v.color = color; v.normal = x.normal; vertex_buffer_data.push_back(v);
	v.pos.Set(-lengthX / 2, -lengthY / 2, -lengthZ / 2); v.color = color; v.normal = x.normal; vertex_buffer_data.push_back(v);
	v.pos.Set(lengthX / 2, -lengthY / 2, -lengthZ / 2); v.color = color; v.normal = x.normal; vertex_buffer_data.push_back(v);

	x.normal.Set(0, 0, -1);
	//Back Square
	v.pos.Set(-lengthX / 2, lengthY / 2, -lengthZ / 2); v.color = color; v.normal = x.normal; vertex_buffer_data.push_back(v);
	v.pos.Set(lengthX / 2, lengthY / 2, -lengthZ / 2); v.color = color; v.normal = x.normal; vertex_buffer_data.push_back(v);
	v.pos.Set(lengthX / 2, -lengthY / 2, -lengthZ / 2); v.color = color; v.normal = x.normal; vertex_buffer_data.push_back(v);
	v.pos.Set(-lengthX / 2, lengthY / 2, -lengthZ / 2); v.color = color; v.normal = x.normal; vertex_buffer_data.push_back(v);
	v.pos.Set(lengthX / 2, -lengthY / 2, -lengthZ / 2); v.color = color; v.normal = x.normal; vertex_buffer_data.push_back(v);
	v.pos.Set(-lengthX / 2, -lengthY / 2, -lengthZ / 2); v.color = color; v.normal = x.normal; vertex_buffer_data.push_back(v);

	x.normal.Set(1, 0, 0);
	//Left square
	v.pos.Set(-lengthX / 2, lengthY / 2, lengthZ / 2); v.color = color; v.normal = x.normal; vertex_buffer_data.push_back(v);
	v.pos.Set(-lengthX / 2, lengthY / 2, -lengthZ / 2); v.color = color; v.normal = x.normal; vertex_buffer_data.push_back(v);
	v.pos.Set(-lengthX / 2, -lengthY / 2, -lengthZ / 2); v.color = color; v.normal = x.normal; vertex_buffer_data.push_back(v);
	v.pos.Set(-lengthX / 2, lengthY / 2, lengthZ / 2); v.color = color; v.normal = x.normal; vertex_buffer_data.push_back(v);
	v.pos.Set(-lengthX / 2, -lengthY / 2, -lengthZ / 2); v.color = color; v.normal = x.normal; vertex_buffer_data.push_back(v);
	v.pos.Set(-lengthX / 2, -lengthY / 2, lengthZ / 2); v.color = color; v.normal = x.normal; vertex_buffer_data.push_back(v);
	
	for (unsigned i = 0; i < 36; ++i)
		index_buffer_data.push_back(i);

	Mesh *mesh = new Mesh(meshName);
	
	glBindBuffer(GL_ARRAY_BUFFER, mesh->vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, vertex_buffer_data.size()*sizeof(Vertex), &vertex_buffer_data[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->indexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, index_buffer_data.size()*sizeof(unsigned), &index_buffer_data[0], GL_STATIC_DRAW);
	
	mesh->indexSize = index_buffer_data.size();

	return mesh;
}

Mesh* MeshBuilder::GenerateCircle(const std::string &meshName, Color color, unsigned numSlices, float radius)
{
	Vertex v;
	std::vector<Vertex>  vertex_buffer_data;
	std::vector<unsigned>  index_buffer_data;
	
	float anglePerSlice = 360.f / numSlices;
	for (int slice = 0; slice < numSlices + 1; ++slice)
	{
		float theta = slice * anglePerSlice;
		v.pos.Set(radius * cos(Math::DegreeToRadian(slice * anglePerSlice)), 0,
			radius * sin(Math::DegreeToRadian(theta)));
		v.color = color;
		vertex_buffer_data.push_back(v);
	}

	v.pos.Set(0, 0, 0);
	v.color = color;
	vertex_buffer_data.push_back(v);

	for (int i = 0; i < numSlices + 1; ++i)
	{
		index_buffer_data.push_back(i);
		index_buffer_data.push_back(numSlices + 1);
	}

	Mesh *mesh = new Mesh(meshName);

	glBindBuffer(GL_ARRAY_BUFFER, mesh->vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, vertex_buffer_data.size()*sizeof(Vertex), &vertex_buffer_data[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->indexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, index_buffer_data.size()*sizeof(unsigned), &index_buffer_data[0], GL_STATIC_DRAW);

	mesh->indexSize = index_buffer_data.size();
	mesh->mode = Mesh::DRAW_TRIANGLE_STRIP;

	return mesh;
}

Mesh* MeshBuilder::GenerateRing(const std::string &meshName, Color color, unsigned numSlices, float radius, float innerRadius)
{
	Vertex v;
	std::vector<Vertex>  vertex_buffer_data;
	std::vector<unsigned>  index_buffer_data;

	float anglePerSlice = 360.f / numSlices;
	for (int slice = 0; slice <= numSlices; ++slice)
	{
		float theta = slice * anglePerSlice;
		v.pos.Set(radius * cos(Math::DegreeToRadian(slice * anglePerSlice)), 0,
			radius * sin(Math::DegreeToRadian(theta)));
		v.color = color;
		vertex_buffer_data.push_back(v);

		v.pos.Set(innerRadius * cos(Math::DegreeToRadian(slice * anglePerSlice)), 0,
			innerRadius * sin(Math::DegreeToRadian(theta)));
		v.color = color;
		vertex_buffer_data.push_back(v);
	}

	Mesh *mesh = new Mesh(meshName);

	for (unsigned i = 0; i < (numSlices + 1) * 2; ++i)
		index_buffer_data.push_back(i);

	glBindBuffer(GL_ARRAY_BUFFER, mesh->vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, vertex_buffer_data.size()*sizeof(Vertex), &vertex_buffer_data[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->indexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, index_buffer_data.size()*sizeof(unsigned), &index_buffer_data[0], GL_STATIC_DRAW);

	mesh->indexSize = index_buffer_data.size();
	mesh->mode = Mesh::DRAW_TRIANGLE_STRIP;

	return mesh;
}

Mesh* MeshBuilder::GenerateHemiSphere(const std::string &meshName, Color color, unsigned numSlices, unsigned numStack, float radius)
{
	Vertex v;
	std::vector<Vertex>  vertex_buffer_data;
	std::vector<unsigned>  index_buffer_data;

	float anglePerSlice = 360.0f / numSlices;
	float anglePerStack = 90.0f / numStack;

	v.pos.Set(0, 0, 0);
	v.color = color;
	v.normal.Set(0, 1, 0);
	vertex_buffer_data.push_back(v);

	for (unsigned stack = 0; stack < numStack + 1; ++stack)
	{
		float phi = stack * anglePerStack;

		for (unsigned slice = 0; slice < numSlices + 1; ++slice)
		{
			float theta = slice * anglePerSlice;
			v.pos.Set(radius *cos(Math::DegreeToRadian(phi))*cos(Math::DegreeToRadian(theta)),
				radius * sin(Math::DegreeToRadian(phi)),
				radius * cos(Math::DegreeToRadian(phi))* sin(Math::DegreeToRadian(theta)));

			v.color = color;
			v.normal.Set(cos(Math::DegreeToRadian(phi))*cos(Math::DegreeToRadian(theta)),
				sin(Math::DegreeToRadian(phi)),
				cos(Math::DegreeToRadian(phi))* sin(Math::DegreeToRadian(theta)));

			vertex_buffer_data.push_back(v);
		}
	}

	for (unsigned stack = 0; stack < numStack; ++stack)
	{
		for (unsigned slice = 0; slice < (numSlices + 1); ++slice)
		{
			index_buffer_data.push_back(stack * (numSlices + 1) + slice);
			index_buffer_data.push_back((stack + 1) * (numSlices + 1) + slice);
		}
	}

	for (int slice = 0; slice <= numSlices; ++slice)
	{
		float theta = slice * anglePerSlice;
		v.pos.Set(radius * cos(Math::DegreeToRadian(slice * anglePerSlice)), 0,
			radius * sin(Math::DegreeToRadian(theta)));
		v.color = color;
		v.normal.Set(cos(Math::DegreeToRadian(slice * anglePerSlice)), 0,
			sin(Math::DegreeToRadian(theta)));
		vertex_buffer_data.push_back(v);
	}
	for (int slice = 0; slice < numSlices + 1; ++slice)
	{
		index_buffer_data.push_back(0);
		index_buffer_data.push_back( slice + (numSlices + 2));

	}

	Mesh *mesh = new Mesh(meshName);


	glBindBuffer(GL_ARRAY_BUFFER, mesh->vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, vertex_buffer_data.size()*sizeof(Vertex), &vertex_buffer_data[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->indexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, index_buffer_data.size()*sizeof(unsigned), &index_buffer_data[0], GL_STATIC_DRAW);

	mesh->indexSize = index_buffer_data.size();
	mesh->mode = Mesh::DRAW_TRIANGLE_STRIP;

	return mesh;
}

Mesh* MeshBuilder::GenerateSphere(const std::string &meshName, Color color, unsigned numSlice, unsigned numStack, float radius)
{
	Vertex v;
	std::vector<Vertex>  vertex_buffer_data;
	std::vector<unsigned>  index_buffer_data;

	float anglePerSlice = 360.0f / numSlice;
	float anglePerStack = 180.0f / numStack;

	for (unsigned stack = 0; stack < numStack + 1; ++stack)
	{
		float phi = -90.f + stack * anglePerStack;
		
		for (unsigned slice = 0; slice < numSlice + 1; ++slice)
		{
			float theta = slice * anglePerSlice;
			v.pos.Set(radius *cos(Math::DegreeToRadian(phi))*cos(Math::DegreeToRadian(theta)),
				radius * sin(Math::DegreeToRadian(phi)),
				radius * cos(Math::DegreeToRadian(phi))* sin(Math::DegreeToRadian(theta)));
			v.color = color;
			v.normal.Set(cos(Math::DegreeToRadian(phi))*cos(Math::DegreeToRadian(theta)),
				sin(Math::DegreeToRadian(phi)),
				cos(Math::DegreeToRadian(phi))* sin(Math::DegreeToRadian(theta)));
			vertex_buffer_data.push_back(v);
		}
	}
	

	for (unsigned stack = 0; stack < numStack; ++stack)
	{
		for (unsigned slice = 0; slice < numSlice + 1; ++slice)
		{
			index_buffer_data.push_back(stack * (numSlice + 1) + slice);
			index_buffer_data.push_back((stack + 1) * (numSlice + 1) + slice);

		}
	}

	Mesh *mesh = new Mesh(meshName);

	glBindBuffer(GL_ARRAY_BUFFER, mesh->vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, vertex_buffer_data.size()*sizeof(Vertex), &vertex_buffer_data[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->indexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, index_buffer_data.size()*sizeof(unsigned), &index_buffer_data[0], GL_STATIC_DRAW);

	mesh->indexSize = index_buffer_data.size();
	mesh->mode = Mesh::DRAW_TRIANGLE_STRIP;

	return mesh;
}

Mesh* MeshBuilder::GenerateCylinder(const std::string &meshName, Color color, unsigned numSlice, unsigned height, float radius)
{
	Vertex v;
	std::vector<Vertex>  vertex_buffer_data;
	std::vector<unsigned>  index_buffer_data;

	float anglePerSlice = 360.0f / numSlice;

	//Bottom circle
	for (unsigned slice = 0; slice < numSlice + 1; ++slice)
	{
		float theta = slice * anglePerSlice;
		v.pos.Set(radius *cos(Math::DegreeToRadian(theta)),-((float)height * 0.5f),radius * sin(Math::DegreeToRadian(theta)));
		v.color = color;
		v.normal.Set(cos(Math::DegreeToRadian(theta)), -1, sin(Math::DegreeToRadian(theta)));
		vertex_buffer_data.push_back(v);
	}

	//Set mid point for the two circles in the end
	v.pos.Set(0, -((float)height / 2), 0);//Bottom mid point
	v.color = color;
	v.pos.Set(0, -1, 0);
	vertex_buffer_data.push_back(v);

	//Curve Side
	for (unsigned slice = 0; slice < numSlice + 1; ++slice)
	{
		float theta = slice * anglePerSlice;
		v.pos.Set(radius *cos(Math::DegreeToRadian(theta)), -((float)height * 0.5f), radius * sin(Math::DegreeToRadian(theta)));
		v.color = color;
		v.normal.Set(cos(Math::DegreeToRadian(theta)), -1, sin(Math::DegreeToRadian(theta)));
		vertex_buffer_data.push_back(v);

		
		v.pos.Set(radius *cos(Math::DegreeToRadian(theta)), ((float)height * 0.5f), radius * sin(Math::DegreeToRadian(theta)));
		v.color = color;
		v.normal.Set(cos(Math::DegreeToRadian(theta)), 1, sin(Math::DegreeToRadian(theta)));
		vertex_buffer_data.push_back(v);
	}

	//Top circle
	for (unsigned slice = 0; slice < numSlice + 1; ++slice)
	{
		float theta = slice * anglePerSlice;
		v.pos.Set(radius *cos(Math::DegreeToRadian(theta)), ((float)height * 0.5f), radius * sin(Math::DegreeToRadian(theta)));
		v.color = color;
		v.normal.Set(cos(Math::DegreeToRadian(theta)), 1, sin(Math::DegreeToRadian(theta)));
		vertex_buffer_data.push_back(v);
	}

	v.pos.Set(0, 1, 0);//Top mid point
	v.color = color;
	vertex_buffer_data.push_back(v);

	for (unsigned i = 0; i < numSlice + 1; ++i)
	{
		index_buffer_data.push_back(numSlice + 1);
		index_buffer_data.push_back(i);
	}

	for (unsigned i = 0; i < numSlice + 1; ++i)
	{
		index_buffer_data.push_back(numSlice + 1 + i * 2 + 1);
		index_buffer_data.push_back(numSlice + 1 + i * 2 + 2);
	}
	for (unsigned i = 0; i < numSlice + 1; ++i)
	{
		index_buffer_data.push_back(numSlice + 1 + (numSlice + 1) * 2 + i + 1);
		index_buffer_data.push_back(numSlice + 1 + (numSlice + 1) * 2 + numSlice + 2);
	}

	Mesh *mesh = new Mesh(meshName);

	glBindBuffer(GL_ARRAY_BUFFER, mesh->vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, vertex_buffer_data.size()*sizeof(Vertex), &vertex_buffer_data[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->indexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, index_buffer_data.size()*sizeof(unsigned), &index_buffer_data[0], GL_STATIC_DRAW);

	mesh->indexSize = index_buffer_data.size();
	mesh->mode = Mesh::DRAW_TRIANGLE_STRIP;
	return mesh;
}

Mesh* MeshBuilder::GenerateHalfCylinder(const std::string &meshName, Color color, unsigned numSlice, unsigned height, float radius)
{
	Vertex v;
	std::vector<Vertex>  vertex_buffer_data;
	std::vector<unsigned>  index_buffer_data;

	float anglePerSlice = 180.0f / numSlice;

	//Bottom circle
	for (unsigned slice = 0; slice < numSlice + 1; ++slice)
	{
		float theta = slice * anglePerSlice;
		v.pos.Set(radius *cos(Math::DegreeToRadian(theta)), -((float)height * 0.5f), radius * sin(Math::DegreeToRadian(theta)));
		v.color = color;
		v.normal.Set(cos(Math::DegreeToRadian(theta)), -1, sin(Math::DegreeToRadian(theta)));
		vertex_buffer_data.push_back(v);
	}

	//Set mid point for the two circles in the end
	v.pos.Set(0, -((float)height / 2), 0);//Bottom mid point
	v.color = color;
	v.pos.Set(0, -1, 0);
	vertex_buffer_data.push_back(v);

	//Curve Side
	for (unsigned slice = 0; slice < numSlice + 1; ++slice)
	{
		float theta = slice * anglePerSlice;
		v.pos.Set(radius *cos(Math::DegreeToRadian(theta)), -((float)height * 0.5f), radius * sin(Math::DegreeToRadian(theta)));
		v.color = color;
		v.normal.Set(cos(Math::DegreeToRadian(theta)), -1, sin(Math::DegreeToRadian(theta)));
		vertex_buffer_data.push_back(v);


		v.pos.Set(radius *cos(Math::DegreeToRadian(theta)), ((float)height * 0.5f), radius * sin(Math::DegreeToRadian(theta)));
		v.color = color;
		v.normal.Set(cos(Math::DegreeToRadian(theta)), 1, sin(Math::DegreeToRadian(theta)));
		vertex_buffer_data.push_back(v);
	}

	//Top circle
	for (unsigned slice = 0; slice < numSlice + 1; ++slice)
	{
		float theta = slice * anglePerSlice;
		v.pos.Set(radius *cos(Math::DegreeToRadian(theta)), ((float)height * 0.5f), radius * sin(Math::DegreeToRadian(theta)));
		v.color = color;
		v.normal.Set(cos(Math::DegreeToRadian(theta)), 1, sin(Math::DegreeToRadian(theta)));
		vertex_buffer_data.push_back(v);
	}

	v.pos.Set(0, 1, 0);//Top mid point
	v.color = color;
	vertex_buffer_data.push_back(v);

	for (unsigned i = 0; i < numSlice + 1; ++i)
	{
		index_buffer_data.push_back(numSlice + 1);
		index_buffer_data.push_back(i);
	}

	for (unsigned i = 0; i < numSlice + 1; ++i)
	{
		index_buffer_data.push_back(numSlice + 1 + i * 2 + 1);
		index_buffer_data.push_back(numSlice + 1 + i * 2 + 2);
	}
	for (unsigned i = 0; i < numSlice + 1; ++i)
	{
		index_buffer_data.push_back(numSlice + 1 + (numSlice + 1) * 2 + i + 1);
		index_buffer_data.push_back(numSlice + 1 + (numSlice + 1) * 2 + numSlice + 2);
	}

	Mesh *mesh = new Mesh(meshName);

	glBindBuffer(GL_ARRAY_BUFFER, mesh->vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, vertex_buffer_data.size()*sizeof(Vertex), &vertex_buffer_data[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->indexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, index_buffer_data.size()*sizeof(unsigned), &index_buffer_data[0], GL_STATIC_DRAW);

	mesh->indexSize = index_buffer_data.size();
	mesh->mode = Mesh::DRAW_TRIANGLE_STRIP;
	return mesh;
}

Mesh* MeshBuilder::GenerateStarCuboid(const std::string &meshName, Color color, unsigned numSlice, unsigned height, float radius)
{
	Vertex v;
	std::vector<Vertex>  vertex_buffer_data;
	std::vector<unsigned>  index_buffer_data;

	float anglePerSlice = 360.0f / numSlice;

	//Bottom circle
	for (unsigned slice = 0; slice < numSlice + 1; ++slice)
	{
		if (slice % 2 == 0)
		{
			float theta = slice * anglePerSlice;
			v.pos.Set((radius*0.5f) *cos(Math::DegreeToRadian(theta)), -((float)height * 0.5f), (radius*0.5f) * sin(Math::DegreeToRadian(theta)));
			v.color = color;
			v.normal.Set(cos(Math::DegreeToRadian(theta)), -1, sin(Math::DegreeToRadian(theta)));
			vertex_buffer_data.push_back(v);
		}
		else
		{
			float theta = slice * anglePerSlice;
			v.pos.Set(radius *cos(Math::DegreeToRadian(theta)), -((float)height * 0.5f), radius * sin(Math::DegreeToRadian(theta)));
			v.color = color;
			v.normal.Set(cos(Math::DegreeToRadian(theta)), -1, sin(Math::DegreeToRadian(theta)));
			vertex_buffer_data.push_back(v);
		}
		
	}

	//Set mid point for the two circles in the end
	v.pos.Set(0, -((float)height / 2), 0);//Bottom mid point
	v.color = color;
	v.normal.Set(0, -1, 0);
	vertex_buffer_data.push_back(v);

	//Curve Side
	for (unsigned slice = 0; slice < numSlice + 1; ++slice)
	{
		if (slice % 2 == 0)
		{
			float theta = slice * anglePerSlice;
			v.pos.Set((radius*0.5f) *cos(Math::DegreeToRadian(theta)), -((float)height * 0.5f), (radius*0.5f) * sin(Math::DegreeToRadian(theta)));
			v.color = color;
			v.normal.Set(cos(Math::DegreeToRadian(theta)), -1, sin(Math::DegreeToRadian(theta)));
			vertex_buffer_data.push_back(v);


			v.pos.Set((radius*0.5f) *cos(Math::DegreeToRadian(theta)), ((float)height * 0.5f), (radius*0.5f) * sin(Math::DegreeToRadian(theta)));
			v.color = color;
			v.normal.Set(cos(Math::DegreeToRadian(theta)), 1, sin(Math::DegreeToRadian(theta)));
			vertex_buffer_data.push_back(v);
		}
		else
		{
			float theta = slice * anglePerSlice;
			v.pos.Set(radius *cos(Math::DegreeToRadian(theta)), -((float)height * 0.5f), radius * sin(Math::DegreeToRadian(theta)));
			v.color = color;
			v.normal.Set(cos(Math::DegreeToRadian(theta)), -1, sin(Math::DegreeToRadian(theta)));
			vertex_buffer_data.push_back(v);


			v.pos.Set(radius *cos(Math::DegreeToRadian(theta)), ((float)height * 0.5f), radius * sin(Math::DegreeToRadian(theta)));
			v.color = color;
			v.normal.Set(cos(Math::DegreeToRadian(theta)), 1, sin(Math::DegreeToRadian(theta)));
			vertex_buffer_data.push_back(v);
		}
	}

	//Top circle
	for (unsigned slice = 0; slice < numSlice + 1; ++slice)
	{
		if (slice % 2 == 0)
		{
			float theta = slice * anglePerSlice;
			v.pos.Set((radius*0.5f) *cos(Math::DegreeToRadian(theta)), ((float)height * 0.5f), (radius*0.5f) * sin(Math::DegreeToRadian(theta)));
			v.color = color;
			v.normal.Set(cos(Math::DegreeToRadian(theta)), 1, sin(Math::DegreeToRadian(theta)));
			vertex_buffer_data.push_back(v);
		}
		else
		{
			float theta = slice * anglePerSlice;
			v.pos.Set(radius *cos(Math::DegreeToRadian(theta)), ((float)height * 0.5f), radius * sin(Math::DegreeToRadian(theta)));
			v.color = color;
			v.normal.Set(cos(Math::DegreeToRadian(theta)), 1, sin(Math::DegreeToRadian(theta)));
			vertex_buffer_data.push_back(v);
		}
		
	}

	v.pos.Set(0, ((float)height / 2), 0);//Top mid point
	v.color = color;
	v.normal.Set(0, 1, 0);
	vertex_buffer_data.push_back(v);

	for (unsigned i = 0; i < numSlice + 1; ++i)
	{
		index_buffer_data.push_back(numSlice + 1);
		index_buffer_data.push_back(i);
	}

	for (unsigned i = 0; i < numSlice + 1; ++i)
	{
		index_buffer_data.push_back(numSlice + 1 + i * 2 + 1);
		index_buffer_data.push_back(numSlice + 1 + i * 2 + 2);
	}
	for (unsigned i = 0; i < numSlice + 1; ++i)
	{
		index_buffer_data.push_back(numSlice + 1 + (numSlice + 1) * 2 + i + 1);
		index_buffer_data.push_back(numSlice + 1 + (numSlice + 1) * 2 + numSlice + 2);
	}

	Mesh *mesh = new Mesh(meshName);

	glBindBuffer(GL_ARRAY_BUFFER, mesh->vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, vertex_buffer_data.size()*sizeof(Vertex), &vertex_buffer_data[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->indexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, index_buffer_data.size()*sizeof(unsigned), &index_buffer_data[0], GL_STATIC_DRAW);

	mesh->indexSize = index_buffer_data.size();
	mesh->mode = Mesh::DRAW_TRIANGLE_STRIP;
	return mesh;
}

Mesh* MeshBuilder::GenerateTorus(const std::string &meshName, Color color, unsigned numSlice, unsigned numStack, float outerR, float innerR)
{
	Vertex v;
	std::vector<Vertex>  vertex_buffer_data;
	std::vector<unsigned>  index_buffer_data;

	float degreePerStack = 360.f / numStack;
	float degreePerSlice = 360.f / numSlice;

	float x1, z1; //y1 is always zero
	float x2, y2, z2;

	for (unsigned stack = 0; stack < numStack + 1; stack++) 
	{
		for (unsigned slice = 0; slice < numSlice + 1; slice++) 
		{

			float theta = (float)(slice * degreePerSlice);
			float phi = (float)(stack * degreePerStack);

			z1 = outerR * cos(Math::DegreeToRadian(phi));
			x1 = outerR * sin(Math::DegreeToRadian(phi));

			z2 = (outerR + innerR * cos(Math::DegreeToRadian(theta))) * cos(Math::DegreeToRadian(phi));
			y2 = innerR * sin(Math::DegreeToRadian(theta));
			x2 = (outerR + innerR * cos(Math::DegreeToRadian(theta))) * sin(Math::DegreeToRadian(phi));
			v.pos.Set(x2, y2, z2);
			v.color = color;
			v.normal.Set(x2 - x1, y2, z2 - z1);
			v.normal.Normalize();
			vertex_buffer_data.push_back(v);
		}
	}

	for (unsigned stack = 0; stack < numStack; stack++) 
	{
		for (unsigned slice = 0; slice < numSlice + 1; slice++) 
		{
			index_buffer_data.push_back((numSlice + 1) * stack + slice);
			index_buffer_data.push_back((numSlice + 1) * (stack + 1) + slice);
		}
	}
	Mesh *mesh = new Mesh(meshName);

	glBindBuffer(GL_ARRAY_BUFFER, mesh->vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, vertex_buffer_data.size()*sizeof(Vertex), &vertex_buffer_data[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->indexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, index_buffer_data.size()*sizeof(unsigned), &index_buffer_data[0], GL_STATIC_DRAW);

	mesh->indexSize = index_buffer_data.size();
	mesh->mode = Mesh::DRAW_TRIANGLE_STRIP;
	return mesh;
}
Mesh* MeshBuilder::GenerateCone(const std::string &meshName, Color color, unsigned numSlices, float radius, float Height){
	std::vector<Vertex> vertex_buffer_data;
	std::vector<unsigned> index_buffer_data;
	Vertex v;

	float anglePerSlice = 360.f / numSlices;
	float offset;
	offset = 0;

	//bottom of Cone
	for (unsigned slice = 0; slice < numSlices + 1; ++slice)
	{
		float theta = slice * anglePerSlice;
		v.pos.Set(radius*cos(Math::DegreeToRadian(theta)), -Height * 0.5f, radius*sin(Math::DegreeToRadian(theta)));
		v.color = color;
		v.normal.Set(0, -1, 0);
		vertex_buffer_data.push_back(v);
	}
	v.pos.Set(0, -Height * 0.5f, 0);
	v.color = color;
	vertex_buffer_data.push_back(v);
	for (unsigned i = 0; i < numSlices + 1; ++i)
	{
		index_buffer_data.push_back(offset + numSlices + 1);
		index_buffer_data.push_back(offset + i);
	}
	offset += numSlices + 2;

	//curved side
	for (unsigned slice = 0; slice < numSlices + 1; ++slice)
	{
		float theta = slice * anglePerSlice;
		v.pos.Set(Height*radius*cos(Math::DegreeToRadian(theta)), -Height * 0.5f, Height*radius*sin(Math::DegreeToRadian(theta)));
		v.normal.Set(Height*radius*cos(Math::DegreeToRadian(theta)), -Height, Height*radius*sin(Math::DegreeToRadian(theta)));
		v.color = color;
		vertex_buffer_data.push_back(v);

		v.pos.Set(0, Height * 0.5f, 0);
		v.normal.Set(Height*radius*cos(Math::DegreeToRadian(theta)), radius, Height*radius*sin(Math::DegreeToRadian(theta)));
		v.color = color;
		vertex_buffer_data.push_back(v);
	}
	for (unsigned i = 0; i < (numSlices + 1); ++i)
	{

		index_buffer_data.push_back(offset + 2 * i);
		index_buffer_data.push_back(offset + 2 * i + 1);
	}
	offset += 2 * (numSlices + 1);


	for (unsigned i = 0; i < numSlices + 1; ++i)
	{
		index_buffer_data.push_back(offset + i);
		index_buffer_data.push_back(offset + numSlices + 1);
	}


	Mesh *mesh = new Mesh(meshName);
	glBindBuffer(GL_ARRAY_BUFFER, mesh->vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, vertex_buffer_data.size()*sizeof(Vertex), &vertex_buffer_data[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->indexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, index_buffer_data.size()*sizeof(unsigned), &index_buffer_data[0], GL_STATIC_DRAW);

	mesh->indexSize = index_buffer_data.size();
	mesh->mode = Mesh::DRAW_TRIANGLE_STRIP;

	return mesh;
}

Mesh* MeshBuilder::GenerateSTorus(const std::string &meshName, Color color, Color color2, unsigned numSlice, unsigned numStack, float outerR, float innerR)
{
	Vertex v;
	std::vector<Vertex>  vertex_buffer_data;
	std::vector<unsigned>  index_buffer_data;

	bool changeColor = true;
	float degreePerStack = 360.f / numStack;
	float degreePerSlice = 360.f / numSlice;

	float x1, z1; //y1 is always zero
	float x2, y2, z2;

	for (unsigned stack = 0; stack < numStack + 1; stack++)
	{

		if ((stack % 3) == 0)
		{
			if (changeColor)
				changeColor = false;
			else
				changeColor = true;
		}
		for (unsigned slice = 0; slice < numSlice + 1; slice++)
		{

			float theta = (float)(slice * degreePerSlice);
			float phi = (float)(stack * degreePerStack);

			z1 = outerR * cos(Math::DegreeToRadian(phi));
			x1 = outerR * sin(Math::DegreeToRadian(phi));

			z2 = (outerR + innerR * cos(Math::DegreeToRadian(theta))) * cos(Math::DegreeToRadian(phi));
			y2 = innerR * sin(Math::DegreeToRadian(theta));
			x2 = (outerR + innerR * cos(Math::DegreeToRadian(theta))) * sin(Math::DegreeToRadian(phi));
			v.pos.Set(x2, y2, z2);
			if (changeColor)
				v.color = color;
			else
				v.color = color2;
			v.normal.Set(x2 - x1, y2, z2 - z1);
			v.normal.Normalize();
			vertex_buffer_data.push_back(v);
		}
	}

	for (unsigned stack = 0; stack < numStack; stack++)
	{
		for (unsigned slice = 0; slice < numSlice + 1; slice++)
		{
			index_buffer_data.push_back((numSlice + 1) * stack + slice);
			index_buffer_data.push_back((numSlice + 1) * (stack + 1) + slice);
		}
	}
	Mesh *mesh = new Mesh(meshName);

	glBindBuffer(GL_ARRAY_BUFFER, mesh->vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, vertex_buffer_data.size()*sizeof(Vertex), &vertex_buffer_data[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->indexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, index_buffer_data.size()*sizeof(unsigned), &index_buffer_data[0], GL_STATIC_DRAW);

	mesh->indexSize = index_buffer_data.size();
	mesh->mode = Mesh::DRAW_TRIANGLE_STRIP;
	return mesh;
}

Mesh* MeshBuilder::GenerateOBJ(const std::string &meshName, const std::string &file_path)
{
	//Read vertices, texcoord & normals from obj
	std::vector<Position> vertices;
	std::vector<TexCoord> uvs;
	std::vector<Vector3> normals;
	
	bool success = LoadOBJ(file_path.c_str(), vertices, uvs, normals);
	if (!success)
		return NULL;

	//Index the vertices, texcoord and normals properly
	std::vector<Vertex> vertex_buffer_data;
	std::vector<GLuint> index_buffer_data;
	IndexVBO(vertices, uvs, normals, index_buffer_data, vertex_buffer_data);

	Mesh *mesh = new Mesh(meshName);

	glBindBuffer(GL_ARRAY_BUFFER, mesh->vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, vertex_buffer_data.size()*sizeof(Vertex), &vertex_buffer_data[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->indexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, index_buffer_data.size()*sizeof(GLuint), &index_buffer_data[0], GL_STATIC_DRAW);

	mesh->indexSize = index_buffer_data.size();
	mesh->mode = Mesh::DRAW_TRIANGLES;
	return mesh;
}

Mesh* MeshBuilder::GenerateText(const std::string &meshName, unsigned numRow, unsigned numCol)
{
	Vertex v;
	std::vector<Vertex>  vertex_buffer_data;
	std::vector<unsigned>  index_buffer_data;

	float width = 1.f / numCol;
	float height = 1.f / numRow;

	for (unsigned i = 0; i < numRow; ++i)
	{
		for (unsigned j = 0; j < numCol; ++j)
		{
			v.pos.Set(0.5f, 0.5f, 0); 
				v.normal.Set(0, 0, 1);
				v.texCoord.Set((j+1) * width, (numRow - i-1+1)*height);
				vertex_buffer_data.push_back(v);

			v.pos.Set(0.5f, -0.5f, 0);
				v.normal.Set(0, 0, 1);
				v.texCoord.Set((j + 1) * width, (numRow - i - 1)*height);
				vertex_buffer_data.push_back(v);

			v.pos.Set(-0.5f, 0.5f, 0);
				v.normal.Set(0, 0, 1);
				v.texCoord.Set(j * width, (numRow - i - 1 + 1)*height);
				vertex_buffer_data.push_back(v);

			v.pos.Set(-0.5f, -0.5f, 0);
				v.normal.Set(0, 0, 1);
				v.texCoord.Set( j * width, (numRow - i - 1)*height);
				vertex_buffer_data.push_back(v);

			index_buffer_data.push_back((i*numCol + j) * 4 + 1);
			index_buffer_data.push_back((i*numCol + j) * 4 + 2);
			index_buffer_data.push_back((i*numCol + j) * 4 + 0);
			index_buffer_data.push_back((i*numCol + j) * 4 + 1);
			index_buffer_data.push_back((i*numCol + j) * 4 + 3);
			index_buffer_data.push_back((i*numCol + j) * 4 + 2);
		}
	}

	Mesh *mesh = new Mesh(meshName);

	glBindBuffer(GL_ARRAY_BUFFER, mesh->vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, vertex_buffer_data.size()*sizeof(Vertex), &vertex_buffer_data[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->indexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, index_buffer_data.size()*sizeof(unsigned), &index_buffer_data[0], GL_STATIC_DRAW);

	mesh->indexSize = index_buffer_data.size();
	mesh->mode = Mesh::DRAW_TRIANGLE_STRIP;

	return mesh;
}