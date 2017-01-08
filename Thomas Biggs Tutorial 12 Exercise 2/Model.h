#pragma once

#define _XM_NO_INTRINSICS_
#define XM_NO_ALIGNMENT

#include <d3d11.h>
#include <d3dx11.h>
#include <dxerr.h>
#include <xnamath.h>
#include <stdio.h>
#include <string>
#include <vector>

#include"ObjFileModel.h"

using namespace std;


class Model
{
	private:
		ID3D11Device*			m_pD3DDevice;
		ID3D11DeviceContext*	m_pImmediateContext;

		ObjFileModel*			m_pObject;
		ID3D11VertexShader*		m_pVShader;
		ID3D11PixelShader*		m_pPShader;
		ID3D11InputLayout*		m_pInputLayout;
		ID3D11Buffer*			m_pConstantBuffer;

	
	

		float m_x, m_y, m_z;
		float m_xAngle, m_zAngle, m_yAngle;
		float m_Scale;
		float m_bounding_sphere_centre_x, m_bounding_sphere_centre_y, m_bounding_sphere_centre_z, m_bounding_sphere_radius;
		float maxX = 0, minX = 1000;
		float maxY = 0, minY = 1000;
		float maxZ = 0, minZ = 1000;

	public:
		Model(ID3D11Device* device, ID3D11DeviceContext* context);
		~Model();

		int LoadObjModel(const char* filename); 
		void CompileShaders();
		
		void draw(XMMATRIX* view, XMMATRIX* projection);

		float SetXPos(float num);
		float SetYPos(float num);
		float SetZPos(float num);

		float SetXRotation(float num);
		float SetYRotation(float num);
		float SetZRotation(float num);

		float SetScale(float scale);

		float GetXPos();
		float GetYPos();
		float GetZPos();

		float GetXRotation();
		float GetYRotation();
		float GetZRotation();

		float GetScale();

		float IncXPos();
		float IncYPos();
		float IncZPos();

		void LookAt_ZX(float x, float z);
		void move_forward(float distance);

		void CalculateModelCentrePoint();
		void CalculateBoundingSphereRadius();

		XMVECTOR GetBoundingSphereWorldSpacePosition();
		float GetBoundingSphereRadius();

		bool CheckCollision(Model* model);


};

