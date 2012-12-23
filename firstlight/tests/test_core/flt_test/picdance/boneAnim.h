#ifndef BONE_ANIM_H
#define BONE_ANIM_H

#include <vector>
#include "firstlight.h"
#include "kfAnim/kfAnimManager.h"

using namespace flt;
using namespace kfAnim;

enum EBodyPart
{
	BP_HEAD = 0,
	BP_BODY,
	BP_L_UPPERARM,
	BP_L_FOREARM,
	BP_R_UPPERARM,
	BP_R_FOREARM,

	BP_PART_COUNT,
};

class BodyPart
{
public:
	BodyPart()
	{		
		m_mesh.initForQuardList3df(0, 1);
	}

	~BodyPart(){}

	void setBonePosRot(f32 x0, f32 y0, f32 x1, f32 y1, f32 x2, f32 y2, f32 x3, f32 y3)
	{
		m_bonePos[0].set(x0,y0,0.0f);
		m_bonePos[1].set(x1,y1,0.0f);
		m_bonePos[2].set(x2,y2,0.0f);
		m_bonePos[3].set(x3,y3,0.0f);					
	}

	void setTexture(const renderer::TexturePtr& texture)
	{
		m_texture = texture;
	}

	void transformMesh(const core::matrix4& mat)
	{
		for(int i=0; i<4; i++)
		{
			mat.transformVect(m_worldPos[i], m_bonePos[i]);
		}

		m_mesh.setQuard3df(0, m_worldPos[0], m_worldPos[1], m_worldPos[2], m_worldPos[3]);
	}

	void render(const renderer::IRendererPtr& renderer, renderer::MaterialPtr& material)
	{
		renderer->setMaterial(material);
		renderer->drawPrimitives(m_mesh);
	}

private:
	core::vector3df m_bonePos[4];	//pos relative to bone	

	core::vector3df m_worldPos[4];

	renderer::TexturePtr m_texture;
	renderer::VertexArray m_mesh;
};

////////////////////////////////////////////////////////////////////////////////////////

class Bone: public kfAnim::IAnimable
{
public:
	Bone(const stringc& name, EBodyPart bp);

	~Bone(){};

	void init();

	void addChild(Bone* child);

	BodyPart& getBodyParty() { return m_bodyPart; }

	void updateXForm(Bone* parent)
	{
		core::matrix4 localT;
		localT.setTranslation(m_localPos);
		core::matrix4 localR = m_localRot.getMatrix();
		core::matrix4 local = localT*localR;

		if(parent==0)
			m_worldMatrix = local;
		else
			m_worldMatrix = parent->m_worldMatrix*local;

		m_bodyPart.transformMesh(m_worldMatrix);

		for(int i=0; i<m_childern.size(); ++i)
		{
			m_childern[i]->updateXForm(this);
		}
	}

	//IAnimable interface
	void setPos3D(const core::vector3df& pos)
	{
		m_localPos = pos;
	}

	void setRot3D(const core::quaternion& rot)
	{
		m_localRot = rot;
	}

	bool isMatch(const stringc& name)
	{
		return m_name == name;
	}

private:
	void setBoneDefault();
	void setPartyDefalut();

private:
	stringc m_name;
	EBodyPart m_bp;

	core::vector3df m_localPos; //pos relative to parent
	core::quaternion m_localRot;

	core::matrix4 m_worldMatrix;

	array_t<Bone*> m_childern;

	BodyPart m_bodyPart;

	friend class PicDancer;
};

////////////////////////////////////////////////////////////////////////////////////////

class PicDancer
{
public:
	PicDancer();
	~PicDancer();

	void buildBody(); //create bones and body parts
	void bindAnim(Anim* anim);
	void freeData();

	void update(f32 dt)
	{
		//DBG("test","dt=%f",dt);
		//TODO: animate bones
		static f32 _tr = 0;
		static int _d = 1;
		m_bones[BP_BODY]->m_localPos.set(_tr-180,0,0);		
		m_bones[BP_L_UPPERARM]->m_localRot.fromAngleAxis(_tr*core::DEGTORAD,core::vector3df(0,0,1));
		m_bones[BP_L_FOREARM]->m_localRot.fromAngleAxis(_tr*core::DEGTORAD,core::vector3df(0,0,1));
		m_bones[BP_R_UPPERARM]->m_localRot.fromAngleAxis(-_tr*core::DEGTORAD,core::vector3df(0,0,1));
		m_bones[BP_R_FOREARM]->m_localRot.fromAngleAxis(-_tr*core::DEGTORAD,core::vector3df(0,0,1));
		_tr+=0.1f*dt*_d;
		if(_tr>=360.0f || _tr<=0.0f)
		{
			_tr = flt::core::clamp(_tr,0.0f,360.0f);
			_d*=-1;
		}
			
		if(m_currentAnim!=0)
			m_currentAnim->update(dt);

		m_rootBone->updateXForm(0);
	}

	void render(const renderer::IRendererPtr& renderer, renderer::MaterialPtr& material)
	{
		flt::core::matrix4 mat;
		mat.setTranslation(flt::core::vector3df(0,0,-1000));

		renderer->setWorldTransform(mat);

		m_bones[BP_BODY]->getBodyParty().render(renderer,material);
		m_bones[BP_HEAD]->getBodyParty().render(renderer,material);
		m_bones[BP_L_UPPERARM]->getBodyParty().render(renderer,material);
		m_bones[BP_L_FOREARM]->getBodyParty().render(renderer,material);
		m_bones[BP_R_UPPERARM]->getBodyParty().render(renderer,material);
		m_bones[BP_R_FOREARM]->getBodyParty().render(renderer,material);
	}

	void setTexture(EBodyPart bp, const renderer::TexturePtr& texture)
	{
		m_bones[bp]->getBodyParty().setTexture(texture);
	}

private:
	Bone* createBone(EBodyPart bp);
	
private:	
	Bone* m_bones[BP_PART_COUNT];
	Bone* m_rootBone;
	Anim* m_currentAnim;
};






#endif //BONE_ANIM_H