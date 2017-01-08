#include"Model.h"

struct MODEL_CONSTANT_BUFFER
{
	XMMATRIX WorldViewProjection;
};
Model::Model(ID3D11Device* device, ID3D11DeviceContext* context)
{
	m_pD3DDevice = device;
	m_pImmediateContext = context;

	m_x = 0.0f;
	m_y = 0.0f;
	m_z = 0.0f;

	m_xAngle = 0.0f;
	m_yAngle = 0.0f;
	m_zAngle = 0.0f;

	m_Scale = 1.0;
}
Model::~Model()
{
	delete m_pObject;
	if (m_pInputLayout) m_pInputLayout->Release();
	if (m_pConstantBuffer) m_pConstantBuffer->Release();
	if (m_pVShader) m_pVShader->Release(); //03-01
	if (m_pPShader) m_pPShader->Release(); // 03-01
}
int Model::LoadObjModel(const char* filename)
{
	
	m_pObject = new ObjFileModel(filename, m_pD3DDevice, m_pImmediateContext); 

	if (m_pObject->filename == "FILE NOT LOADED")
	{
		return S_FALSE;
	}



}
void Model::CompileShaders()
{
	D3D11_BUFFER_DESC constant_buffer_desc;
	ZeroMemory(&constant_buffer_desc, sizeof(constant_buffer_desc));

	constant_buffer_desc.Usage = D3D11_USAGE_DEFAULT;  // can use updatesubresource() to update
	constant_buffer_desc.ByteWidth = 128; // MUST BE A MULTIPLE OF 16, calc from CB struct
	constant_buffer_desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER; // use as constant buffer
	HRESULT hr =
	m_pD3DDevice->CreateBuffer(&constant_buffer_desc, NULL, &m_pConstantBuffer);
	

	ID3DBlob *VS, *PS, *error;
	hr = D3DX11CompileFromFile("model_shaders.hlsl", 0, 0, "ModelVS", "vs_4_0", 0, 0, 0, &VS, &error, 0);
	
	hr = D3DX11CompileFromFile("model_shaders.hlsl", 0, 0, "ModelPS", "ps_4_0", 0, 0, 0, &PS, &error, 0);
	
	hr =
	m_pD3DDevice->CreateVertexShader(VS->GetBufferPointer(), VS->GetBufferSize(), NULL, &m_pVShader);

	hr =
	m_pD3DDevice->CreatePixelShader(PS->GetBufferPointer(), PS->GetBufferSize(), NULL, &m_pPShader);


	D3D11_INPUT_ELEMENT_DESC iedesc[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,0,D3D11_INPUT_PER_VERTEX_DATA,0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,0, D3D11_APPEND_ALIGNED_ELEMENT,D3D11_INPUT_PER_VERTEX_DATA,0 },
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT,0, D3D11_APPEND_ALIGNED_ELEMENT,D3D11_INPUT_PER_VERTEX_DATA,0 },
	};

	m_pD3DDevice->CreateInputLayout(iedesc, ARRAYSIZE(iedesc), VS->GetBufferPointer(), VS->GetBufferSize(), &m_pInputLayout);

}
void Model::draw(XMMATRIX* view, XMMATRIX* projection)
{
	MODEL_CONSTANT_BUFFER model_cb_values;
	XMMATRIX world;
	
	world = XMMatrixRotationZ(XMConvertToRadians(m_zAngle));
	world *= XMMatrixRotationX(XMConvertToRadians(m_xAngle));
	world *= XMMatrixRotationY(XMConvertToRadians(m_yAngle));
	m_Scale = 0.1f;

	world *= XMMatrixScaling(m_Scale, m_Scale, m_Scale);
	world *= XMMatrixTranslation(m_x, m_y, m_z);

	model_cb_values.WorldViewProjection = world*(*view)*(*projection);

	m_pImmediateContext->VSSetConstantBuffers(0, 1, &m_pConstantBuffer);
	m_pImmediateContext->UpdateSubresource(m_pConstantBuffer, 0, 0, &model_cb_values, 0, 0);
	m_pImmediateContext->VSSetShader(m_pVShader, 0, 0);
	m_pImmediateContext->PSSetShader(m_pPShader, 0, 0);
	m_pImmediateContext->IASetInputLayout(m_pInputLayout);

	m_pObject->Draw();
	CalculateModelCentrePoint();
	CalculateBoundingSphereRadius();

}
float Model::SetXPos(float num)
{
	m_x = num;
	return m_x;
}
float Model::SetYPos(float num)
{
	m_y = num;
	return m_y;
}
float Model::SetZPos(float num)
{
	m_z = num;
	return m_z;
}
float Model::SetXRotation(float num)
{
	m_xAngle = num;
	return m_xAngle;
}
float Model::SetYRotation(float num)
{
	m_yAngle = num;
	return m_yAngle;
}
float Model::SetZRotation(float num)
{
	m_zAngle = num;
	return m_zAngle;
}
float Model::SetScale(float num)
{
	m_Scale = num;
	return m_Scale;
}
float Model::GetXPos()
{
	return m_x;
}
float Model::GetYPos()
{
	return m_y;
}
float Model::GetZPos()
{
	return m_z;
}
float Model::GetXRotation()
{
	return m_xAngle;
}
float Model::GetYRotation()
{
	return m_yAngle;
}
float Model::GetZRotation()
{
	return m_zAngle;
}
float Model::GetScale()
{
	return m_Scale;
}
float Model::IncXPos()
{
	m_x = m_x + 0.01;
	return m_x;
}
float Model::IncYPos()
{
	m_y = m_y + 0.01;
	return m_y;
}
float Model::IncZPos()
{
	m_z = m_z + 0.01;
	return m_z;
}
void Model::LookAt_ZX(float x, float z)
{
	//you are here
	m_yAngle = atan2(x, z) * (180.0 / XM_PI);
}
void Model::move_forward(float distance)
{
	m_x += sin(m_yAngle * (XM_PI / 180.0))* distance;
	m_z += cos(m_yAngle * (XM_PI / 180.0))* distance;
}
void Model::CalculateModelCentrePoint()
{
	for (int i = 0; i < m_pObject->numverts; i++)
	{
		m_pObject->vertices[i].Pos.x;

		if (m_pObject->vertices[i].Pos.x > maxX)
			maxX = m_pObject->vertices[i].Pos.x;
		
		if (m_pObject->vertices[i].Pos.x < minX)
			minX = m_pObject->vertices[i].Pos.x;
		
		if (m_pObject->vertices[i].Pos.y > maxY)
			maxY = m_pObject->vertices[i].Pos.y;
		
		if (m_pObject->vertices[i].Pos.y < minY)
			minY = m_pObject->vertices[i].Pos.y;

		if (m_pObject->vertices[i].Pos.z > maxZ)
			maxZ = m_pObject->vertices[i].Pos.z;

		if (m_pObject->vertices[i].Pos.z < minZ)
			minZ = m_pObject->vertices[i].Pos.z;
	}

	m_bounding_sphere_centre_x = (maxX - minX) / 2;
	m_bounding_sphere_centre_y = (maxY - minY) / 2;
	m_bounding_sphere_centre_z = (maxZ - minZ) / 2;

	//return XMFLOAT3(m_bounding_sphere_centre_x, m_bounding_sphere_centre_y, m_bounding_sphere_centre_z);
}
void Model::CalculateBoundingSphereRadius()
{
	if (m_bounding_sphere_centre_x >= m_bounding_sphere_centre_y)
	{
		if (m_bounding_sphere_centre_x >= m_bounding_sphere_centre_z)
		{
			m_bounding_sphere_radius = m_bounding_sphere_centre_x;
		}
	}
	if (m_bounding_sphere_centre_y >= m_bounding_sphere_centre_x)
	{
		if (m_bounding_sphere_centre_y >= m_bounding_sphere_centre_z)
		{
			m_bounding_sphere_radius = m_bounding_sphere_centre_y;
		}
	}
	if (m_bounding_sphere_centre_z >= m_bounding_sphere_centre_x)
	{
		if (m_bounding_sphere_centre_z >= m_bounding_sphere_centre_y)
		{
			m_bounding_sphere_radius = m_bounding_sphere_centre_z;
		}
	}
	printf("Radius: ", &m_bounding_sphere_radius);

}
XMVECTOR Model::GetBoundingSphereWorldSpacePosition()
{
	XMMATRIX world;

	world = XMMatrixRotationZ(XMConvertToRadians(m_zAngle));
	world *= XMMatrixRotationX(XMConvertToRadians(m_xAngle));
	world *= XMMatrixRotationY(XMConvertToRadians(m_yAngle));
	m_Scale = 0.1f;

	world *= XMMatrixScaling(m_Scale, m_Scale, m_Scale);
	world *= XMMatrixTranslation(m_x, m_y, m_z);

	XMVECTOR offset;
	offset = XMVectorSet(m_bounding_sphere_centre_x, m_bounding_sphere_centre_y, m_bounding_sphere_centre_z, 0.0);
	offset = XMVector3Transform(offset, world);

	return offset;
}
float Model::GetBoundingSphereRadius()
{
	return m_bounding_sphere_radius*m_Scale;
}
bool Model::CheckCollision(Model* model)
{
	float x, y, z;
	float x1, y1, z1;
	float xDistance, yDistance, zDistance;
	float distance;
	float sphereRadius = m_bounding_sphere_radius * m_Scale;

	if (model == this)
		return false;
	GetBoundingSphereWorldSpacePosition();
	model->GetBoundingSphereWorldSpacePosition();

	x = GetXPos();
	y = GetYPos();
	z = GetZPos();

	x1 = model->GetXPos();
	y1 = model->GetYPos();
	z1 = model->GetZPos();

	xDistance = (x - x1);
	yDistance = (y - y1);
	zDistance = (z - z1);

	distance = sqrt((xDistance * xDistance) + (yDistance * yDistance) + (zDistance * zDistance));
		
	if (distance < sphereRadius * 2)
		
	{
		return true;
	}
	else
	{
		return false;
	}


}