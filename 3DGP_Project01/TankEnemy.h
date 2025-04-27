#pragma once

#include "GameObject.h"

class CTankEnemy : public CGameObject
{
public:
    CTankEnemy();
    virtual ~CTankEnemy();

    virtual void Animate(float fElapsedTime) override;
    virtual void Render(HDC hDCFrameBuffer, CCamera* pCamera) override;

private:
    CGameObject* m_pLowerBody = nullptr;
    CGameObject* m_pUpperBody = nullptr;
    CGameObject* m_pCannon = nullptr;

    float m_fRandomMoveTime = 0.0f;
    float m_fElapsedTime = 0.0f;
    float m_fMoveSpeed = 5.0f;
    float m_fRotationSpeed = 45.0f; // degrees per second

    XMFLOAT3 m_xmf3Direction = { 0.0f, 0.0f, 1.0f }; // 초기 방향 (앞쪽)
};