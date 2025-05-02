#include "stdafx.h"
#include "Player.h"

/////////////////////////////////////////////////////////////////////////////////////////////////////
//
CPlayer::CPlayer()
{
}

CPlayer::~CPlayer()
{
	if (m_pCamera) delete m_pCamera;
}

void CPlayer::SetPosition(float x, float y, float z)
{
	m_xmf3Position = XMFLOAT3(x, y, z);

	CGameObject::SetPosition(x, y, z);
}

void CPlayer::SetCameraOffset(XMFLOAT3& xmf3CameraOffset)
{
	m_xmf3CameraOffset = xmf3CameraOffset;
	m_pCamera->SetLookAt(Vector3::Add(m_xmf3Position, m_xmf3CameraOffset), m_xmf3Position, m_xmf3Up);
	m_pCamera->GenerateViewMatrix();
}

void CPlayer::Move(DWORD dwDirection, float fDistance)
{
	if (dwDirection)
	{
		XMFLOAT3 xmf3Shift = XMFLOAT3(0, 0, 0);
		if (dwDirection & DIR_FORWARD) xmf3Shift = Vector3::Add(xmf3Shift, m_xmf3Look, fDistance);
		if (dwDirection & DIR_BACKWARD) xmf3Shift = Vector3::Add(xmf3Shift, m_xmf3Look, -fDistance);
		if (dwDirection & DIR_RIGHT) xmf3Shift = Vector3::Add(xmf3Shift, m_xmf3Right, fDistance);
		if (dwDirection & DIR_LEFT) xmf3Shift = Vector3::Add(xmf3Shift, m_xmf3Right, -fDistance);
		if (dwDirection & DIR_UP) xmf3Shift = Vector3::Add(xmf3Shift, m_xmf3Up, fDistance);
		if (dwDirection & DIR_DOWN) xmf3Shift = Vector3::Add(xmf3Shift, m_xmf3Up, -fDistance);

		Move(xmf3Shift, true);
	}
}

void CPlayer::Move(XMFLOAT3& xmf3Shift, bool bUpdateVelocity)
{
	if (bUpdateVelocity)
	{
		m_xmf3Velocity = Vector3::Add(m_xmf3Velocity, xmf3Shift);
	}
	else
	{
		m_xmf3Position = Vector3::Add(xmf3Shift, m_xmf3Position);
		m_pCamera->Move(xmf3Shift);
	}
}

void CPlayer::Move(float x, float y, float z)
{
	Move(XMFLOAT3(x, y, z), false);
}

void CPlayer::Rotate(float fPitch, float fYaw, float fRoll)
{
	m_pCamera->Rotate(fPitch, fYaw, fRoll);
	if (fPitch != 0.0f)
	{
		XMMATRIX mtxRotate = XMMatrixRotationAxis(XMLoadFloat3(&m_xmf3Right), XMConvertToRadians(fPitch));
		m_xmf3Look = Vector3::TransformNormal(m_xmf3Look, mtxRotate);
		m_xmf3Up = Vector3::TransformNormal(m_xmf3Up, mtxRotate);
	}
	if (fYaw != 0.0f)
	{
		XMMATRIX mtxRotate = XMMatrixRotationAxis(XMLoadFloat3(&m_xmf3Up), XMConvertToRadians(fYaw));
		m_xmf3Look = Vector3::TransformNormal(m_xmf3Look, mtxRotate);
		m_xmf3Right = Vector3::TransformNormal(m_xmf3Right, mtxRotate);
	}
	if (fRoll != 0.0f)
	{
		XMMATRIX mtxRotate = XMMatrixRotationAxis(XMLoadFloat3(&m_xmf3Look), XMConvertToRadians(fRoll));
		m_xmf3Up = Vector3::TransformNormal(m_xmf3Up, mtxRotate);
		m_xmf3Right = Vector3::TransformNormal(m_xmf3Right, mtxRotate);
	}

	m_xmf3Look = Vector3::Normalize(m_xmf3Look);
	m_xmf3Right = Vector3::Normalize(Vector3::CrossProduct(m_xmf3Up, m_xmf3Look));
	m_xmf3Up = Vector3::Normalize(Vector3::CrossProduct(m_xmf3Look, m_xmf3Right));
}

