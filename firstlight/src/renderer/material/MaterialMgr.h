#ifndef FLT_MATERIAL_MGR_H
#define FLT_MATERIAL_MGR_H

#include "Material.h"
#include "ResourceManager.h"
#include "utils/ISingleton.h"

namespace flt
{	
namespace renderer
{	

class MaterialManager: public ResourceManager<MaterialPtr>, public utils::ISingleton<MaterialManager>
{
public:
	MaterialManager(){};
	virtual ~MaterialManager(){};

	virtual MaterialPtr createRes(const stringc& name, int flag);

	virtual MaterialPtr cloneRes(const MaterialPtr& rhs, const stringc& newResName);

};


} //end namespace renderer
} //end namespace flt

#endif //FLT_MATERIAL_MGR_H
