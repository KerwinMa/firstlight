#include "RenderTechnique.h"

namespace flt
{
namespace renderer
{

RenderTechnique::RenderTechnique()
{

}

RenderTechnique::~RenderTechnique()
{
	removeAllPasses();
}

RenderTechnique& RenderTechnique::operator=( const RenderTechnique& rhs )
{
	if(this==&rhs)
		return *this;

	removeAllPasses();

	u32 pass_num = rhs.getPassCount();

	for(u32 i=0; i<pass_num; ++i)
	{
		RenderPass* pass = addPass();	
		*pass = *rhs.getPass(i);
	}

	return *this;
}

RenderPass* RenderTechnique::addPass()
{
	RenderPass* pass = new RenderPass();
	m_passes.push_back(pass);
	return pass;
}

void RenderTechnique::removePass(u32 index)
{
	FLT_ASSERT(index>=0 && index<m_passes.size());	

	delete m_passes[index];

	m_passes.erase(m_passes.begin()+index);
}

RenderPass* RenderTechnique::getPass(u32 index)
{
	FLT_ASSERT(index>=0 && index<m_passes.size());

	return m_passes[index];
}

RenderPass* RenderTechnique::getPass(u32 index) const
{
	FLT_ASSERT(index>=0 && index<m_passes.size());

	return m_passes[index];
}

void RenderTechnique::removeAllPasses()
{
	ForEach(RenderPassArray, m_passes, iter)
	{
		delete *iter;
	}

	m_passes.clear();
}

}// end namespace renderer
}// end namespace flt