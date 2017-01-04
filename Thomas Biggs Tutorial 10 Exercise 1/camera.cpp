#include "Camera.h"

Camera::Camera(float x, float y, float z, float camera_rotation)
{
	m_x = x;
	m_y = y;
	m_z = z;

	m_camera_rotation = camera_rotation;

}
Camera::~Camera() 
{

}
void Camera::Rotate(float degrees)
{

	m_camera_rotation = m_camera_rotation + degrees;

}
void Camera::Forward(float distance)
{
	m_x = m_dx*distance;
	m_z = m_dz*distance;
}
XMMATRIX Camera::GetViewMatrix()
{
	m_dx = sin(m_camera_rotation * (XM_PI / 180));
	m_dz = cos(m_camera_rotation * (XM_PI / 180));


	m_position = { m_x, m_y , m_z, 0.0 };
	m_lookat = { m_x + m_dx, m_y, m_z + m_dz, 0.0 };
	m_up = { 0.0f, 1.0f, 0.0f, 0.0 };

	return XMMatrixLookAtLH(  m_position, m_lookat, m_up);




}