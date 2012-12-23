#ifndef FLT_VERTEX_ARRAY_H
#define FLT_VERTEX_ARRAY_H

#include "common.h"
#include "core/vector3d.h"
#include "renderer/Color.h"
#include "core/matrix4.h"
#include "core/rect.h"
#include "RendererDefs.h"

namespace flt
{	
namespace renderer
{

enum VERTEX_ARRAY_FLAG
{	
	VAF_NORMAL = 1,
	VAF_COLOR = VAF_NORMAL<<1,
	VAF_UV = VAF_COLOR<<1,
	VAF_UV2 = VAF_UV<<1,
	VAF_UV3 = VAF_UV2<<1,
	VAF_UV4 = VAF_UV3<<1,
};

enum PRIMITIVE_TYPE
{
	PT_TRIANGLES = 0,
	PT_TRIANGLE_STRIP,
	PT_LINES,
	PT_LINE_STRIP,
    PT_LINE_LOOP,
	PT_POINTS,
};

enum VERTEX_COMPONEN_TYPE
{
	VCT_BYTE = 0,
	VCT_UNSIGNED_BYTE,
	VCT_SHORT,
	VCT_UNSIGNED_SHORT,
	VCT_INT,
	VCT_UNSIGNED_INT,
	VCT_FLOAT	
};

enum INDEX_TYPE
{
	INDEX_NULL = 0,
	INDEX_U8,
	INDEX_U16,
	INDEX_U32
};

class Color;

class VertexArray
{
public:
	VertexArray();	
	~VertexArray();

	void compile();	

	u32 getVertexCount() const { return m_vertexCount; }

	//init for a triangle list with interleaved array
	//all vertex components are float, and index is U16
	void initTriangleListInterleaved(s32 flags, u32 vertexCount, u32 triangleCount);
	void setInterleavedArrayVertexPos(u32 vertexIndex, const flt::core::vector3df &pos);
	void setInterleavedArrayVertexColor(u32 vertexIndex, const flt::renderer::Colorf &color);
	void setInterleavedArrayNormal(u32 vertexIndex, const flt::core::vector3df &normal);
	void setInterleavedArrayUV(u32 vertexIndex, u32 texUnit, float u, float v);
	void setInterleavedArrayIndex(u32 triangelIndex, u16 v1, u16 v2, u16 v3);

	//init for a single 2d quard
	void initForQuard2D(s32 flags, u32 quardNum=1);
	void setQuard2DPos(const core::rectf& quard, core::matrix4 *pMatrix=0);
	void setQuard2DColor(const Color* colors, bool singleColor=false);
	void setQuard2DUV(f32 u0, f32 v0, f32 u1, f32 v1);

	//only for test draw primitive
	void initForQuardList3df(int flags, int quardNum);
	void appendQuard3df(const flt::core::vector3df& pos, f32 width, f32 height, Color* color);
	void setQuard3df(int quardIdx, const flt::core::vector3df& pos0, const flt::core::vector3df& pos1, const flt::core::vector3df& pos2, const flt::core::vector3df& pos3);

	///--------for batch-----------------

	bool isMatchBatch(BATCH_BUFFER_TYPE batchType, const VertexArray& rhs);

	//init for a batch buffer
	void initForBatchBuffer(BATCH_BUFFER_TYPE batchType, u32 vertexCount);

	u32 getBatchRemaingVertexCount() const { return m_vertexCount-m_batchedVertexCount; }

	u32 getBatchedVertexCount() const { return m_batchedVertexCount; }

	void appendToBatch(BATCH_BUFFER_TYPE batchType, const VertexArray& rhs);

	u32 getRenderIndexCount() const;

	void resetBatch();

public:

	PRIMITIVE_TYPE m_primitiveType;

	//! Count of vertices in vertex array
	int m_vertexCount;
	int m_vertexSize; //size of a vertex

	//! Position component
	VERTEX_COMPONEN_TYPE m_posType;
	int m_posCompCount;
	int m_posStride;
	union{
		void* m_positions;
		s8* m_posS8;
		s16* m_posS16;
		f32* m_posF32;
	};

	VERTEX_COMPONEN_TYPE m_normalType;
	int m_normalStride;
	union{
		void* m_normals;
		s8* m_normalsS8;
		s16* m_normalsS16;
		f32* m_normalsF32;
	};

	VERTEX_COMPONEN_TYPE m_colorType;
	int m_colorStride;
	union{
		void* m_colors;			
		u8* m_colorsU8;
		f32* m_colorsF32;
	};
	
	int m_textUnitCount; //Must less than FLT_MAX_TEXTURE_UNITS;
	int m_texCoordStride[FLT_MAX_TEXTURE_UNITS];	
	f32* m_texCoords[FLT_MAX_TEXTURE_UNITS];
	
	

	//! Index
	INDEX_TYPE m_indexType;
	union{
		void* m_indices;
		u16* m_indicesU16;
	};

	int m_indexCount;

	union{
		void* m_interleavedArray;
		f32* m_interleavedArrayF32;
	};

	bool m_ownData;
	bool m_useInterleavedArray;

private:
	void reset();
	void freeData();
	inline bool testFlag(int flag) const { return (m_flags & flag) > 0; }			

	int m_flags;

	int m_appendLastElementIdx;

	u32 m_batchedVertexCount; //vertex count already appended to batch

	bool m_bIsBatch;
};

} //end namespace renderer
} //end namespace flt

#endif //FLT_VERTEX_ARRAY_H