#include "renderer/BatchBuffer.h"
#include "renderer/IRenderer.h"
#include "utils/Logger.h"

namespace flt
{
	namespace renderer
	{
		/// max vertex count allowed to batch in each time, default value
		static const u32 k_maxVertexCountToBatchDefault = 128;

		BatchBuffer::BatchBuffer(BATCH_BUFFER_TYPE batchType, u32 bufferVertexCount, IRenderer* pRenderer)
			:m_batchType(batchType),
			m_maxVertexCountToBatch(bufferVertexCount/2<k_maxVertexCountToBatchDefault?bufferVertexCount/2:k_maxVertexCountToBatchDefault),
			m_pRenderer(pRenderer)
		{
			m_vertexBuffer.initForBatchBuffer(batchType, bufferVertexCount);
		}

		BatchBuffer::~BatchBuffer()
		{
		}

		/// If can batch the vertex array:
		/// 1) vertex array must match the vertex format of batch buffer
		/// 2) vertex array's vertex count is smaller than batch buffer's max-vertex-to-batch, that is, if vertex array is too large, do not batch it.
		/// 3) material must can batch, multi-pass is not current batchable!!
		bool BatchBuffer::canBatch(const VertexArray& vertexArray, const MaterialPtr& material)
		{
			return vertexArray.getVertexCount()<m_maxVertexCountToBatch && 
				m_vertexBuffer.isMatchBatch(m_batchType, vertexArray) && 
				material->canBatch();
		}

		/// If batch buffer has enough space to append the vertex array
		bool BatchBuffer::hasEnoughSpace(const VertexArray& vertexArray)
		{
			return m_vertexBuffer.getBatchRemaingVertexCount()>= vertexArray.getVertexCount();
		}

		/// Append vertexArray in batch
		/// If material is not match current batch material, will flush the batch.
		/// else append vertex array to batch, but must transform it first.
		/// transform: transform the vertex array to world space.
		void BatchBuffer::append(const VertexArray& vertexArray, const MaterialPtr& material, const core::matrix4& transform)
		{
			if(!m_batchMaterial.isValid())
			{
				//first time append, create new material
				m_batchMaterial = new Material(*(material.get()));				
			}

			if(m_batchMaterial->isMatchForBatch(material))
			{
				//flush if space is up
				if(!hasEnoughSpace(vertexArray))
				{
					flush();
				}

				//append incoming vertex array to bach
				doAppend(vertexArray, transform);
			}
			else
			{
				flush();
				m_batchMaterial->operator =(*(material.get()));
				doAppend(vertexArray, transform);
			}
		}

		void BatchBuffer::doAppend(const VertexArray& vertexArray, const core::matrix4& transform)
		{
			//TODO: do transform

			//Note: ignore transform for 2d batching, so no problem now

			m_vertexBuffer.appendToBatch(m_batchType, vertexArray);
		}

		/// Flush the batch, draw all vertices in batch
		void BatchBuffer::flush()
		{
			if(m_vertexBuffer.getBatchedVertexCount()==0)
				return;

			const char* textureName = m_batchMaterial->getRenderPass(0)->getTextureLayer(0).m_texture->getName().c_str();
			DBG("engineDbg_Batch", "Flush batch, texture=%s, batched vertex count=%d, index count=%d", textureName, m_vertexBuffer.getBatchedVertexCount(), m_vertexBuffer.getRenderIndexCount());

			m_pRenderer->drawBatchBuffer(*this);
			m_vertexBuffer.resetBatch();
		}


	} //end namespace renderer
} //end namespace flt
