#ifndef __terrian_h__
#define __terrian_h__

#include "firstlight.h"

using namespace flt;

struct SHeightData
{
	u8* m_data;
	int m_size;

	SHeightData():m_data(NULL),m_size(0)
	{
	}
};

class Terrian
{
public:
	Terrian();
	virtual ~Terrian();

	bool loadHeightMap(const char* fileName, int size);
	bool unloadHeightMap();

	void buildMesh();

	virtual void render(const renderer::IRendererPtr& renderer);

	void setXYScale(float scale)
	{
		m_xyScale = scale;
	}

	void setHeightScale(float scale)
	{
		m_heightScale = scale;
	}

	void setHeightAtPoint(int x, int y, u8 height)
	{
		m_heightData.m_data[y*m_heightData.m_size + x] = height;
	}

	float getTrueHeightAtPoint(int x, int y)
	{
		return m_heightData.m_data[y*m_heightData.m_size + x];
	}

	float getScaledHeightAtPoint(int x, int y)
	{
		return m_heightData.m_data[y*m_heightData.m_size + x] * m_heightScale;
	}

protected:
	SHeightData m_heightData;

	float m_xyScale;
	float m_heightScale;
	 
	flt::renderer::VertexArray m_vertexArray;

};

#endif