#include "stdafx.h"
#include "TankEnemy.h"

CTankEnemy::CTankEnemy()
{
    m_fRandomMoveTime = 1.0f + ((float)(rand() % 200) / 100.0f); // 1~3초 사이 랜덤
}

CTankEnemy::~CTankEnemy()
{
    // 메쉬 해제는 부모가 관리함
}

void CTankEnemy::Animate(float fElapsedTime)
{
    m_fElapsedTime += fElapsedTime;

    if (m_fElapsedTime >= m_fRandomMoveTime)
    {
        // 랜덤하게 방향 변경
        int nMoveType = rand() % 2;
        m_nMoveDirection = (nMoveType == 0) ? 1 : -1;

        m_fRotationAngle = (rand() % 61) - 30.0f; // -30도 ~ +30도 회전

        m_fElapsedTime = 0.0f;
        m_fRandomMoveTime = 1.0f + ((float)(rand() % 200) / 100.0f);
    }

    // 회전
    if (fabs(m_fRotationAngle) > 0.0f)
    {
        float rotateSpeed = 90.0f; // 초당 90도
        float rotateAmount = rotateSpeed * fElapsedTime;

        if (m_fRotationAngle > 0.0f)
        {
            Rotate(rotateAmount); // CTankPlayer::Rotate(float fYaw)
            m_fRotationAngle -= rotateAmount;
            if (m_fRotationAngle < 0.0f) m_fRotationAngle = 0.0f;
        }
        else
        {
            Rotate(-rotateAmount);
            m_fRotationAngle += rotateAmount;
            if (m_fRotationAngle > 0.0f) m_fRotationAngle = 0.0f;
        }
    }

    // 이동 (전진 또는 후진)
    XMFLOAT3 moveDir = GetLook();
    XMFLOAT3 moveShift = Vector3::ScalarProduct(moveDir, m_nMoveDirection * 5.0f * fElapsedTime, false);
    Move(moveShift, false);

    CTankPlayer::Animate(fElapsedTime);
}
