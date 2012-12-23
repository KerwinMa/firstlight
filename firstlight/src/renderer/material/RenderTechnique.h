#ifndef FLT_RENDER_TECHNIQUE_H
#define FLT_RENDER_TECHNIQUE_H

#include "renderer/material/RenderPass.h"

namespace flt
{	
namespace renderer
{	


class RenderTechnique
{
public:		
	RenderTechnique();
	~RenderTechnique();
	
	RenderTechnique& operator=( const RenderTechnique& rhs );

	RenderPass* addPass();

	void removePass(u32 index);

	RenderPass* getPass(u32 index);

	RenderPass* getPass(u32 index) const;

	void removeAllPasses();

	u32 getPassCount() const
	{
		return static_cast<u32>(m_passes.size());
	}


private:	
	typedef array_t<RenderPass*> RenderPassArray;
	
	RenderPassArray m_passes;	

	RenderTechnique(const RenderTechnique& rhs);
};


} //end namespace renderer
} //end namespace flt

#endif //FLT_RENDER_TECHNIQUE_H