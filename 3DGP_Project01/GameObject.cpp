#include "stdafx.h"
#include "GameObject.h"
#include "GraphicsPipeline.h"
#include "TextGlyphTable.h"

/////////////////////////////////////////////////////////////////////////////////////////////////////
//
inline float RandF(float fMin, float fMax)
{
	return(fMin + ((float)rand() / (float)RAND_MAX) * (fMax - fMin));
}

XMVECTOR RandomUnitVectorOnSphere()
{
	XMVECTOR xmvOne = XMVectorSet(1.0f, 1.0f, 1.0f, 1.0f);
	XMVECTOR xmvZero = XMVectorZero();

	while (true)
	{
		XMVECTOR v = XMVectorSet(RandF(-1.0f, 1.0f), RandF(-1.0f, 1.0f), RandF(-1.0f, 1.0f), 0.0f);
		if (!XMVector3Greater(XMVector3LengthSq(v), xmvOne)) return(XMVector3Normalize(v));
	}
}


CGameObject::~CGameObject(void)
{
	if (m_pMesh) m_pMesh->Release();
}

void CGameObject::SetPosition(float x, float y, float z)
{
	m_xmf4x4World._41 = x;
	m_xmf4x4World._42 = y;
	m_xmf4x4World._43 = z;
}

void CGameObject::SetPosition(XMFLOAT3& xmf3Position)
{
	m_xmf4x4World._41 = xmf3Position.x;
	m_xmf4x4World._42 = xmf3Position.y;
	m_xmf4x4World._43 = xmf3Position.z;
}

XMFLOAT3 CGameObject::GetPosition()
{
	return(XMFLOAT3(m_xmf4x4World._41, m_xmf4x4World._42, m_xmf4x4World._43));
}

XMFLOAT3 CGameObject::GetLook()
{
	XMFLOAT3 xmf3LookAt(m_xmf4x4World._31, m_xmf4x4World._32, m_xmf4x4World._33);
	xmf3LookAt = Vector3::Normalize(xmf3LookAt);
	return(xmf3LookAt);
}

XMFLOAT3 CGameObject::GetUp()
{
	XMFLOAT3 xmf3Up(m_xmf4x4World._21, m_xmf4x4World._22, m_xmf4x4World._23);
	xmf3Up = Vector3::Normalize(xmf3Up);
	return(xmf3Up);
}

XMFLOAT3 CGameObject::GetRight()
{
	XMFLOAT3 xmf3Right(m_xmf4x4World._11, m_xmf4x4World._12, m_xmf4x4World._13);
	xmf3Right = Vector3::Normalize(xmf3Right);
	return(xmf3Right);
}

void CGameObject::SetWorldMatrix(const XMFLOAT4X4& xmf4x4World)
{
	m_xmf4x4World = xmf4x4World;
}

void CGameObject::SetRotationTransform(XMFLOAT4X4* pmxf4x4Transform)
{
	m_xmf4x4World._11 = pmxf4x4Transform->_11; m_xmf4x4World._12 = pmxf4x4Transform->_12; m_xmf4x4World._13 = pmxf4x4Transform->_13;
	m_xmf4x4World._21 = pmxf4x4Transform->_21; m_xmf4x4World._22 = pmxf4x4Transform->_22; m_xmf4x4World._23 = pmxf4x4Transform->_23;
	m_xmf4x4World._31 = pmxf4x4Transform->_31; m_xmf4x4World._32 = pmxf4x4Transform->_32; m_xmf4x4World._33 = pmxf4x4Transform->_33;
}

void CGameObject::MoveStrafe(float fDistance)
{
	XMFLOAT3 xmf3Position = GetPosition();
	XMFLOAT3 xmf3Right = GetRight();
	xmf3Position = Vector3::Add(xmf3Position, Vector3::ScalarProduct(xmf3Right, fDistance));

	CGameObject::SetPosition(xmf3Position);
}

