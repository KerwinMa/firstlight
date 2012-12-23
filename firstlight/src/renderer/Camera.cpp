#include "Camera.h"

namespace flt
{
namespace renderer
{

Camera::Camera(const core::vector3df &pos, const core::vector3df &target, const core::vector3df &up)
	:m_position(pos),m_target(target),m_up(up),
	 m_fovY(60.f),m_aspect(3.f/2.f),m_nearZ(1.f),m_farZ(3000.f)
{
	updateProjectionMatrix();
	updateViewMatrix();
}

Camera::~Camera()
{

}

void Camera::setPosition(const core::vector3df &pos)
{
	m_position = pos;
	updateViewMatrix();
}

void Camera::setTarget(const core::vector3df &target)
{
	m_target = target;
	updateViewMatrix();
}

void Camera::setUpDirection(const core::vector3df &up)
{
	m_up = up;
	updateViewMatrix();
}

void Camera::set3DProjection(f32 fovy, f32 aspect, f32 nearZ, f32 farZ)
{
	m_fovY = fovy;
	m_aspect = aspect;
	m_nearZ = nearZ;
	m_farZ = farZ;
	updateProjectionMatrix();
}

void Camera::setFOVY(f32 fovy)
{
	m_fovY = fovy;
	updateProjectionMatrix();
}

void Camera::setAspectRatio(f32 ratioWH)
{
	m_aspect = ratioWH;
	updateProjectionMatrix();
}

void Camera::setNearZ(f32 nearZ)
{
	m_nearZ = nearZ;
	updateProjectionMatrix();
}

void Camera::setFarZ(f32 farZ)
{
	m_farZ = farZ;
	updateProjectionMatrix();
}

void Camera::updateProjectionMatrix()
{
	m_projectionMatrix.buildProjectionMatrixPerspectiveFovOpenGL(m_fovY*core::DEGTORAD, m_aspect, m_nearZ, m_farZ);				
}

void Camera::updateViewMatrix()
{
	m_direction = m_target - m_position;
	m_direction.normalize();

	core::vector3df up = m_up;
	up.normalize();

	f32 dp = m_direction.dotProduct(up);

	// if up vector and vector to the target are the same, we have a
	// problem. so solve this problem:
	if (core::equals((float)fabs(dp), 1.0f))
	{
		up.X += 0.5f;
	}

	m_viewMatrix.buildCameraLookAtMatrixRH(m_position, m_target, up);

	m_right.X = m_viewMatrix[0];
	m_right.Y = m_viewMatrix[4];
	m_right.Z = m_viewMatrix[8];
}

}//renderer
}//flt