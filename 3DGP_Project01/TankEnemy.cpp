#include "stdafx.h"
#include "TankEnemy.h"

CTankEnemy::CTankEnemy()
{
    // 메쉬 생성
    m_pLowerBody = new CGameObject();
    m_pLowerBody->SetMesh(new CCubeMesh(8.0f, 2.0f, 8.0f));
    m_pLowerBody->SetColor(RGB(100, 100, 100));

    m_pUpperBody = new CGameObject();
    m_pUpperBody->SetMesh(new CCubeMesh(6.0f, 2.0f, 6.0f));
    m_pUpperBody->SetColor(RGB(150, 150, 150));
    m_pUpperBody->SetPosition(0.0f, 2.0f, 0.0f);

    m_pCannon = new CGameObject();
    m_pCannon->SetMesh(new CCubeMesh(1.0f, 1.0f, 6.0f));
    m_pCannon->SetColor(RGB(200, 0, 0));
    m_pCannon->SetPosition(0.0f, 1.0f, 5.0f);

    m_fRandomMoveTime = 1.0f + ((float)(rand() % 200) / 100.0f);
}

CTankEnemy::~CTankEnemy()
{
    delete m_pLowerBody;
    delete m_pUpperBody;
    delete m_pCannon;
}

void CTankEnemy::Animate(float fElapsedTime)
{
    m_fElapsedTime += fElapsedTime;

    if (m_fElapsedTime >= m_fRandomMoveTime)
    {
        // 랜덤 회전
        float randomAngle = ((rand() % 360) - 180.0f);
        XMMATRIX mtxRotate = XMMatrixRotationY(XMConvertToRadians(randomAngle));
        m_xmf3Direction = Vector3::TransformNormal(m_xmf3Direction, mtxRotate);
        m_xmf3Direction = Vector3::Normalize(m_xmf3Direction);

        m_fElapsedTime = 0.0f;
        m_fRandomMoveTime = 1.0f + ((float)(rand() % 200) / 100.0f);
    }

    // 이동
    XMFLOAT3 pos = GetPosition();
    XMFLOAT3 move = Vector3::ScalarProduct(m_xmf3Direction, m_fMoveSpeed * fElapsedTime, false);
    XMFLOAT3 newPos = Vector3::Add(pos, move);
    SetPosition(newPos);

    // 하위 오브젝트 Animate
    m_pLowerBody->Animate(fElapsedTime);
    m_pUpperBody->Animate(fElapsedTime);
    m_pCannon->Animate(fElapsedTime);
}

void CTankEnemy::Render(HDC hDCFrameBuffer, CCamera* pCamera)
{
    XMFLOAT3 basePos = GetPosition();

    {
        XMFLOAT4X4 world = Matrix4x4::Identity();
        world._41 = basePos.x;
        world._42 = basePos.y;
        world._43 = basePos.z;
        m_pLowerBody->Render(hDCFrameBuffer, &world, m_pLowerBody->GetMesh());
    }

    {
        XMFLOAT4X4 world = Matrix4x4::Identity();
        world._41 = basePos.x + m_pUpperBody->GetPosition().x;
        world._42 = basePos.y + m_pUpperBody->GetPosition().y;
        world._43 = basePos.z + m_pUpperBody->GetPosition().z;
        m_pUpperBody->Render(hDCFrameBuffer, &world, m_pUpperBody->GetMesh());
    }

    {
        XMFLOAT4X4 world = Matrix4x4::Identity();
        world._41 = basePos.x + m_pCannon->GetPosition().x;
        world._42 = basePos.y + m_pCannon->GetPosition().y;
        world._43 = basePos.z + m_pCannon->GetPosition().z;
        m_pCannon->Render(hDCFrameBuffer, &world, m_pCannon->GetMesh());
    }
}