#include "SubMesh.h"
#include "IRenderer.h"
#include "material/Material.h"

namespace flt
{
namespace renderer
{

SubMesh::SubMesh()
{

}
SubMesh::~SubMesh()
{

}

void SubMesh::render(const IRendererPtr& renderer)
{
	if(m_material.isValid())
		renderer->setMaterial(m_material);

	renderer->drawPrimitives(m_vertexArray);
}

}//renderer
}//flt