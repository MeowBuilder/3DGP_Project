#include "stdafx.h"
#include "TankEnemy.h"

CTankEnemy::CTankEnemy()
{
    // 무작위 색상 생성
    int r = rand() % 156 + 100;
    int g = rand() % 156 + 100;
    int b = rand() % 156 + 100;
    COLORREF randomColor = RGB(r, g, b);

    SetColor(randomColor);
}

CTankEnemy::~CTankEnemy()
{
    // 메쉬 해제는 부모가 관리함
}

void CTankEnemy::Move(XMFLOAT3& xmf3Shift, bool bUpdateVelocity)
{
    // bUpdateVelocity가 true건 false건 무조건 포지션만 이동
    m_xmf3Position = Vector3::Add(xmf3Shift, m_xmf3Position);
}

void CTankEnemy::Animate(float fElapsedTime)
{
    if (!m_bActive) return;

    m_fElapsedTime += fElapsedTime;

    if (m_fElapsedTime >= m_fRandomMoveTime)
    {
        // 랜덤하게 방향 변경
        m_fRotationAngle = (rand() % 180) - 90.0f;

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
    XMFLOAT3 moveShift = Vector3::ScalarProduct(moveDir, m_iMoveDirection * 5.0f * fElapsedTime, false);
    Move(moveShift, false);

    CTankPlayer::Animate(fElapsedTime);
}

int CTankEnemy::PickObjectByRayIntersection(XMVECTOR& xmvPickPosition, XMMATRIX& xmmtxView, float* pfHitDistance)
{
    int nIntersected = 0;
    float fNearestHitDistance = FLT_MAX;

    XMVECTOR xmvPickRayOrigin, xmvPickRayDirection;
    GenerateRayForPicking(xmvPickPosition, xmmtxView, xmvPickRayOrigin, xmvPickRayDirection);

    if (m_pMeshLowerBody)
    {
        float fHitDistance = FLT_MAX;
        if (m_pMeshLowerBody->CheckRayIntersection(xmvPickRayOrigin, xmvPickRayDirection, &fHitDistance))
        {
            nIntersected = 1;
            fNearestHitDistance = fHitDistance;
        }
    }

    if (m_pMeshUpperBody)
    {
        float fHitDistance = FLT_MAX;
        if (m_pMeshUpperBody->CheckRayIntersection(xmvPickRayOrigin, xmvPickRayDirection, &fHitDistance))
        {
            if (!nIntersected || (fHitDistance < fNearestHitDistance))
            {
                nIntersected = 1;
                fNearestHitDistance = fHitDistance;
            }
        }
    }

    if (nIntersected && pfHitDistance) *pfHitDistance = fNearestHitDistance;

    return nIntersected;
}