void CPlayer::Rotate(XMFLOAT3& xmf3RotationAxis, float fAngle)
{
	XMMATRIX mtxRotate = XMMatrixRotationAxis(XMLoadFloat3(&xmf3RotationAxis), XMConvertToRadians(fAngle));

	m_xmf3Look = Vector3::TransformNormal(m_xmf3Look, mtxRotate);
	m_xmf3Up = Vector3::TransformNormal(m_xmf3Up, mtxRotate);
	m_xmf3Right = Vector3::TransformNormal(m_xmf3Right, mtxRotate);

	m_xmf3Look = Vector3::Normalize(m_xmf3Look);
	m_xmf3Up = Vector3::Normalize(m_xmf3Up);
	m_xmf3Right = Vector3::Normalize(m_xmf3Right);

	m_pCamera->Rotate(xmf3RotationAxis, fAngle);
}

void CPlayer::SetOrientation(const XMFLOAT3& right, const XMFLOAT3& up, const XMFLOAT3& look)
{
	m_xmf3Right = right;
	m_xmf3Up = up;
	m_xmf3Look = look;
}

void CPlayer::LookAt(XMFLOAT3& xmf3LookAt, XMFLOAT3& xmf3Up)
{
	XMFLOAT4X4 xmf4x4View = Matrix4x4::LookAtLH(m_xmf3Position, xmf3LookAt, xmf3Up);
	m_xmf3Right = Vector3::Normalize(XMFLOAT3(xmf4x4View._11, xmf4x4View._21, xmf4x4View._31));
	m_xmf3Up = Vector3::Normalize(XMFLOAT3(xmf4x4View._12, xmf4x4View._22, xmf4x4View._32));
	m_xmf3Look = Vector3::Normalize(XMFLOAT3(xmf4x4View._13, xmf4x4View._23, xmf4x4View._33));
}

void CPlayer::Update(float fTimeElapsed)
{
	Move(m_xmf3Velocity, false);

	m_pCamera->Update(this, m_xmf3Position, fTimeElapsed);
	m_pCamera->GenerateViewMatrix();

	XMFLOAT3 xmf3Deceleration = Vector3::Normalize(Vector3::ScalarProduct(m_xmf3Velocity, -1.0f));
	float fLength = Vector3::Length(m_xmf3Velocity);
	float fDeceleration = m_fFriction * fTimeElapsed;
	if (fDeceleration > fLength) fDeceleration = fLength;
	m_xmf3Velocity = Vector3::Add(m_xmf3Velocity, xmf3Deceleration, fDeceleration);
}

void CPlayer::Animate(float fElapsedTime)
{
	OnUpdateTransform();

	CGameObject::Animate(fElapsedTime);
}

void CPlayer::OnUpdateTransform()
{
	m_xmf4x4World._11 = m_xmf3Right.x; m_xmf4x4World._12 = m_xmf3Right.y; m_xmf4x4World._13 = m_xmf3Right.z;
	m_xmf4x4World._21 = m_xmf3Up.x; m_xmf4x4World._22 = m_xmf3Up.y; m_xmf4x4World._23 = m_xmf3Up.z;
	m_xmf4x4World._31 = m_xmf3Look.x; m_xmf4x4World._32 = m_xmf3Look.y; m_xmf4x4World._33 = m_xmf3Look.z;
	m_xmf4x4World._41 = m_xmf3Position.x; m_xmf4x4World._42 = m_xmf3Position.y; m_xmf4x4World._43 = m_xmf3Position.z;
}

void CPlayer::Render(HDC hDCFrameBuffer, CCamera* pCamera)
{
	CGameObject::Render(hDCFrameBuffer, pCamera);
}

