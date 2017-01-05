#include<d3d11.h>
#include<xnamath.h>
#include<math.h>

#define _XM_NO_INTRINSICS_
#define XM_NO_ALIGNMENT

#pragma once

class Camera
{
private:
	float m_x, m_y, m_z, m_dx, m_dz;
	float m_camera_rotation;

	XMVECTOR m_position, m_lookat, m_up;

public:

	Camera(float x, float y, float z, float camera_rotation);
	~Camera();

	void Rotate(float Degrees);
	void Forward(float disstance);
	XMMATRIX GetViewMatrix();

	float getX();
	float getY();
	float getZ();

	void SetZ(float distance);
};



