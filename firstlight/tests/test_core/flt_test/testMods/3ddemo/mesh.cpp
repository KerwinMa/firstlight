#include "mesh.h"

Mesh::Mesh()
{

}

Mesh::~Mesh()
{

}

void Mesh::create()
{
	//cube for test
	int vertexCount = 8;
	int triangleCount = 12;
	m_vertexArray.initTriangleListInterleaved(renderer::VAF_COLOR, vertexCount, triangleCount);

	core::vector3df pos;
	Colorf color(1,1,1,1);

	//bottom 4 vertex
	pos.set(-0.5, -0.5, 0);
	m_vertexArray.setInterleavedArrayVertexPos(0, pos);
	m_vertexArray.setInterleavedArrayVertexColor(0, color);

	pos.set(0.5, -0.5, 0);
	m_vertexArray.setInterleavedArrayVertexPos(1, pos);
	m_vertexArray.setInterleavedArrayVertexColor(1, color);

	pos.set(0.5, 0.5, 0);
	m_vertexArray.setInterleavedArrayVertexPos(2, pos);
	m_vertexArray.setInterleavedArrayVertexColor(2, color);

	pos.set(-0.5, 0.5, 0);
	m_vertexArray.setInterleavedArrayVertexPos(3, pos);
	m_vertexArray.setInterleavedArrayVertexColor(3, color);

	//top 4 vertex
	color.set(0, 0, 1);

	pos.set(-0.5, -0.5, 1);
	m_vertexArray.setInterleavedArrayVertexPos(4, pos);
	m_vertexArray.setInterleavedArrayVertexColor(4, color);

	color.set(0, 1, 0);

	pos.set(0.5, -0.5, 1);
	m_vertexArray.setInterleavedArrayVertexPos(5, pos);
	m_vertexArray.setInterleavedArrayVertexColor(5, color);

	color.set(1, 0, 0);

	pos.set(0.5, 0.5, 1);
	m_vertexArray.setInterleavedArrayVertexPos(6, pos);
	m_vertexArray.setInterleavedArrayVertexColor(6, color);

	color.set(0, 1, 1);

	pos.set(-0.5, 0.5, 1);
	m_vertexArray.setInterleavedArrayVertexPos(7, pos);
	m_vertexArray.setInterleavedArrayVertexColor(7, color);

	//triangles
	m_vertexArray.setInterleavedArrayIndex(0, 0, 1, 4);
	m_vertexArray.setInterleavedArrayIndex(1, 4, 1, 5);
	m_vertexArray.setInterleavedArrayIndex(2, 1, 2, 5);
	m_vertexArray.setInterleavedArrayIndex(3, 5, 2, 6);
	m_vertexArray.setInterleavedArrayIndex(4, 3, 6, 2);
	m_vertexArray.setInterleavedArrayIndex(5, 6, 3, 7);
	m_vertexArray.setInterleavedArrayIndex(6, 3, 0, 7);
	m_vertexArray.setInterleavedArrayIndex(7, 7, 0, 4);
	m_vertexArray.setInterleavedArrayIndex(8, 1, 0, 2);
	m_vertexArray.setInterleavedArrayIndex(9, 2, 0, 3);
	m_vertexArray.setInterleavedArrayIndex(10,5, 6, 4);
	m_vertexArray.setInterleavedArrayIndex(11,4, 6, 7);
	
}

void Mesh::render(const IRendererPtr& renderer)
{
	static float rot = 0;
	rot += 0.01;
	if(rot>360)
		rot = 0;

	core::matrix4 mat,mat2;
	mat.setScale(20);
	mat2.setRotationRadians(core::vector3df(0,0,rot));
	renderer->setWorldTransform(mat*mat2);

	renderer->drawPrimitives(m_vertexArray);
}