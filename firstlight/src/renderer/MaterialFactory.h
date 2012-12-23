#ifndef FLT_MATERIAL_FACTORY_H
#define FLT_MATERIAL_FACTORY_H

#include "utils/ISingleton.h"
#include "renderer/RendererDefs.h"
#include "renderer/material/Material.h"


namespace flt
{

namespace renderer
{

FLT_FORWARD_PTR(IMaterialModifier);

class MaterialFactory: public utils::ISingleton<MaterialFactory>
{
public:		

	MaterialFactory();

	virtual ~MaterialFactory();	

	void addModifiers();

	void addModifier(const IMaterialModifierPtr& modifier);
	
	void modifyMaterialByType(MaterialPtr& material, BUILT_IN_MATERIAL_TYPE materialType, bool set);

private:

	void deleteModifiers();

	array_t<IMaterialModifierPtr> m_modifiers;

};

class IMaterialModifier: public BaseObject
{
public:
	virtual ~IMaterialModifier(){}

	virtual void set(MaterialPtr& material) = 0;

	virtual void unset(MaterialPtr& material) = 0;
};

/////////////////////////////////////////////////
// material modifiers

class MaterialModifier_2D_SOLID: public IMaterialModifier
{
public:

	virtual void set(MaterialPtr& material);

	virtual void unset(MaterialPtr& material);
};

class MaterialModifier_2D_ALPHA_BLEND: public IMaterialModifier
{
public:

	virtual void set(MaterialPtr& material);

	virtual void unset(MaterialPtr& material);
};

class MaterialModifier_2D_ADDITIVE: public IMaterialModifier
{
public:

	virtual void set(MaterialPtr& material);

	virtual void unset(MaterialPtr& material);
};

class MaterialModifier_2D_TEXTURE_ALPHA_BLEND_MODULATE_COLOR: public IMaterialModifier
{
public:

	virtual void set(MaterialPtr& material);

	virtual void unset(MaterialPtr& material);
};

class MaterialModifier_2D_TEXTURE_ADDITIVE_MODULATE_COLOR: public IMaterialModifier
{
public:

	virtual void set(MaterialPtr& material);

	virtual void unset(MaterialPtr& material);
};





} //end namespace renderer
} //end namespace flt

#endif //FLT_MATERIAL_FACTORY_H