#include "animation/animationSet.h"

namespace flt
{
namespace anim
{

	AnimationSet::AnimationSet()
		:m_isReady(false)
	{

		///Temp code: to make some anim data//////////////////////////////////////
		//Animation* anim = new Animation();
		//anim->setName("enter");
		//addAnimation(anim);

		//anim = new Animation();
		//anim->setName("exit");
		//addAnimation(anim);
		//////////////////////////////////////////////////////////////////////////
	}

	AnimationSet::~AnimationSet()
	{
		clearAll();
	}

	void AnimationSet::clearAll()
	{
		AnimMap::iterator iter = m_animations.begin();

		for(; iter!=m_animations.end(); ++iter)
		{
			Animation* anim = (*iter).second;
			delete anim;
		}

		m_animations.clear();

		m_isReady = false;
	}

	bool AnimationSet::loadFromXMLFile(const char* xmlFile)
	{
		TiXmlDocument doc(xmlFile);
		if (!doc.LoadFile()) 
			return false;

		TiXmlHandle hDoc(&doc);
		
		TiXmlElement* pDocRoot = hDoc.FirstChildElement().Element();		
		if (!pDocRoot) 
			return false;	

		stringc docRootName=pDocRoot->Value();
		if(docRootName != "FLTResXML")
			return false;

		TiXmlHandle hDocRoot(pDocRoot);
		TiXmlElement* pXMLNodeAS = hDocRoot.FirstChild("AnimationSet").Element();

		bool bLoaded = loadFromXML(pXMLNodeAS);

		if(bLoaded)
			init();

		return bLoaded;
	}

	void AnimationSet::saveToXMLFile(const char* xmlFile)
	{

	}

	bool AnimationSet::loadFromXML(TiXmlElement* xmlNodeAnimSet)
	{
		if(xmlNodeAnimSet==0)
			return false;

		TiXmlHandle hXmlNode=TiXmlHandle(xmlNodeAnimSet);				

		// load anims
		TiXmlElement* pAnimNode = hXmlNode.FirstChild( "Animation" ).Element();
		for( ; pAnimNode; pAnimNode=pAnimNode->NextSiblingElement())
		{
			Animation* anim = new Animation();
			if(anim->loadFromXML(pAnimNode))
			{
				AnimMap::iterator iter = m_animations.find(anim->getName());
				if(iter==m_animations.end())
					addAnimation(anim);				
				else
					delete anim;
			}
		}

		return true;
	}

	void AnimationSet::saveToXML(TiXmlElement* xmlNodeAnimSet)
	{
		AnimMap::iterator iter = m_animations.begin();
		for(; iter!=m_animations.end(); ++iter)
		{
			Animation *anim = (*iter).second;
			FLT_ASSERT(anim!=0);

			TiXmlElement * pXmlNodeAnim = new TiXmlElement( "Animation" );  
			xmlNodeAnimSet->LinkEndChild( pXmlNodeAnim );
			anim->saveToXML(pXmlNodeAnim);						
		}		
	}

	void AnimationSet::init()
	{
		AnimMap::iterator iter = m_animations.begin();
		for(; iter!=m_animations.end(); ++iter)
		{
			Animation *anim = (*iter).second;
			FLT_ASSERT(anim!=0);
						
			anim->bindChannels();			
		}

		m_isReady = true;
	}

	void AnimationSet::addAnimation(Animation *anim)
	{		
		m_animations.insert(AnimMap::value_type(anim->getName(), anim));
	}

	void AnimationSet::removeAnimation(const stringc& name)
	{
		Animation* anim = getAnimation(name);
		if(anim!=0)
		{
			delete anim;
			m_animations.erase(name);
		}
	}	

	Animation* AnimationSet::getAnimation(const stringc& name)
	{
		AnimMap::iterator iter = m_animations.find(name);
		if(iter==m_animations.end())
		{
			return 0;
		}
		else
		{
			return (*iter).second;
		}
	}

	Animation* AnimationSet::getAnimation(int index)
	{
		FLT_ASSERT(index>=0 && index<m_animations.size());

		AnimMap::iterator iter = m_animations.begin();
		for(int i=0; i<index; ++i)
			++iter;
		
		return (*iter).second;
	}

	///////////////////////////////////////////////////////////////

		




} //end namespace anim
} //end namespace flt