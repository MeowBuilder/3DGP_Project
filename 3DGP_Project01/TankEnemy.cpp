#include "stdafx.h"
#include "TankEnemy.h"

CTankEnemy::CTankEnemy()
{
    m_fRandomMoveTime = 1.0f + ((float)(rand() % 200) / 100.0f); // 1~3�� ���� ����
}

CTankEnemy::~CTankEnemy()
{
    // �޽� ������ �θ� ������
}

void CTankEnemy::Animate(float fElapsedTime)
{
    m_fElapsedTime += fElapsedTime;

    if (m_fElapsedTime >= m_fRandomMoveTime)
    {
        // �����ϰ� ���� ����
        int nMoveType = rand() % 2;
        m_nMoveDirection = (nMoveType == 0) ? 1 : -1;

        m_fRotationAngle = (rand() % 61) - 30.0f; // -30�� ~ +30�� ȸ��

        m_fElapsedTime = 0.0f;
        m_fRandomMoveTime = 1.0f + ((float)(rand() % 200) / 100.0f);
    }

    // ȸ��
    if (fabs(m_fRotationAngle) > 0.0f)
    {
        float rotateSpeed = 90.0f; // �ʴ� 90��
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

    // �̵� (���� �Ǵ� ����)
    XMFLOAT3 moveDir = GetLook();
    XMFLOAT3 moveShift = Vector3::ScalarProduct(moveDir, m_nMoveDirection * 5.0f * fElapsedTime, false);
    Move(moveShift, false);

    CTankPlayer::Animate(fElapsedTime);
}
