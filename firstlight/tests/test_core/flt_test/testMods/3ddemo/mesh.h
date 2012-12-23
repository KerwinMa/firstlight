#ifndef FLT_3DDEMO_MESH_H
#define FLT_3DDEMO_MESH_H

#include "firstlight.h"

using namespace flt;
using namespace flt::renderer;

class Mesh
{
public:
	Mesh();

	virtual ~Mesh();

	void create();

	void render(const IRendererPtr& renderer);

protected:
	VertexArray m_vertexArray;
};


#endif