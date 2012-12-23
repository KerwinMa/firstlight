#ifndef FLT_BATCH_BUFFER_H
#define FLT_BATCH_BUFFER_H

#include "VertexArray.h"
#include "renderer/material/Material.h"
#include "core/matrix4.h"

namespace flt
{	
	namespace renderer
	{		
		class IRenderer;

		class BatchBuffer
		{
		public:
			BatchBuffer(BATCH_BUFFER_TYPE batchType, u32 bufferVertexCount, IRenderer* pRenderer);	
			~BatchBuffer();			

			/// Set max vertex count allowed to batch in each time
			void setMaxVertexCountToBatch(u32 vertexCount) { m_maxVertexCountToBatch = vertexCount; }

			/// If can batch the vertex array:
			/// 1) vertex array must match the vertex format of batch buffer
			/// 2) vertex array's vertex count is smaller than batch buffer's max-vertex-to-batch, that is, if vertex array is too large, do not batch it.
			/// 3) material must can batch, multi-pass is not current batchable!!
			bool canBatch(const VertexArray& vertexArray, const MaterialPtr& material);

			/// If batch buffer has enough space to append the vertex array
			bool hasEnoughSpace(const VertexArray& vertexArray);

			/// Append vertexArray in batch
			/// If material is not match current batch material, will flush the batch.
			/// else append vertex array to batch, but must transform it first.
			/// transform: transform the vertex array to world space.
			void append(const VertexArray& vertexArray, const MaterialPtr& material, const core::matrix4& transform);

			/// Flush the batch, draw all vertices in batch
			void flush();

			RenderPass* getRenderPass() const
			{
				return m_batchMaterial->getRenderPass(0);
			}

			const VertexArray& getVertexArray() const
			{
				return m_vertexBuffer;
			}

		private:

			BatchBuffer();

			BatchBuffer(const BatchBuffer& rhs);

			VertexArray m_vertexBuffer;

			BATCH_BUFFER_TYPE m_batchType;

			/// max vertex count allowed to batch in each time
			u32 m_maxVertexCountToBatch;

			/// Material used by batch
			MaterialPtr m_batchMaterial;

			IRenderer* m_pRenderer;

			void doAppend(const VertexArray& vertexArray, const core::matrix4& transform);
							
		};

	} //end namespace renderer
} //end namespace flt

#endif //FLT_BATCH_BUFFER_H