void CGameObject::MoveUp(float fDistance)
{
	XMFLOAT3 xmf3Position = GetPosition();
	XMFLOAT3 xmf3Up = GetUp();
	xmf3Position = Vector3::Add(xmf3Position, Vector3::ScalarProduct(xmf3Up, fDistance));

	CGameObject::SetPosition(xmf3Position);
}

void CGameObject::MoveForward(float fDistance)
{
	XMFLOAT3 xmf3Position = GetPosition();
	XMFLOAT3 xmf3LookAt = GetLook();
	xmf3Position = Vector3::Add(xmf3Position, Vector3::ScalarProduct(xmf3LookAt, fDistance));

	CGameObject::SetPosition(xmf3Position);
}

void CGameObject::Rotate(float fPitch, float fYaw, float fRoll)
{
	XMFLOAT4X4 mtxRotate = Matrix4x4::RotationYawPitchRoll(fPitch, fYaw, fRoll);
	m_xmf4x4World = Matrix4x4::Multiply(mtxRotate, m_xmf4x4World);
}

void CGameObject::Rotate(XMFLOAT3& xmf3RotationAxis, float fAngle)
{
	XMFLOAT4X4 mtxRotate = Matrix4x4::RotationAxis(xmf3RotationAxis, fAngle);
	m_xmf4x4World = Matrix4x4::Multiply(mtxRotate, m_xmf4x4World);
}

void CGameObject::Move(XMFLOAT3& vDirection, float fSpeed)
{
	SetPosition(m_xmf4x4World._41 + vDirection.x * fSpeed, m_xmf4x4World._42 + vDirection.y * fSpeed, m_xmf4x4World._43 + vDirection.z * fSpeed);
}

void CGameObject::LookTo(XMFLOAT3& xmf3LookTo, XMFLOAT3& xmf3Up)
{
	XMFLOAT4X4 xmf4x4View = Matrix4x4::LookToLH(GetPosition(), xmf3LookTo, xmf3Up);
	m_xmf4x4World._11 = xmf4x4View._11; m_xmf4x4World._12 = xmf4x4View._21; m_xmf4x4World._13 = xmf4x4View._31;
	m_xmf4x4World._21 = xmf4x4View._12; m_xmf4x4World._22 = xmf4x4View._22; m_xmf4x4World._23 = xmf4x4View._32;
	m_xmf4x4World._31 = xmf4x4View._13; m_xmf4x4World._32 = xmf4x4View._23; m_xmf4x4World._33 = xmf4x4View._33;
}

void CGameObject::LookAt(XMFLOAT3& xmf3LookAt, XMFLOAT3& xmf3Up)
{
	XMFLOAT4X4 xmf4x4View = Matrix4x4::LookAtLH(GetPosition(), xmf3LookAt, xmf3Up);
	m_xmf4x4World._11 = xmf4x4View._11; m_xmf4x4World._12 = xmf4x4View._21; m_xmf4x4World._13 = xmf4x4View._31;
	m_xmf4x4World._21 = xmf4x4View._12; m_xmf4x4World._22 = xmf4x4View._22; m_xmf4x4World._23 = xmf4x4View._32;
	m_xmf4x4World._31 = xmf4x4View._13; m_xmf4x4World._32 = xmf4x4View._23; m_xmf4x4World._33 = xmf4x4View._33;
}

void CGameObject::UpdateBoundingBox()
{
	if (m_pMesh)
	{
		m_pMesh->m_xmOOBB.Transform(m_xmOOBB, XMLoadFloat4x4(&m_xmf4x4World));
		XMStoreFloat4(&m_xmOOBB.Orientation, XMQuaternionNormalize(XMLoadFloat4(&m_xmOOBB.Orientation)));
	}
}

void CGameObject::Animate(float fElapsedTime)
{
	if (m_fRotationSpeed != 0.0f) Rotate(m_xmf3RotationAxis, m_fRotationSpeed * fElapsedTime);
	if (m_fMovingSpeed != 0.0f) Move(m_xmf3MovingDirection, m_fMovingSpeed * fElapsedTime);

	UpdateBoundingBox();
}

