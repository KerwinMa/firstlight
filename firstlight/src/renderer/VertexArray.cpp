#include "renderer/VertexArray.h"
#include "renderer/Color.h"
#include "utils/Logger.h"

namespace flt
{
namespace renderer
{

VertexArray::VertexArray()
{	
	reset();
}

VertexArray::~VertexArray()
{
	freeData();
}

void VertexArray::initTriangleListInterleaved(s32 flags, u32 vertexCount, u32 triangleCount)
{
	freeData();	
	reset();

	m_flags = flags;

	m_ownData = true;
	m_useInterleavedArray = true;
	m_vertexCount = vertexCount;

	m_primitiveType = PT_TRIANGLES;

	//pos
	m_posType = VCT_FLOAT;
	m_posCompCount = 3;
	m_vertexSize = 3; //number of floats of a single vertex, at least 3 for position

	//color
	if(testFlag(VAF_COLOR))
	{
		m_colorType = VCT_FLOAT;
		m_colorStride = m_vertexSize;//used as component offset, real stride is the vertex size, and will be set later, same as other components
		m_vertexSize += 4;
	}

	//normal
	if(testFlag(VAF_NORMAL))
	{
		m_normalType = VCT_FLOAT;
		m_normalStride = m_vertexSize;
		m_vertexSize += 3;
	}
	
	//uv
	if(testFlag(VAF_UV))
	{
		m_texCoordStride[m_textUnitCount++] = m_vertexSize;
		m_vertexSize+=2;
	}

	if(testFlag(VAF_UV2))
	{
		m_texCoordStride[m_textUnitCount++] = m_vertexSize;
		m_vertexSize+=2;
	}

	if(testFlag(VAF_UV3))
	{
		m_texCoordStride[m_textUnitCount++] = m_vertexSize;
		m_vertexSize+=2;
	}

	if(testFlag(VAF_UV4))
	{
		m_texCoordStride[m_textUnitCount++] = m_vertexSize;
		m_vertexSize+=2;
	}

	//vertex array, interleaved
	m_interleavedArrayF32 = new f32[m_vertexSize*m_vertexCount];

	int offset = 0;

	m_vertexSize*=sizeof(float);

	//pos
	m_posF32 = m_interleavedArrayF32;
	m_posStride = m_vertexSize;

	//color
	if(testFlag(VAF_COLOR))
	{
		m_colorsF32 = m_interleavedArrayF32+m_colorStride;
		m_colorStride = m_vertexSize;
	}

	//normal
	if(testFlag(VAF_NORMAL))
	{
		m_normalsF32 = m_interleavedArrayF32+m_normalStride;
		m_normalStride = m_vertexSize;
	}

	//uv
	if(testFlag(VAF_UV))
	{
		m_texCoords[0] = m_interleavedArrayF32+m_texCoordStride[0];
		m_texCoordStride[0] = m_vertexSize;
	}

	if(testFlag(VAF_UV2))
	{
		m_texCoords[1] = m_interleavedArrayF32+m_texCoordStride[1];
		m_texCoordStride[1] = m_vertexSize;
	}

	if(testFlag(VAF_UV3))
	{
		m_texCoords[2] = m_interleavedArrayF32+m_texCoordStride[2];
		m_texCoordStride[2] = m_vertexSize;
	}

	if(testFlag(VAF_UV4))
	{
		m_texCoords[3] = m_interleavedArrayF32+m_texCoordStride[3];
		m_texCoordStride[3] = m_vertexSize;
	}

	//index
	m_indexCount = triangleCount*3;
	m_indexType = INDEX_U16;
	m_indicesU16 = new u16[m_indexCount];
}

void VertexArray::setInterleavedArrayVertexPos(u32 vertexIndex, const flt::core::vector3df &pos)
{
	int idx = vertexIndex*m_posStride/sizeof(float);

	m_posF32[idx] = pos.X;
	m_posF32[idx+1] = pos.Y;
	m_posF32[idx+2] = pos.Z;
}

void VertexArray::setInterleavedArrayVertexColor(u32 vertexIndex, const flt::renderer::Colorf &color)
{
	if(testFlag(VAF_COLOR))
	{
		int idx = vertexIndex*m_colorStride/sizeof(float);

		m_colorsF32[idx] = color.r;
		m_colorsF32[idx+1] = color.g;
		m_colorsF32[idx+2] = color.b;
		m_colorsF32[idx+3] = color.a;
	}
}

void VertexArray::setInterleavedArrayNormal(u32 vertexIndex, const flt::core::vector3df &normal)
{
	if(testFlag(VAF_NORMAL))
	{
		int idx = vertexIndex*m_normalStride/sizeof(float);

		m_normalsF32[idx] = normal.X;
		m_normalsF32[idx+1] = normal.Y;
		m_normalsF32[idx+2] = normal.Z;
	}
}

void VertexArray::setInterleavedArrayUV(u32 vertexIndex, u32 texUnit, float u, float v)
{
	if((texUnit==0 && testFlag(VAF_UV)) ||
		(texUnit==1 && testFlag(VAF_UV2)) ||
		(texUnit==2 && testFlag(VAF_UV3)) ||
		(texUnit==3 && testFlag(VAF_UV4)))
	{
		m_texCoords[texUnit][vertexIndex*m_texCoordStride[texUnit]/sizeof(float)] = u;
		m_texCoords[texUnit][vertexIndex*m_texCoordStride[texUnit]/sizeof(float)+1] = v;
	}
}

void VertexArray::setInterleavedArrayIndex(u32 triangelIndex, u16 v1, u16 v2, u16 v3)
{
	m_indicesU16[triangelIndex*3] = v1;
	m_indicesU16[triangelIndex*3+1] = v2;
	m_indicesU16[triangelIndex*3+2] = v3;
}

// 2d quard layout
//  0----2
//  |  / |
//  | /  |
//  1----3
//

void VertexArray::initForQuard2D(s32 flags, u32 quardNum)
{
	freeData();	
	reset();

	m_flags = flags;

	m_ownData = true;
	m_useInterleavedArray = false;

	m_primitiveType = PT_TRIANGLES;

	m_vertexCount = quardNum*4;

	//pos
	{		
		m_posType = VCT_FLOAT;
		m_posCompCount = 2;
		m_posStride = 0;
		m_positions = new f32[m_vertexCount*2];
	}

	//uv
	if(testFlag(VAF_UV))
	{		
		m_texCoordStride[0] = 0;
		m_texCoords[0] = new f32[m_vertexCount*2];
		
		//v0
		m_texCoords[0][0] = 0.0f;
		m_texCoords[0][1] = 0.0f;
		//v1
		m_texCoords[0][2] = 0.0f;
		m_texCoords[0][3] = 1.0f;
		//v2
		m_texCoords[0][4] = 1.0f;
		m_texCoords[0][5] = 0.0f;
		//v3
		m_texCoords[0][6] = 1.0f;
		m_texCoords[0][7] = 1.0f;			
	}

	//color
	if(testFlag(VAF_COLOR))
	{		
		m_colorType = VCT_UNSIGNED_BYTE;
		m_colorStride = 0;
		m_colors = new u8[m_vertexCount*4];
		memset(m_colors,255,sizeof(u8)*m_vertexCount*4);
	}

	//index	
	m_indexCount = quardNum*6;
	m_indexType = INDEX_U16;
	m_indices = new u16[m_indexCount];

	{		
		int i = 0;
		int j = 0;
		m_indicesU16[i] = j;
		m_indicesU16[i+1] = j+1;
		m_indicesU16[i+2] = j+2;
		m_indicesU16[i+3] = j+2;
		m_indicesU16[i+4] = j+1;
		m_indicesU16[i+5] = j+3;	
	}
}

void VertexArray::setQuard2DPos(const core::rectf& quard, core::matrix4 *pMatrix)
{	
	f32 x = quard.UpperLeftCorner.X;
	f32 y = quard.UpperLeftCorner.Y;
	f32 width = quard.getWidth();
	f32 height = quard.getHeight();

	//v0
	m_posF32[0] = x;
	m_posF32[1] = y;

	//v1
	m_posF32[2] = x;
	m_posF32[3] = y+height;	

	//v2
	m_posF32[4] = x+width;
	m_posF32[5] = y;	

	//v3
	m_posF32[6] = x+width;
	m_posF32[7] = y+height;

	//transform the quard
	if(pMatrix!=0)
	{
		core::vector3df tmpVec;
		for(int i=0; i<4; i++)
		{
			tmpVec.set(m_posF32[i*2],m_posF32[i*2+1],0.0f);
			pMatrix->transformVect(tmpVec);
			m_posF32[i*2] = tmpVec.X;
			m_posF32[i*2+1] = tmpVec.Y;
		}
	}
}

void VertexArray::setQuard2DColor(const Color* colors, bool singleColor)
{
	if(!testFlag(VAF_COLOR))
	{
		return;
	}

	int i = 0;
	for(int k=0; k<4; k++)
	{
		if(singleColor)
		{
			colors->toRGBA32(&m_colorsU8[i]);
		}
		else
		{
			colors[k].toRGBA32(&m_colorsU8[i]);
		}

		i+=4;
	}
}

void VertexArray::setQuard2DUV(f32 u0, f32 v0, f32 u1, f32 v1)
{
	if(!testFlag(VAF_UV))
	{
		return;
	}

	//v0
	m_texCoords[0][0] = u0;
	m_texCoords[0][1] = v0;
	//v1
	m_texCoords[0][2] = u0;
	m_texCoords[0][3] = v1;
	//v2
	m_texCoords[0][4] = u1;
	m_texCoords[0][5] = v0;
	//v3
	m_texCoords[0][6] = u1;
	m_texCoords[0][7] = v1;	
}

//--------------------

void VertexArray::initForQuardList3df(int flags, int quardNum)
{
	freeData();	
	reset();

	m_flags = flags;

	m_ownData = true;
	m_useInterleavedArray = false;

	m_primitiveType = PT_TRIANGLES;

	m_vertexCount = 4*quardNum;

	//pos
	{		
		m_posType = VCT_FLOAT;
		m_posCompCount = 3;
		m_posStride = 0;
		m_positions = new f32[m_vertexCount*3];
	}

	//uv	
	m_texCoordStride[0] = 0;
	m_texCoords[0] = new f32[m_vertexCount*2];

	//color
	if(testFlag(VAF_COLOR))
	{		
		m_colorType = VCT_UNSIGNED_BYTE;
		m_colorStride = 0;
		m_colors = new u8[m_vertexCount*4];
	}

	//index
	m_indexCount = 6*quardNum;
	m_indexType = INDEX_U16;
	m_indices = new u16[m_indexCount];			
}

void VertexArray::appendQuard3df(const flt::core::vector3df& pos, f32 width, f32 height, Color* color)
{
	if(m_appendLastElementIdx>=m_vertexCount/4)
		return;

	//  0----2
	//  |  / |
	//  | /  |
	//  1----3
	//

	int i = m_appendLastElementIdx*12;	

	f32 hw = width/2;
	f32 hh = height/2;

	//--pos------

	//v0
	m_posF32[i++] = pos.X-hw;
	m_posF32[i++] = pos.Y+hh;
	m_posF32[i++] = pos.Z;

	//v1
	m_posF32[i++] = pos.X-hw;
	m_posF32[i++] = pos.Y-hh;
	m_posF32[i++] = pos.Z;

	//v2
	m_posF32[i++] = pos.X+hw;
	m_posF32[i++] = pos.Y+hh;
	m_posF32[i++] = pos.Z;

	//v3
	m_posF32[i++] = pos.X+hw;
	m_posF32[i++] = pos.Y-hh;
	m_posF32[i++] = pos.Z;

	//--uv--
	i = m_appendLastElementIdx*8;	
	//v0
	m_texCoords[0][i++] = 0.0f;
	m_texCoords[0][i++] = 0.0f;
	//v1
	m_texCoords[0][i++] = 0.0f;
	m_texCoords[0][i++] = 1.0f;
	//v2
	m_texCoords[0][i++] = 1.0f;
	m_texCoords[0][i++] = 0.0f;
	//v3
	m_texCoords[0][i++] = 1.0f;
	m_texCoords[0][i++] = 1.0f;

	//--color---
	if(testFlag(VAF_COLOR) && color!=0)
	{
		i = m_appendLastElementIdx*16;
		for(int k=0; k<4; k++)
		{
			color->toRGBA32(&m_colorsU8[i]);
			i+=4;
		}
	}


	//----index---------
	i = m_appendLastElementIdx*6;
	int j = m_appendLastElementIdx*4;
	m_indicesU16[i] = j;
	m_indicesU16[i+1] = j+1;
	m_indicesU16[i+2] = j+2;
	m_indicesU16[i+3] = j+2;
	m_indicesU16[i+4] = j+1;
	m_indicesU16[i+5] = j+3;

	m_appendLastElementIdx++;
}

void VertexArray::setQuard3df(int quardIdx, const flt::core::vector3df& pos0, const flt::core::vector3df& pos1, const flt::core::vector3df& pos2, const flt::core::vector3df& pos3)
{
	if(quardIdx>=m_vertexCount/4)
		return;

	//  0----2
	//  |  / |
	//  | /  |
	//  1----3
	//

	int i = quardIdx*12;		

	//--pos------

	//v0
	m_posF32[i++] = pos0.X;
	m_posF32[i++] = pos0.Y;
	m_posF32[i++] = pos0.Z;

	//v1
	m_posF32[i++] = pos1.X;
	m_posF32[i++] = pos1.Y;
	m_posF32[i++] = pos1.Z;

	//v2
	m_posF32[i++] = pos2.X;
	m_posF32[i++] = pos2.Y;
	m_posF32[i++] = pos2.Z;

	//v3
	m_posF32[i++] = pos3.X;
	m_posF32[i++] = pos3.Y;
	m_posF32[i++] = pos3.Z;

	//--uv--
	i = quardIdx*8;	
	//v0
	m_texCoords[0][i++] = 0.0f;
	m_texCoords[0][i++] = 0.0f;
	//v1
	m_texCoords[0][i++] = 0.0f;
	m_texCoords[0][i++] = 1.0f;
	//v2
	m_texCoords[0][i++] = 1.0f;
	m_texCoords[0][i++] = 0.0f;
	//v3
	m_texCoords[0][i++] = 1.0f;
	m_texCoords[0][i++] = 1.0f;	


	//----index---------
	i = quardIdx*6;
	int j = quardIdx*4;
	m_indicesU16[i] = j;
	m_indicesU16[i+1] = j+1;
	m_indicesU16[i+2] = j+2;
	m_indicesU16[i+3] = j+2;
	m_indicesU16[i+4] = j+1;
	m_indicesU16[i+5] = j+3;	
}

void VertexArray::compile()
{
	
}


void VertexArray::reset()
{
	m_primitiveType = PT_TRIANGLES;

	m_vertexCount = 0;
	m_vertexSize= 0;

	m_posType = VCT_FLOAT;
	m_posCompCount = 3;
	m_posStride = 0;
	m_positions = 0;

	m_normalType = VCT_FLOAT;
	m_normalStride = 0;
	m_normals = 0;

	m_colorType = VCT_UNSIGNED_BYTE;
	m_colorStride = 0;
	m_colors = 0;
	
	m_textUnitCount = 0;

	for(int i=0; i<FLT_MAX_TEXTURE_UNITS; ++i)
	{
		m_texCoordStride[i] = 0;
		m_texCoords[i] = 0;
	}		

	m_indexType = INDEX_NULL;
	m_indices = 0;
	m_indexCount = 0;

	m_interleavedArray = 0;
	m_ownData = false;
	m_useInterleavedArray = false;

	m_flags = 0;
	m_appendLastElementIdx = 0;

	//for batch
	resetBatch();
	
	m_bIsBatch = false;
}


void VertexArray::freeData()
{
	if(m_ownData)
	{
		if(m_useInterleavedArray)
		{
			SAFE_DEL_ARRAY(m_interleavedArray);			
		}
		else
		{
			SAFE_DEL_ARRAY(m_positions);			
			SAFE_DEL_ARRAY(m_colors);					
			SAFE_DEL_ARRAY(m_normals);			
			for(u32 i=0; i<FLT_MAX_TEXTURE_UNITS; ++i)
			{
				SAFE_DEL_ARRAY(m_texCoords[i]);
			}			
		}

		SAFE_DEL_ARRAY(m_indices);
	}	
}

/// batch //////////////////////////////////////////

bool VertexArray::isMatchBatch(BATCH_BUFFER_TYPE batchType, const VertexArray& rhs)
{
	if(batchType==BBT_2D)
	{
		return m_flags==rhs.m_flags &&
			rhs.m_primitiveType==PT_TRIANGLES && 
			rhs.m_posType==VCT_FLOAT && 
			rhs.m_posCompCount==2 &&			
			rhs.m_texCoords[0]!=0 &&
			rhs.m_texCoords[1]==0;
	}
	else
	{
		//TODO: 3d batch
		return false;
	}
}

void VertexArray::initForBatchBuffer(BATCH_BUFFER_TYPE batchType, u32 vertexCount)
{
	if(batchType==BBT_2D)
	{
		initForQuard2D(VAF_COLOR|VAF_UV,vertexCount/4);
	}
	else
	{
		//TODO: 3d batch		
	}	

	m_bIsBatch = true;

	resetBatch();
}

void VertexArray::appendToBatch(BATCH_BUFFER_TYPE batchType, const VertexArray& rhs)
{
	if(batchType==BBT_2D)
	{
		f32* pointer = 0;

		pointer = m_posF32 + m_batchedVertexCount*2;
		memcpy(pointer, rhs.m_positions, rhs.m_vertexCount*2*sizeof(f32));

		if(testFlag(VAF_UV))
		{
			pointer = m_texCoords[0] + m_batchedVertexCount*2;
			memcpy(pointer, rhs.m_texCoords[0], rhs.m_vertexCount*2*sizeof(f32));
		}

		if(testFlag(VAF_COLOR))
		{
			u8* p = m_colorsU8 + m_batchedVertexCount*4;
			memcpy(p, rhs.m_colors, rhs.m_vertexCount*4*sizeof(u8));			
		}

		//Note: For 2d drawing, VertexArray has only 1 quard, so index can based on m_batchedVertexCount
		int newIndexCount = rhs.m_vertexCount*3/2;		
		for(int i=0; i<newIndexCount; ++i)
		{
			m_indicesU16[m_indexCount+i] = m_batchedVertexCount+rhs.m_indicesU16[i];
		}				

		m_batchedVertexCount += rhs.m_vertexCount;
		m_indexCount += newIndexCount;
		
		/*
		u32 quardIdx = m_batchedVertexCount/4;
		u32 quardNum = rhs.m_vertexCount/4;

		for(int i=0; i<quardNum; ++quardIdx, ++i)
		{			
			//pos
			{	
				int pi = quardIdx*8;
				int rpi = i*8;
				//v0
				m_posF32[pi] = rhs.m_posF32[rpi];
				m_posF32[pi+1] = rhs.m_posF32[rpi+1];

				//v1
				m_posF32[pi+2] = rhs.m_posF32[rpi+2];
				m_posF32[pi+3] = rhs.m_posF32[rpi+3];

				//v2
				m_posF32[pi+4] = rhs.m_posF32[rpi+4];
				m_posF32[pi+5] = rhs.m_posF32[rpi+5];

				//v3
				m_posF32[pi+6] = rhs.m_posF32[rpi+6];
				m_posF32[pi+7] = rhs.m_posF32[rpi+7];
			}

			//uv
			if(testFlag(VAF_UV))
			{		
				int uvi = quardIdx*8;
				int ruvi = i*8;

				//v0
				m_texCoords[0][uvi] = rhs.m_texCoords[0][ruvi];
				m_texCoords[0][uvi+1] = rhs.m_texCoords[0][ruvi+1];
				//v1
				m_texCoords[0][uvi+2] = rhs.m_texCoords[0][ruvi+2];
				m_texCoords[0][uvi+3] = rhs.m_texCoords[0][ruvi+3];
				//v2
				m_texCoords[0][uvi+4] = rhs.m_texCoords[0][ruvi+4];
				m_texCoords[0][uvi+5] = rhs.m_texCoords[0][ruvi+5];
				//v3
				m_texCoords[0][uvi+6] = rhs.m_texCoords[0][ruvi+6];
				m_texCoords[0][uvi+7] = rhs.m_texCoords[0][ruvi+7];			
			}

			//color
			if(testFlag(VAF_COLOR))
			{		
				int ci = quardIdx*16;
				int rci = i*16;

				for(int c=0; c<16; c++)
					m_colorsU8[ci+c] = rhs.m_colorsU8[rci+c];
			}

			//index				
			{		
				int ii = quardIdx*6;
				int rii = i*6;
				m_indicesU16[ii] = m_batchedVertexCount+rhs.m_indicesU16[rii];
				m_indicesU16[ii+1] = m_batchedVertexCount+rhs.m_indicesU16[rii+1];
				m_indicesU16[ii+2] = m_batchedVertexCount+rhs.m_indicesU16[rii+2];
				m_indicesU16[ii+3] = m_batchedVertexCount+rhs.m_indicesU16[rii+3];
				m_indicesU16[ii+4] = m_batchedVertexCount+rhs.m_indicesU16[rii+4];
				m_indicesU16[ii+5] = m_batchedVertexCount+rhs.m_indicesU16[rii+5];	

				m_indexCount += 6;
			}			
		}

		m_batchedVertexCount += rhs.m_vertexCount;
		*/
		//DBG("engineDbg_Batch", "Append %d vertices to batch, now batched vertex num: %d", rhs.m_vertexCount, m_batchedVertexCount);
	}
}

u32 VertexArray::getRenderIndexCount() const
{
	//if(m_bIsBatch)
	//{
	//	return (m_batchedVertexCount*3)/2;
	//}
	//else
	{
		return m_indexCount;
	}	
}

void VertexArray::resetBatch()
{
	m_batchedVertexCount = 0;
	m_indexCount = 0;
}

} //end namespace renderer
} //end namespace flt
