#ifndef FLT_3DDEMO_GIRD_H
#define FLT_3DDEMO_GIRD_H

#include "firstlight.h"

using namespace flt;
using namespace flt::renderer;

class Grid
{
public:
	Grid(int rows, int cols, float girdWidth);
	void buildMesh();

	void render(const IRendererPtr& renderer);

private:

	int m_rows, m_cols;
	float m_girdWidth;
	VertexArray m_vertexArray;
};

#endif //FLT_3DDEMO_GIRD_H