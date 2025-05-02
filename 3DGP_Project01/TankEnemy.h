#pragma once

#include "Player.h"

class CTankEnemy : public CTankPlayer
{
public:
    CTankEnemy();
    virtual ~CTankEnemy();

    virtual void Animate(float fElapsedTime) override;
    void Move(XMFLOAT3& xmf3Shift, bool bUpdateVelocity);
    void ReverseDirection() { m_iMoveDirection = -m_iMoveDirection; };

    int PickObjectByRayIntersection(XMVECTOR& xmvPickPosition, XMMATRIX& xmmtxView, float* pfHitDistance);

private:
    float m_fElapsedTime = 0.0f;
    float m_fRandomMoveTime = 1.0f;
    float m_fRotationAngle = 0.0f;
    int m_iMoveDirection = 1;
};