/////////////////////////////////////////////////////////////////////////////////////////////
//
CAirplanePlayer::CAirplanePlayer()
{
	CCubeMesh* pBulletMesh = new CCubeMesh(1.0f, 4.0f, 1.0f);
	for (int i = 0; i < BULLETS; i++)
	{
		m_ppBullets[i] = new CBulletObject(m_fBulletEffectiveRange);
		m_ppBullets[i]->SetMesh(pBulletMesh);
		m_ppBullets[i]->SetRotationAxis(XMFLOAT3(0.0f, 1.0f, 0.0f));
		m_ppBullets[i]->SetRotationSpeed(360.0f);
		m_ppBullets[i]->SetMovingSpeed(120.0f);
		m_ppBullets[i]->SetActive(false);
	}
}

CAirplanePlayer::~CAirplanePlayer()
{
	for (int i = 0; i < BULLETS; i++) if (m_ppBullets[i]) delete m_ppBullets[i];
}

void CAirplanePlayer::Animate(float fElapsedTime)
{
	CPlayer::Animate(fElapsedTime);

	for (int i = 0; i < BULLETS; i++)
	{
		if (m_ppBullets[i]->m_bActive) m_ppBullets[i]->Animate(fElapsedTime);
	}
}

void CAirplanePlayer::OnUpdateTransform()
{
	CPlayer::OnUpdateTransform();

	m_xmf4x4World = Matrix4x4::Multiply(XMMatrixRotationRollPitchYaw(XMConvertToRadians(90.0f), 0.0f, 0.0f), m_xmf4x4World);
}

void CAirplanePlayer::Render(HDC hDCFrameBuffer, CCamera* pCamera)
{
	CPlayer::Render(hDCFrameBuffer, pCamera);

	for (int i = 0; i < BULLETS; i++) if (m_ppBullets[i]->m_bActive) m_ppBullets[i]->Render(hDCFrameBuffer, pCamera);
}

void CAirplanePlayer::FireBullet(CGameObject* pLockedObject)
{
	CBulletObject* pBulletObject = NULL;
	for (int i = 0; i < BULLETS; i++)
	{
		if (!m_ppBullets[i]->m_bActive)
		{
			pBulletObject = m_ppBullets[i];
			break;
		}
	}

	if (pBulletObject)
	{
		XMFLOAT3 xmf3Position = GetPosition();
		XMFLOAT3 xmf3Direction = GetUp();
		XMFLOAT3 xmf3FirePosition = Vector3::Add(xmf3Position, Vector3::ScalarProduct(xmf3Direction, 6.0f, false));

		pBulletObject->m_xmf4x4World = m_xmf4x4World;

		pBulletObject->SetFirePosition(xmf3FirePosition);
		pBulletObject->SetMovingDirection(xmf3Direction);
		pBulletObject->SetColor(RGB(255, 0, 0));
		pBulletObject->SetActive(true);

		if (pLockedObject)
		{
			pBulletObject->m_pLockedObject = pLockedObject;
			pBulletObject->SetColor(RGB(0, 0, 255));
		}
	}
}

/////////////////////////////////////////////////////////////////////////////////////////////

CTankPlayer::CTankPlayer()
{
	CCubeMesh* pBulletMesh = new CCubeMesh(2.0f, 2.0f, 2.0f);
	for (int i = 0; i < BULLETS; i++)
	{
		m_ppBullets[i] = new CBulletObject(m_fBulletEffectiveRange);
		m_ppBullets[i]->SetMesh(pBulletMesh);
		m_ppBullets[i]->SetRotationAxis(XMFLOAT3(0.0f, 1.0f, 0.0f));
		m_ppBullets[i]->SetRotationSpeed(0.0f);
		m_ppBullets[i]->SetMovingSpeed(120.0f);
		m_ppBullets[i]->SetActive(false);
	}
}

