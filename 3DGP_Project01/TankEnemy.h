#pragma once

#include "Player.h"

class CTankEnemy : public CTankPlayer
{
public:
    CTankEnemy();
    virtual ~CTankEnemy();

    virtual void Animate(float fElapsedTime) override;

private:
    float m_fElapsedTime = 0.0f;
    float m_fRandomMoveTime = 1.0f;
    int m_nMoveDirection = 1; // 1: 전진, -1: 후진
    float m_fRotationAngle = 0.0f; // 회전해야 할 각도 (도 단위)
};