void CGameObject::Render(HDC hDCFrameBuffer, XMFLOAT4X4* pxmf4x4World, CMesh* pMesh)
{
	if (pMesh)
	{
		CGraphicsPipeline::SetWorldTransform(pxmf4x4World);

		HPEN hPen = ::CreatePen(PS_SOLID, 0, m_dwColor);
		HPEN hOldPen = (HPEN)::SelectObject(hDCFrameBuffer, hPen);
		pMesh->Render(hDCFrameBuffer);
		::SelectObject(hDCFrameBuffer, hOldPen);
		::DeleteObject(hPen);
	}
}

void CGameObject::Render(HDC hDCFrameBuffer, CCamera* pCamera)
{
	if (pCamera->IsInFrustum(m_xmOOBB)) CGameObject::Render(hDCFrameBuffer, &m_xmf4x4World, m_pMesh);
}

void CGameObject::GenerateRayForPicking(XMVECTOR& xmvPickPosition, XMMATRIX& xmmtxView, XMVECTOR& xmvPickRayOrigin, XMVECTOR& xmvPickRayDirection)
{
	XMMATRIX xmmtxToModel = XMMatrixInverse(NULL, XMLoadFloat4x4(&m_xmf4x4World) * xmmtxView);

	XMFLOAT3 xmf3CameraOrigin(0.0f, 0.0f, 0.0f);
	xmvPickRayOrigin = XMVector3TransformCoord(XMLoadFloat3(&xmf3CameraOrigin), xmmtxToModel);
	xmvPickRayDirection = XMVector3TransformCoord(xmvPickPosition, xmmtxToModel);
	xmvPickRayDirection = XMVector3Normalize(xmvPickRayDirection - xmvPickRayOrigin);
}