CTankPlayer::~CTankPlayer()
{
	m_pMeshLowerBody->Release();
	m_pMeshLowerBody = nullptr;
	m_pMeshUpperBody->Release();
	m_pMeshUpperBody = nullptr;
	m_pMeshTurret->Release();
	m_pMeshTurret = nullptr;

	for (int i = 0; i < BULLETS; i++) if (m_ppBullets[i]) delete m_ppBullets[i];
}

void CTankPlayer::SetCameraOffset(XMFLOAT3& xmf3CameraOffset)
{
	m_xmf3BaseCameraOffset = xmf3CameraOffset;
	m_xmf3CameraOffset = m_xmf3BaseCameraOffset;
	m_pCamera->SetLookAt(Vector3::Add(m_xmf3Position, m_xmf3CameraOffset), m_xmf3Position, m_xmf3Up);
	m_pCamera->GenerateViewMatrix();
}

void CTankPlayer::Rotate(float fYaw)
{
	XMMATRIX mtxRotate = XMMatrixRotationY(XMConvertToRadians(fYaw));

	m_xmf3Look = Vector3::TransformNormal(m_xmf3Look, mtxRotate);
	m_xmf3Right = Vector3::TransformNormal(m_xmf3Right, mtxRotate);

	m_xmf3Look = Vector3::Normalize(m_xmf3Look);
	m_xmf3Right = Vector3::Normalize(m_xmf3Right);
}

void CTankPlayer::Animate(float fElapsedTime)
{
	if (!m_bActive) return;

	UpdateTopParts();

	CPlayer::Animate(fElapsedTime);

	for (int i = 0; i < BULLETS; i++)
	{
		if (m_ppBullets[i]->m_bActive) m_ppBullets[i]->Animate(fElapsedTime);
	}

	if (m_pCamera)
	{
		m_pCamera->SetLookAt(Vector3::Add(m_xmf3Position, m_xmf3CameraOffset), m_xmf3Position, XMFLOAT3(0.0f, 1.0f, 0.0f));
		m_pCamera->GenerateViewMatrix();
	}

	UpdateBoundingBox();
}

void CTankPlayer::Render(HDC hDCFrameBuffer, CCamera* pCamera)
{
	if (!m_bActive) return;

	HPEN hPen = ::CreatePen(PS_SOLID, 0, m_dwColor);
	HPEN hOldPen = (HPEN)::SelectObject(hDCFrameBuffer, hPen);

	if (m_pMeshLowerBody)
	{
		CGraphicsPipeline::SetWorldTransform(&m_xmf4x4World);
		m_pMeshLowerBody->Render(hDCFrameBuffer);
	}

	if (m_pMeshUpperBody)
	{
		XMFLOAT4X4 xmf4TopWorld = m_xmf4x4World;
		xmf4TopWorld._11 = m_xmf3TopRight.x; xmf4TopWorld._12 = m_xmf3TopRight.y; xmf4TopWorld._13 = m_xmf3TopRight.z;
		xmf4TopWorld._21 = m_xmf3TopUp.x;    xmf4TopWorld._22 = m_xmf3TopUp.y;    xmf4TopWorld._23 = m_xmf3TopUp.z;
		xmf4TopWorld._31 = m_xmf3TopLook.x;  xmf4TopWorld._32 = m_xmf3TopLook.y;  xmf4TopWorld._33 = m_xmf3TopLook.z;

		xmf4TopWorld._42 += m_fBottomHeight;

		CGraphicsPipeline::SetWorldTransform(&xmf4TopWorld);
		m_pMeshUpperBody->Render(hDCFrameBuffer);
	}

	if (m_pMeshTurret)
	{
		XMFLOAT4X4 xmf4CannonWorld = Matrix4x4::Identity();
		xmf4CannonWorld._11 = m_xmf3TopRight.x; xmf4CannonWorld._12 = m_xmf3TopRight.y; xmf4CannonWorld._13 = m_xmf3TopRight.z;
		xmf4CannonWorld._21 = m_xmf3TopUp.x;    xmf4CannonWorld._22 = m_xmf3TopUp.y;    xmf4CannonWorld._23 = m_xmf3TopUp.z;
		xmf4CannonWorld._31 = m_xmf3TopLook.x;  xmf4CannonWorld._32 = m_xmf3TopLook.y;  xmf4CannonWorld._33 = m_xmf3TopLook.z;

		XMFLOAT3 basePos = GetPosition();
		basePos.y += m_fBottomHeight + m_fUpperHeight * 0.1f;
		basePos = Vector3::Add(basePos, Vector3::ScalarProduct(m_xmf3TopLook, m_fUpperWidth));

		xmf4CannonWorld._41 = basePos.x;
		xmf4CannonWorld._42 = basePos.y;
		xmf4CannonWorld._43 = basePos.z;

		CGraphicsPipeline::SetWorldTransform(&xmf4CannonWorld);
		m_pMeshTurret->Render(hDCFrameBuffer);
	}

	::SelectObject(hDCFrameBuffer, hOldPen);
	::DeleteObject(hPen);

	for (int i = 0; i < BULLETS; i++) if (m_ppBullets[i]->m_bActive) m_ppBullets[i]->Render(hDCFrameBuffer, pCamera);
}


