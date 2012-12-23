#include "boneAnim.h"

///////////////// Bone //////////////////////////

const char* _bone_names[] =
{
	"head",
	"body",
	"lupperarm",
	"lforearm",
	"rupperarm",
	"rforearm",
};

const float HEAD_HS = 40.0f;
const float BODY_HS = 90.0f;
const float ARM_HW = 20.0f;
const float ARM_LEN = 80.0f;

Bone::Bone(const stringc& name, EBodyPart bp)
{
	m_name = name;
	m_bp = bp;	
}

void Bone::init()
{
	setBoneDefault();
	setPartyDefalut();
}

void Bone::addChild(Bone* child)
{
	m_childern.push_back(child);
}

const float _bone_default_data[BP_PART_COUNT][3] =
{
	//x, y, rotDegree
	//head
	0, (HEAD_HS+BODY_HS), 0.0f,
	//body
	0, 0, 0,
	//left upper arm
	-BODY_HS,	BODY_HS,	180.0f,
	//left fore arm
	ARM_LEN,	0.0f,		0.0f,
	//right upper arm
	BODY_HS,	BODY_HS,	0.0f,
	//right fore arm
	ARM_LEN,	0.0f,		0.0f,
};

void Bone::setBoneDefault()
{
	FLT_ASSERT(m_bp<BP_PART_COUNT);

	m_localPos.set(_bone_default_data[m_bp][0], _bone_default_data[m_bp][1], 0.0f);
	m_localRot.fromAngleAxis(_bone_default_data[m_bp][2]*core::DEGTORAD, core::vector3df(0,0,1));
}



//  0----2
//  |  / |
//  | /  |
//  1----3
//

const float _part_default_data[BP_PART_COUNT][8] =
{
	//p0(xy),p1,p2,p3

	//head
	{ 
		-HEAD_HS,HEAD_HS, -HEAD_HS,-HEAD_HS, HEAD_HS,HEAD_HS, HEAD_HS,-HEAD_HS,
	},
	//body
	{ 
		-BODY_HS,BODY_HS, -BODY_HS,-BODY_HS, BODY_HS,BODY_HS, BODY_HS,-BODY_HS,
	},
	//left-upper-arm
	{
		0,ARM_HW, 0,-ARM_HW, ARM_LEN, ARM_HW, ARM_LEN, -ARM_HW,
	},
	//left-fore-arm
	{
		0,ARM_HW, 0,-ARM_HW, ARM_LEN, ARM_HW, ARM_LEN, -ARM_HW,
	},
	//right-upper-arm
	{
		0,ARM_HW, 0,-ARM_HW, ARM_LEN, ARM_HW, ARM_LEN, -ARM_HW,
	},
	//right-fore-arm
	{
		0,ARM_HW, 0,-ARM_HW, ARM_LEN, ARM_HW, ARM_LEN, -ARM_HW,
	},

};

void Bone::setPartyDefalut()
{
	FLT_ASSERT(m_bp<BP_PART_COUNT);

	m_bodyPart.setBonePosRot(_part_default_data[m_bp][0],_part_default_data[m_bp][1],
							_part_default_data[m_bp][2],_part_default_data[m_bp][3],
							_part_default_data[m_bp][4],_part_default_data[m_bp][5],
							_part_default_data[m_bp][6],_part_default_data[m_bp][7]);
}


PicDancer::PicDancer()
{
	for(int i=0; i<BP_PART_COUNT; ++i)
	{
		m_bones[i] = 0;
	}

	m_currentAnim = 0;
}

PicDancer::~PicDancer()
{
	freeData();
}


void PicDancer::buildBody()
{
	createBone(BP_HEAD);
	createBone(BP_BODY);
	createBone(BP_L_UPPERARM);
	createBone(BP_L_FOREARM);
	createBone(BP_R_UPPERARM);
	createBone(BP_R_FOREARM);

	m_bones[BP_BODY]->addChild(m_bones[BP_HEAD]);
	m_bones[BP_BODY]->addChild(m_bones[BP_L_UPPERARM]);
	m_bones[BP_BODY]->addChild(m_bones[BP_R_UPPERARM]);
	m_bones[BP_L_UPPERARM]->addChild(m_bones[BP_L_FOREARM]);
	m_bones[BP_R_UPPERARM]->addChild(m_bones[BP_R_FOREARM]);
	
	m_rootBone = m_bones[BP_BODY];

	update(0);
}

void PicDancer::bindAnim(Anim* anim)
{
	for(int i=0; i<BP_PART_COUNT; i++)
	{
		anim->bindTracksTo(m_bones[i]);
	}

	m_currentAnim = anim;
	m_currentAnim->start(true);
}

void PicDancer::freeData()
{
	for(int i=0; i<BP_PART_COUNT; ++i)
	{
		delete m_bones[i];
		m_bones[i] = 0;		
	}	
}

Bone* PicDancer::createBone(EBodyPart bp)
{
	if(bp>=BP_PART_COUNT)
	{
		return 0;
	}

	Bone* bone = new Bone(_bone_names[bp],bp);
	if(m_bones[bp]!=0)
	{
		delete m_bones[bp];
	}	

	m_bones[bp] = bone;

	bone->init();

	return bone;
}



