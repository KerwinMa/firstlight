#ifndef FLT_RESOURCE_MANAGER_H
#define FLT_RESOURCE_MANAGER_H

#include "common.h"
#include <algorithm>
#include "utils/Logger.h"

namespace flt
{

template<typename ResObjectPtr>
class ResourceManager
{
public:
	
	typedef array_t< ResObjectPtr > ResObjectArray;

	ResourceManager(){};
	virtual ~ResourceManager()
	{
		removeAllUnusedRes();
	};

	struct Finder
	{
		Finder(const stringc& name)
			: m_name(name)
		{
		}

		bool operator()( const ResObjectPtr& test )
		{
			return test.isValid() && test->getName() == m_name;
		}

		stringc m_name;
	};

	ResObjectPtr getRes(const stringc& name, int flag=0)
	{			
		typename ResObjectArray::const_iterator iter = std::find_if(m_resources.begin(), m_resources.end(), Finder(name));
		if (m_resources.end() == iter)
		{
			ResObjectPtr newRes = createRes(name, flag);
			
			//check if newRes is valid (createRes may return a invalid ptr for the res is not found or create failed
			//if not valid,  not push to array
			if(newRes.isValid())
				m_resources.push_back(newRes);

			return newRes;
		}
		else
		{
			return *iter;
		}
	}

	void addRes(const ResObjectPtr& rhs)
	{
		//Note: maybe need check if res already in mgr
		m_resources.push_back(rhs);
	}

	void removeRes(const stringc& name)
	{
		typename ResObjectArray::iterator iter = std::find_if(m_resources.begin(), m_resources.end(), Finder(name));
		if (m_resources.end() != iter)
		{			
			m_resources.erase(iter);
		}
	}

	void removeAllUnusedRes()
	{
		typename ResObjectArray::iterator iter = m_resources.begin();

		while(iter!=m_resources.end())
		{
			if((*iter)->getRefCount()==1)
			{
				DBG("engineDbg_Res","remove unused res: %s", (*iter)->getName().c_str());
				iter = m_resources.erase(iter);				
			}			
			else
			{
				iter++;
			}
		}		
	}

	int getResCount()
	{
		return static_cast<int>(m_resources.size());
	}	

	bool isResUsed(const stringc& name)
	{
		typename ResObjectArray::const_iterator iter = std::find_if(m_resources.begin(), m_resources.end(), Finder(name));
		return (m_resources.end() != iter && (*iter)->getRefCount()>1);		
	}

	virtual ResObjectPtr createRes(const stringc& name, int flag) = 0;
	
	virtual ResObjectPtr cloneRes(const ResObjectPtr& rhs, const stringc& newResName) = 0;	
	
private:

	ResObjectArray m_resources;
};

}//end namespace flt

#endif //FLT_RESOURCE_MANAGER_H