void CTankPlayer::SetTankMesh(CMesh* pLowerBodyMesh, CMesh* pUpperBodyMesh, CMesh* pTurretMesh)
{
	if (!pLowerBodyMesh || !pUpperBodyMesh || !pTurretMesh) return;
	m_pMeshLowerBody = pLowerBodyMesh;
	pLowerBodyMesh->AddRef();
	m_pMeshUpperBody = pUpperBodyMesh;
	pUpperBodyMesh->AddRef();
	m_pMeshTurret = pTurretMesh;
	pTurretMesh->AddRef();

	BoundingOrientedBox boxLower = m_pMeshLowerBody->m_xmOOBB;
	BoundingOrientedBox boxUpper = m_pMeshUpperBody->m_xmOOBB;

	boxUpper.Center.y += m_fBottomHeight;

	XMFLOAT3 minPos = {
		min(boxLower.Center.x - boxLower.Extents.x, boxUpper.Center.x - boxUpper.Extents.x),
		min(boxLower.Center.y - boxLower.Extents.y, boxUpper.Center.y - boxUpper.Extents.y),
		min(boxLower.Center.z - boxLower.Extents.z, boxUpper.Center.z - boxUpper.Extents.z)
	};

	XMFLOAT3 maxPos = {
		max(boxLower.Center.x + boxLower.Extents.x, boxUpper.Center.x + boxUpper.Extents.x),
		max(boxLower.Center.y + boxLower.Extents.y, boxUpper.Center.y + boxUpper.Extents.y),
		max(boxLower.Center.z + boxLower.Extents.z, boxUpper.Center.z + boxUpper.Extents.z)
	};

	XMFLOAT3 center = {
		(minPos.x + maxPos.x) * 0.5f,
		(minPos.y + maxPos.y) * 0.5f,
		(minPos.z + maxPos.z) * 0.5f
	};

	XMFLOAT3 extents = {
		(maxPos.x - minPos.x) * 0.5f,
		(maxPos.y - minPos.y) * 0.5f,
		(maxPos.z - minPos.z) * 0.5f
	};

	m_xmOOBB.Center = center;
	m_xmOOBB.Extents = extents;
	XMStoreFloat4(&m_xmOOBB.Orientation, XMQuaternionIdentity());
}