int CGameObject::PickObjectByRayIntersection(XMVECTOR& xmvPickPosition, XMMATRIX& xmmtxView, float* pfHitDistance)
{
	int nIntersected = 0;
	if (m_pMesh)
	{
		XMVECTOR xmvPickRayOrigin, xmvPickRayDirection;
		GenerateRayForPicking(xmvPickPosition, xmmtxView, xmvPickRayOrigin, xmvPickRayDirection);
		nIntersected = m_pMesh->CheckRayIntersection(xmvPickRayOrigin, xmvPickRayDirection, pfHitDistance);
	}
	return(nIntersected);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
//
CWallsObject::CWallsObject()
{
}

CWallsObject::~CWallsObject()
{
}

void CWallsObject::Render(HDC hDCFrameBuffer, CCamera* pCamera)
{
	CGameObject::Render(hDCFrameBuffer, &m_xmf4x4World, m_pMesh);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
//
XMFLOAT3 CExplosiveObject::m_pxmf3SphereVectors[EXPLOSION_DEBRISES];
CMesh* CExplosiveObject::m_pExplosionMesh = NULL;

CExplosiveObject::CExplosiveObject()
{
}

CExplosiveObject::~CExplosiveObject()
{
}

void CExplosiveObject::PrepareExplosion()
{
	for (int i = 0; i < EXPLOSION_DEBRISES; i++) XMStoreFloat3(&m_pxmf3SphereVectors[i], ::RandomUnitVectorOnSphere());

	m_pExplosionMesh = new CCubeMesh(0.5f, 0.5f, 0.5f);
}

void CExplosiveObject::Animate(float fElapsedTime)
{
	if (m_bBlowingUp)
	{
		m_fElapsedTimes += fElapsedTime;
		if (m_fElapsedTimes <= m_fDuration)
		{
			XMFLOAT3 xmf3Position = GetPosition();
			for (int i = 0; i < EXPLOSION_DEBRISES; i++)
			{
				m_pxmf4x4Transforms[i] = Matrix4x4::Identity();
				m_pxmf4x4Transforms[i]._41 = xmf3Position.x + m_pxmf3SphereVectors[i].x * m_fExplosionSpeed * m_fElapsedTimes;
				m_pxmf4x4Transforms[i]._42 = xmf3Position.y + m_pxmf3SphereVectors[i].y * m_fExplosionSpeed * m_fElapsedTimes;
				m_pxmf4x4Transforms[i]._43 = xmf3Position.z + m_pxmf3SphereVectors[i].z * m_fExplosionSpeed * m_fElapsedTimes;
				m_pxmf4x4Transforms[i] = Matrix4x4::Multiply(Matrix4x4::RotationAxis(m_pxmf3SphereVectors[i], m_fExplosionRotation * m_fElapsedTimes), m_pxmf4x4Transforms[i]);
			}
		}
		else
		{
			m_bBlowingUp = false;
			m_fElapsedTimes = 0.0f;
		}
	}
	else
	{
		CGameObject::Animate(fElapsedTime);
	}
}

void CExplosiveObject::Render(HDC hDCFrameBuffer, CCamera* pCamera)
{
	if (m_bBlowingUp)
	{
		for (int i = 0; i < EXPLOSION_DEBRISES; i++)
		{
			CGameObject::Render(hDCFrameBuffer, &m_pxmf4x4Transforms[i], m_pExplosionMesh);
		}
	}
	else
	{
		CGameObject::Render(hDCFrameBuffer, pCamera);
	}
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
//
CBulletObject::CBulletObject(float fEffectiveRange)
{
	m_fBulletEffectiveRange = fEffectiveRange;
}

CBulletObject::~CBulletObject()
{
}

void CBulletObject::SetFirePosition(XMFLOAT3 xmf3FirePosition)
{
	m_xmf3FirePosition = xmf3FirePosition;
	SetPosition(xmf3FirePosition);
}

void CBulletObject::Reset()
{
	m_pLockedObject = NULL;
	m_fElapsedTimeAfterFire = 0;
	m_fMovingDistance = 0;
	m_fRotationAngle = 0.0f;

	m_bActive = false;
}

void CBulletObject::Animate(float fElapsedTime)
{
	m_fElapsedTimeAfterFire += fElapsedTime;

	float fDistance = m_fMovingSpeed * fElapsedTime;

	if ((m_fElapsedTimeAfterFire > m_fLockingDelayTime) && m_pLockedObject)
	{
		XMFLOAT3 xmf3Position = GetPosition();
		XMVECTOR xmvPosition = XMLoadFloat3(&xmf3Position);

		XMFLOAT3 xmf3LockedObjectPosition = m_pLockedObject->GetPosition();
		XMVECTOR xmvLockedObjectPosition = XMLoadFloat3(&xmf3LockedObjectPosition);
		XMVECTOR xmvToLockedObject = xmvLockedObjectPosition - xmvPosition;
		xmvToLockedObject = XMVector3Normalize(xmvToLockedObject);

		XMVECTOR xmvMovingDirection = XMLoadFloat3(&m_xmf3MovingDirection);
		xmvMovingDirection = XMVector3Normalize(XMVectorLerp(xmvMovingDirection, xmvToLockedObject, 0.25f));
		XMStoreFloat3(&m_xmf3MovingDirection, xmvMovingDirection);
	}

	XMFLOAT4X4 mtxRotate = Matrix4x4::RotationYawPitchRoll(0.0f, m_fRotationSpeed * fElapsedTime, 0.0f);
	m_xmf4x4World = Matrix4x4::Multiply(mtxRotate, m_xmf4x4World);
	XMFLOAT3 xmf3Movement = Vector3::ScalarProduct(m_xmf3MovingDirection, fDistance, false);
	XMFLOAT3 xmf3Position = GetPosition();
	xmf3Position = Vector3::Add(xmf3Position, xmf3Movement);
	SetPosition(xmf3Position);
	m_fMovingDistance += fDistance;

	UpdateBoundingBox();

	if ((m_fMovingDistance > m_fBulletEffectiveRange) || (m_fElapsedTimeAfterFire > m_fLockingTime)) Reset();
}

/////////////////////////////////////////////////////////////////////////////////////////////////////

CTextCharacterObject::CTextCharacterObject(wchar_t ch)
{
	BuildCharacterShape(ch);
}

CTextCharacterObject::~CTextCharacterObject()
{
	for (auto cube : m_Cubes) {
		ReleaseMesh();
		delete cube;
	}
	m_Cubes.clear();
}

void CTextCharacterObject::BuildCharacterShape(wchar_t ch)
{
	PrepareMesh();

	auto AddCube = [&](int x, int y)
		{
			CGameObject* pCube = new CGameObject();
			pCube->SetMesh(m_pSharedCubeMesh);
			pCube->SetPosition((float)x, (float)y, 0.0f);
			m_Cubes.push_back(pCube);
		};

	const wchar_t** shape = GetGlyph(ch);
	if (!shape) return;

	for (int y = 0; y < GLYPH_HEIGHT; ++y)
	{
		const wchar_t* line = shape[GLYPH_HEIGHT - 1 - y];
		for (int x = 0; x < GLYPH_WIDTH; ++x)
		{
			if (line[x] == L'#')
			{
				AddCube(x, y);
			}
		}
	}

	for (auto& cube : m_Cubes) {
		XMFLOAT3 pos = cube->GetPosition();
		pos.x -= 3.0f;
		pos.y -= 3.0f;
		cube->SetPosition(pos);
	}
}

CCubeMesh* CTextCharacterObject::m_pSharedCubeMesh = nullptr;

void CTextCharacterObject::PrepareMesh()
{
	if (!m_pSharedCubeMesh) {
		m_pSharedCubeMesh = new CCubeMesh(1.0f, 1.0f, 1.0f);
	}
	else {
		m_pSharedCubeMesh->AddRef();
	}
}

void CTextCharacterObject::ReleaseMesh()
{
	if (m_pSharedCubeMesh) {
		m_pSharedCubeMesh->Release();
		m_pSharedCubeMesh = nullptr;
	}
}

void CTextCharacterObject::Animate(float fElapsedTime)
{
	for (auto cube : m_Cubes)
		cube->Animate(fElapsedTime);
}

void CTextCharacterObject::Render(HDC hDCFrameBuffer, CCamera* pCamera, const XMFLOAT3& parentOffset)
{
	for (auto cube : m_Cubes)
	{
		XMFLOAT3 cubeLocal = cube->GetPosition();
		XMFLOAT3 rotatedLocal = Vector3::TransformCoord(cubeLocal, m_xmf4x4Rotation);
		XMFLOAT3 finalPos = Vector3::Add(parentOffset, rotatedLocal);

		XMFLOAT4X4 world = Matrix4x4::Identity();
		world._41 = finalPos.x;
		world._42 = finalPos.y;
		world._43 = finalPos.z;

		cube->Render(hDCFrameBuffer, &world, m_pSharedCubeMesh);
	}
}

void CTextCharacterObject::SetColor(COLORREF color)
{
	for (auto cube : m_Cubes)
		cube->SetColor(color);
}

int CTextCharacterObject::PickObjectByRayIntersection(XMVECTOR& xmvPickPosition, XMMATRIX& xmmtxView, float* pfHitDistance, const XMFLOAT3& parentOffset)
{
	int nIntersected = 0;

	for (auto& cube : m_Cubes)
	{
		XMFLOAT3 cubeLocal = cube->GetPosition();
		XMFLOAT3 rotatedLocal = Vector3::TransformCoord(cubeLocal, m_xmf4x4Rotation);
		XMFLOAT3 finalPos = Vector3::Add(parentOffset, rotatedLocal);

		XMFLOAT4X4 world = Matrix4x4::Identity();
		world._41 = finalPos.x;
		world._42 = finalPos.y;
		world._43 = finalPos.z;

		cube->SetPosition(finalPos);
		nIntersected = cube->PickObjectByRayIntersection(xmvPickPosition, xmmtxView, pfHitDistance);
		cube->SetPosition(cubeLocal);

		if (nIntersected > 0) break;
	}

	return nIntersected;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////

CTextObject::CTextObject(const std::wstring& text)
{
	SetText(text);
}

CTextObject::~CTextObject()
{
	for (auto& ch : m_Characters) {
		delete ch;
	}
	m_Characters.clear();
}

void CTextObject::SetText(const std::wstring& text)
{
	float xOffset = 0.0f;
	const float spacing = 8.0f;

	for (wchar_t ch : text)
	{
		CTextCharacterObject* pChar = new CTextCharacterObject(ch);
		pChar->SetPosition(XMFLOAT3(xOffset, 0.0f, 0.0f));  // 각 글자의 기준 위치
		xOffset += spacing;
		m_Characters.push_back(pChar);
	}
}

void CTextObject::SetColor(COLORREF color)
{
	m_dwColor = color;
	for (auto& ch : m_Characters)
		ch->SetColor(color);
}

void CTextObject::Animate(float fElapsedTime)
{
	if (!m_bActive) return;

	m_fRotationAngle += 45.0f * fElapsedTime;

	XMFLOAT3 center = { 0, 0, 0 };
	if (!m_Characters.empty())
	{
		size_t mid = m_Characters.size() / 2;
		center = m_Characters[mid]->GetPosition();
	}

	XMMATRIX mtxRotate = XMMatrixRotationAxis(XMLoadFloat3(&m_xmf3RotationAxis), XMConvertToRadians(m_fRotationAngle));
	XMMATRIX mtxTransToOrigin = XMMatrixTranslation(-center.x, -center.y, -center.z);
	XMMATRIX mtxTransBack = XMMatrixTranslation(center.x, center.y, center.z);
	XMMATRIX mtxWorld = mtxTransToOrigin * mtxRotate * mtxTransBack;

	XMFLOAT3 basePos = GetPosition();

	XMFLOAT4X4 xmf4Rotation;
	XMStoreFloat4x4(&xmf4Rotation, mtxRotate);

	for (auto ch : m_Characters)
	{
		XMFLOAT3 charLocal = ch->GetPosition();
		XMVECTOR vLocal = XMVectorSet(charLocal.x, charLocal.y, charLocal.z, 1.0f);
		XMVECTOR vWorld = XMVector3TransformCoord(vLocal, mtxWorld);

		XMFLOAT3 rotatedCharPos;
		XMStoreFloat3(&rotatedCharPos, XMVectorAdd(vWorld, XMLoadFloat3(&basePos)));

		ch->SetRotationMatrix(xmf4Rotation);
		ch->SetRotatedPos(rotatedCharPos);
	}

	for (auto& ch : m_Characters)
		ch->Animate(fElapsedTime);
}

void CTextObject::Render(HDC hDCFrameBuffer, CCamera* pCamera)
{
	if (!m_bActive) return;
	for (auto ch : m_Characters)
	{
		ch->Render(hDCFrameBuffer, pCamera, ch->GetRotatedPos());
	}
}

int CTextObject::PickObjectByRayIntersection(XMVECTOR& xmvPickPosition, XMMATRIX& xmmtxView, float* pfHitDistance)
{
	if (!m_bActive) return 0;

	int nIntersected = 0;

	for (auto ch : m_Characters)
	{
		XMFLOAT3 rotatedCharPos = ch->GetRotatedPos();

		nIntersected = ch->PickObjectByRayIntersection(xmvPickPosition, xmmtxView, pfHitDistance, rotatedCharPos);
		if (nIntersected > 0) return nIntersected;
	}

	return nIntersected;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////

CRailObject::CRailObject()
{
	SetMesh(new CRailMesh());
	SetColor(RGB(128, 128, 128));
}

CRailObject::~CRailObject()
{
}