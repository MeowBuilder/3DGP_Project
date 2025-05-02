#pragma once

#include "GraphicsPipeline.h"
#include "GameObject.h"
#include "Camera.h"

class CPlayer : public CGameObject
{
public:
	CPlayer();
	virtual ~CPlayer();

public:
	XMFLOAT3					m_xmf3Position = XMFLOAT3(0.0f, 0.0f, 0.0f);
	XMFLOAT3					m_xmf3Right = XMFLOAT3(1.0f, 0.0f, 0.0f);
	XMFLOAT3					m_xmf3Up = XMFLOAT3(0.0f, 1.0f, 0.0f);
	XMFLOAT3					m_xmf3Look = XMFLOAT3(0.0f, 0.0f, 1.0f);

	XMFLOAT3					m_xmf3CameraOffset = XMFLOAT3(0.0f, 0.0f, 0.0f);
	XMFLOAT3					m_xmf3Velocity = XMFLOAT3(0.0f, 0.0f, 0.0f);

	float						m_fFriction = 125.0f;

	float           			m_fPitch = 0.0f;
	float           			m_fYaw = 0.0f;
	float           			m_fRoll = 0.0f;

	CCamera* m_pCamera = NULL;

public:
	void SetPosition(float x, float y, float z);

	void LookAt(XMFLOAT3& xmf3LookAt, XMFLOAT3& xmf3Up);

	void Move(DWORD dwDirection, float fDistance);
	void Move(XMFLOAT3& xmf3Shift, bool bUpdateVelocity);
	void Move(float x, float y, float z);

	void Rotate(float fPitch = 0.0f, float fYaw = 0.0f, float fRoll = 0.0f);
	void Rotate(XMFLOAT3& xmf3RotationAxis, float fAngle);

	void SetOrientation(const XMFLOAT3& right, const XMFLOAT3& up, const XMFLOAT3& look);

	void SetCameraOffset(XMFLOAT3& xmf3CameraOffset);
	XMFLOAT3 GetCameraOffset() { return m_xmf3CameraOffset; };

	void Update(float fTimeElapsed = 0.016f);

	virtual void OnUpdateTransform();
	virtual void Animate(float fElapsedTime);
	virtual void Render(HDC hDCFrameBuffer, CCamera* pCamera);

	void SetCamera(CCamera* pCamera) { m_pCamera = pCamera; }
	CCamera* GetCamera() { return(m_pCamera); }
};

#define BULLETS					50

class CAirplanePlayer : public CPlayer
{
public:
	CAirplanePlayer();
	virtual ~CAirplanePlayer();

	float						m_fBulletEffectiveRange = 150.0f;
	CBulletObject* m_ppBullets[BULLETS];

	void FireBullet(CGameObject* pLockedObject);

	virtual void OnUpdateTransform();
	virtual void Animate(float fElapsedTime);
	virtual void Render(HDC hDCFrameBuffer, CCamera* pCamera);
};

class CTankPlayer : public CPlayer
{
private:
	bool m_bAutoFire = false;
	float m_fAutoFireElapsed = 0.0f;
	float m_fAutoFireInterval = 1.0f;
	bool m_bShield = false;
public:
    CTankPlayer();
    virtual ~CTankPlayer();

	void ToggleAutoFire() { m_bAutoFire = !m_bAutoFire; }
	void AutoFire(float fElapsedTime, CGameObject* pLockedObject);

	void ToggleShield();
	bool GetbShield() { return m_bShield; }

	void SetCameraOffset(XMFLOAT3& xmf3CameraOffset);

	void SetPrePos() { m_xmf3PrePos = m_xmf3Position; };
	XMFLOAT3 GetPrePos() { return m_xmf3PrePos; };

    virtual void Animate(float fElapsedTime);
    virtual void Render(HDC hDCFrameBuffer, CCamera* pCamera);
	void FireBullet(CGameObject*);

	void setSize(float BottomHeight, float UpperHeight, float UpperWidth) {
		m_fBottomHeight = BottomHeight;
		m_fUpperHeight = UpperHeight;
		m_fUpperWidth = UpperWidth;
	}

	void Rotate(float fYaw);

	void SetTankMesh(CMesh*, CMesh*, CMesh*);
	void RotateCameraOffset(float fAngleDegree);

	void UpdateTopParts();
	void UpdateBoundingBox();

	float m_fBulletEffectiveRange = 1500.0f;
	CBulletObject* m_ppBullets[BULLETS];

protected:
    CMesh* m_pMeshLowerBody = nullptr;
    CMesh* m_pMeshUpperBody = nullptr;
    CMesh* m_pMeshTurret = nullptr;

	float m_fBottomHeight = 0.0f;
	float m_fUpperHeight = 0.0f;
	float m_fUpperWidth = 0.0f;

	XMFLOAT3 m_xmf3TopRight = { 1.0f, 0.0f, 0.0f };
	XMFLOAT3 m_xmf3TopUp = { 0.0f, 1.0f, 0.0f };
	XMFLOAT3 m_xmf3TopLook = { 0.0f, 0.0f, 1.0f };

	XMFLOAT3 m_xmf3PrePos = { 0.0f, 0.0f, 0.0f };

	XMFLOAT3 m_xmf3BaseCameraOffset;
};