void CTankPlayer::RotateCameraOffset(float fAngleDegree)
{
	XMFLOAT3 offset = m_xmf3CameraOffset;

	float height = offset.y;

	XMFLOAT3 flatOffset(offset.x, 0.0f, offset.z);
	XMMATRIX rotateMatrix = XMMatrixRotationY(XMConvertToRadians(fAngleDegree));
	XMVECTOR vFlatOffset = XMLoadFloat3(&flatOffset);
	vFlatOffset = XMVector3TransformCoord(vFlatOffset, rotateMatrix);

	XMFLOAT3 rotatedFlatOffset;
	XMStoreFloat3(&rotatedFlatOffset, vFlatOffset);

	m_xmf3CameraOffset = XMFLOAT3(rotatedFlatOffset.x, height, rotatedFlatOffset.z);

	m_pCamera->SetLookAt(Vector3::Add(m_xmf3Position, m_xmf3CameraOffset), m_xmf3Position, m_xmf3Up);
	m_pCamera->GenerateViewMatrix();
}


void CTankPlayer::UpdateTopParts()
{
	if (!m_pCamera) return;

	XMFLOAT3 camLook = m_pCamera->GetLook();

	camLook.y = 0.0f;

	camLook = Vector3::Normalize(camLook);

	m_xmf3TopLook = camLook;

	m_xmf3TopUp = XMFLOAT3(0.0f, 1.0f, 0.0f);
	m_xmf3TopRight = Vector3::Normalize(Vector3::CrossProduct(m_xmf3TopUp, m_xmf3TopLook));
}

void CTankPlayer::ToggleShield()
{
	m_bShield = !m_bShield;
	if (m_bShield)
		SetColor(RGB(0, 0, 0));
	else
		SetColor(RGB(200, 100, 0));
}

void CTankPlayer::AutoFire(float fElapsedTime,CGameObject* pLockedObject)
{
	if (m_bAutoFire)
	{
		m_fAutoFireElapsed += fElapsedTime;
		if (m_fAutoFireElapsed >= m_fAutoFireInterval)
		{
			FireBullet(pLockedObject);
			m_fAutoFireElapsed = 0.0f;
		}
	}
}

void CTankPlayer::FireBullet(CGameObject* pLockedObject)
{
	CBulletObject* pBulletObject = NULL;

	for (int i = 0; i < BULLETS; i++)
	{
		if (!m_ppBullets[i]->m_bActive)
		{
			pBulletObject = m_ppBullets[i];
			break;
		}
	}

	if (pBulletObject)
	{
		XMFLOAT3 firePos = GetPosition();
		firePos.y += m_fBottomHeight + m_fUpperHeight * 0.1f;
		firePos = Vector3::Add(firePos, Vector3::ScalarProduct(m_xmf3TopLook, m_fUpperWidth));

		pBulletObject->SetFirePosition(firePos);
		pBulletObject->SetMovingDirection(m_xmf3TopLook);
		pBulletObject->SetColor(RGB(255, 0, 0));
		pBulletObject->SetActive(true);

		if (pLockedObject)
		{
			pBulletObject->m_pLockedObject = pLockedObject;
			pBulletObject->SetColor(RGB(0, 0, 255));
		}
	}
}

void CTankPlayer::UpdateBoundingBox()
{
	if (m_pMeshLowerBody && m_pMeshUpperBody)
	{
		BoundingOrientedBox obbLower = m_pMeshLowerBody->m_xmOOBB;
		BoundingOrientedBox obbUpper = m_pMeshUpperBody->m_xmOOBB;

		obbLower.Transform(obbLower, XMLoadFloat4x4(&m_xmf4x4World));

		XMFLOAT4X4 upperWorld = m_xmf4x4World;
		upperWorld._42 += m_fBottomHeight;
		obbUpper.Transform(obbUpper, XMLoadFloat4x4(&upperWorld));

		BoundingBox aabb;
		BoundingBox::CreateMerged(aabb, BoundingBox(obbLower.Center, obbLower.Extents), BoundingBox(obbUpper.Center, obbUpper.Extents));

		BoundingOrientedBox::CreateFromBoundingBox(m_xmOOBB, aabb);

		m_xmOOBB.Orientation = XMFLOAT4(0, 0, 0, 1);
	}
}