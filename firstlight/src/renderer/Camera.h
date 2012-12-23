#ifndef FLT_CAMERA_H
#define FLT_CAMERA_H

#include "BaseObject.h"
#include "sharePtr.h"
#include "core/matrix4.h"

namespace flt
{
namespace renderer
{

	FLT_FORWARD_PTR(Camera)

	class Camera: public BaseObject
	{
	public:	
		Camera(const core::vector3df &pos = core::vector3df(0.f,0.f,0.f),
				const core::vector3df &target = core::vector3df(0.f,100.f,0.f),
				const core::vector3df &up = core::vector3df(0.f,0.f,1.f));
		virtual ~Camera();

		const core::vector3df& getPosition() const { return m_position; }
		void setPosition(const core::vector3df &pos);
		void setTarget(const core::vector3df &target);
		void setUpDirection(const core::vector3df &up);

		void set3DProjection(f32 fovy, f32 aspect, f32 nearZ, f32 farZ);
		void setFOVY(f32 fovy);
		void setAspectRatio(f32 ratioWH);
		void setNearZ(f32 nearZ);
		void setFarZ(f32 farZ);
		
		const core::matrix4& getProjectionMatrix() const { return m_projectionMatrix; }
		const core::matrix4& getViewMatrix() const { return m_viewMatrix; }
		const core::vector3df& getDirection() const { return m_direction; }
		const core::vector3df& getRight() const { return m_right; }
		const core::vector3df& getUp() const { return m_up; }
		

	protected:
		void updateProjectionMatrix();
		void updateViewMatrix();

		core::vector3df m_position;
		core::vector3df m_target;
		core::vector3df m_up;

		f32 m_fovY;
		f32 m_aspect;
		f32 m_nearZ;
		f32 m_farZ;

		core::vector3df m_direction;
		core::vector3df m_right;

		core::matrix4 m_viewMatrix;
		core::matrix4 m_projectionMatrix;

	};

} //end namespace renderer
} //end namespace flt

#endif //FLT_CAMERA_H