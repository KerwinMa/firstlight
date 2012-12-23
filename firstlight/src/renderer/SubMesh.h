#ifndef FLT_SUB_MESH_H
#define FLT_SUB_MESH_H

#include "BaseObject.h"
#include "sharePtr.h"
#include "VertexArray.h"

namespace flt
{
namespace renderer
{

FLT_FORWARD_PTR(SubMesh)
FLT_FORWARD_PTR(IRenderer);
FLT_FORWARD_PTR(Material);

class SubMesh: public BaseObject
{
public:	
	SubMesh();
	virtual ~SubMesh();

	void setMaterial(const MaterialPtr& material);
	VertexArray& getVertexArray();

	void render(const IRendererPtr& renderer);

protected:
	MaterialPtr m_material;
	VertexArray m_vertexArray;

};

} //end namespace renderer
} //end namespace flt

#endif //FLT_SUB_MESH_H