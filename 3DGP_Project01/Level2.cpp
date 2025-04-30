#include "Level2.h"


CLevel2::CLevel2()
{
}

CLevel2::~CLevel2()
{
    ReleaseObjects();
}

void CLevel2::BuildObjects()
{
    CCamera* pCamera = new CCamera();
    pCamera->SetViewport(0, 0, FRAMEBUFFER_WIDTH, FRAMEBUFFER_HEIGHT);
    pCamera->GeneratePerspectiveProjectionMatrix(1.01f, 500.0f, 60.0f);
    pCamera->SetFOVAngle(60.0f);

    pCamera->GenerateOrthographicProjectionMatrix(1.01f, 50.0f, FRAMEBUFFER_WIDTH, FRAMEBUFFER_HEIGHT);

    // �÷��̾� ����
    m_pPlayer = new CTankPlayer(); // ���� CTankPlayer�� Ȯ�� ����
    m_pPlayer->SetPosition(0.0f, 0.0f, 0.0f);
    m_pPlayer->SetCamera(pCamera);
    m_pPlayer->SetCameraOffset(XMFLOAT3(0.0f, 20.0f, -75.0f));

    float fBottomHeight = 2.0f;
    float fUpperHeight = 2.0f;
    float fUpperWidth = 6.0f;

    m_pPlayer->m_fBottomHeight = fBottomHeight;
    m_pPlayer->m_fUpperHeight = fUpperHeight;
    m_pPlayer->m_fUpperWidth = fUpperWidth;

    CCubeMesh* pLowerBodyMesh = new CCubeMesh(8.0f, m_pPlayer->m_fBottomHeight, 12.0f);   // �ظ���: ��� ����
    CCubeMesh* pUpperBodyMesh = new CCubeMesh(6.0f, m_pPlayer->m_fUpperHeight, 6.0f);    // ������(��ž): ���簢��
    CCubeMesh* pTurretMesh = new CCubeMesh(1.0f, 1.0f, m_pPlayer->m_fUpperWidth);    // ����: ��� �� �Ѿ���

    // 2. CTankPlayer�� �޽� ����
    m_pPlayer->SetTankMesh(pLowerBodyMesh, pUpperBodyMesh, pTurretMesh);

    // 3. ���� ���� (����)
    m_pPlayer->SetColor(RGB(0, 100, 0)); // �⺻ ��ũ ��

    for (int i = 0; i < 10; ++i)
    {
        CTankEnemy* pEnemy = new CTankEnemy();
        pEnemy->m_fBottomHeight = fBottomHeight;
        pEnemy->m_fUpperHeight = fUpperHeight;
        pEnemy->m_fUpperWidth = fUpperWidth;
        pEnemy->SetPosition(i*100.0f,0.0f,0.0f);
        pEnemy->SetTankMesh(pLowerBodyMesh, pUpperBodyMesh, pTurretMesh);
        m_pEnemies.push_back(pEnemy);
    }

    CCubeMesh* ObstacleMesh = new CCubeMesh(16.0f, 16.0f, 16.0f);

    // ��ֹ� ����
    for (int i = 0; i < 5; ++i)
    {
        CGameObject* pObstacle = new CGameObject();
        float x = (rand() % 200) - 100;
        float z = (rand() % 200) - 100;
        pObstacle->SetPosition(x, 0.0f, z);
        pObstacle->SetMesh(ObstacleMesh);
        pObstacle->SetColor(RGB(255, 50, 10));
        m_pObstacles.push_back(pObstacle);
    }
}

void CLevel2::ReleaseObjects()
{
    if (m_pPlayer)
    {
        delete m_pPlayer;
        m_pPlayer = nullptr;
    }

    for (auto& enemy : m_pEnemies)
        delete enemy;
    m_pEnemies.clear();

    for (auto& obstacle : m_pObstacles)
        delete obstacle;
    m_pObstacles.clear();
}

void CLevel2::Animate(float fElapsedTime)
{
    if (m_pPlayer) m_pPlayer->Animate(fElapsedTime);
    for (auto& enemy : m_pEnemies)
        enemy->Animate(fElapsedTime);

    if (m_bAutoFire)
    {
        m_fAutoFireElapsed += fElapsedTime;
        if (m_fAutoFireElapsed >= m_fAutoFireInterval)
        {
            m_pPlayer->FireBullet(m_pLockedObject);
            m_fAutoFireElapsed = 0.0f;
        }
    }

    for (auto& obstacle : m_pObstacles)
        obstacle->Animate(fElapsedTime);


    // �浹 �˻�
    CheckPlayerEnemyCollision();
    CheckBulletEnemyCollision();
    CheckObstacleCollision();

    // �¸� ���� �˻�
    CheckWinCondition();
}

void CLevel2::Render(HDC hDCFrameBuffer)
{
    if (m_pPlayer) m_pPlayer->Render(hDCFrameBuffer, m_pPlayer->GetCamera());
    for (auto& enemy : m_pEnemies)
        enemy->Render(hDCFrameBuffer, m_pPlayer->GetCamera());
    for (auto& obstacle : m_pObstacles)
        obstacle->Render(hDCFrameBuffer, m_pPlayer->GetCamera());
    // �¸� �޽��� ���
    if (m_bShowWinMessage)
    {
        TextOut(hDCFrameBuffer, 400, 300, _T("You Win!"), 8);
    }
}

void CLevel2::UpdateCamera(float fElapsedTime)
{
    if (m_pPlayer && m_pPlayer->GetCamera())
        m_pPlayer->Update(fElapsedTime);
}

void CLevel2::OnProcessingKeyboardMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam)
{
    switch (nMessageID)
    {
    case WM_KEYDOWN:
        switch (wParam)
        {
        case 'A': case 'a':
            m_bAutoFire = !m_bAutoFire;
            break;
        case 'S': case 's':
            m_bShield = !m_bShield;
            break;
        case 'W': case 'w':
            m_bShowWinMessage = true;
            break;
        case VK_ESCAPE:
            m_nNextSceneID = 1;
            m_bSceneChange = true;
            break;
        }
        break;
    }
}

void CLevel2::OnProcessingMouseMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam)
{
    switch (nMessageID)
    {
    case WM_RBUTTONDOWN:
        m_pLockedObject = PickObjectPointedByCursor(LOWORD(lParam), HIWORD(lParam), m_pPlayer->GetCamera());
        break;
    default:
        break;
    }
}

void CLevel2::ProcessMouseInput(float dx, float dy, bool bRightButton)
{
    if (!bRightButton)
    {
        if (m_pPlayer)
        {
            m_pPlayer->RotateCameraOffset(dx * 0.5f); // �� dx�� ȸ��, ȸ�� �ӵ� ����
        }
    }
}

void CLevel2::ProcessInput(UCHAR* pKeyBuffer)
{
    if (!m_pPlayer) return;
    
    m_pPlayer->setPrePos();

    bool bMoved = false;

    if (pKeyBuffer[VK_UP] & 0xF0) {
        m_pPlayer->Move(DIR_FORWARD, 0.15f);
        bMoved = true;
    }
    if (pKeyBuffer[VK_DOWN] & 0xF0) {
        m_pPlayer->Move(DIR_BACKWARD, 0.15f);
        bMoved = true;
    }
    if (pKeyBuffer[VK_LEFT] & 0xF0) {
        m_pPlayer->Rotate(-2.0f);  // ���� (Yaw -)
        bMoved = true;
    }
    if (pKeyBuffer[VK_RIGHT] & 0xF0) {
        m_pPlayer->Rotate(2.0f);   // ������ (Yaw +)
        bMoved = true;
    }

    if (bMoved) m_pPlayer->Update(0.0f); // �̵� �Ǵ� ȸ���� �߻������� ������Ʈ
}

void CLevel2::CheckWinCondition()
{
    if (m_pEnemies.empty())
        m_bShowWinMessage = true;
}

CGameObject* CLevel2::PickObjectPointedByCursor(int xClient, int yClient, CCamera* pCamera)
{
    XMFLOAT3 xmf3PickPosition;
    xmf3PickPosition.x = (((2.0f * xClient) / (float)pCamera->m_Viewport.m_nWidth) - 1) / pCamera->m_xmf4x4PerspectiveProject._11;
    xmf3PickPosition.y = -(((2.0f * yClient) / (float)pCamera->m_Viewport.m_nHeight) - 1) / pCamera->m_xmf4x4PerspectiveProject._22;
    xmf3PickPosition.z = 1.0f;

    XMVECTOR xmvPickPosition = XMLoadFloat3(&xmf3PickPosition);
    XMMATRIX xmmtxView = XMLoadFloat4x4(&pCamera->m_xmf4x4View);

    int nIntersected = 0;
    float fNearestHitDistance = FLT_MAX;
    CGameObject* pNearestObject = NULL;

    for (auto& pEnemy : m_pEnemies)
    {
        float fHitDistance = FLT_MAX;
        nIntersected = pEnemy->PickObjectByRayIntersection(xmvPickPosition, xmmtxView, &fHitDistance);
        if ((nIntersected > 0) && (fHitDistance < fNearestHitDistance))
        {
            fNearestHitDistance = fHitDistance;
            pNearestObject = pEnemy;
        }
    }

    return pNearestObject;
}

void CLevel2::CheckPlayerEnemyCollision()
{
    if (!m_pPlayer) return;

    // �÷��̾��� OBB ����
    m_pPlayer->UpdateBoundingBox();
    BoundingOrientedBox playerOBB = m_pPlayer->m_xmOOBB;

    for (auto it = m_pEnemies.begin(); it != m_pEnemies.end(); )
    {
        CTankEnemy* pEnemy = (*it);

        // ���� OBB�� ����
        pEnemy->UpdateBoundingBox();
        BoundingOrientedBox enemyOBB = pEnemy->m_xmOOBB;

        // �浹 �˻�
        if (playerOBB.Intersects(enemyOBB))
        {
            if (m_bShield) // �ǵ� ����������
            {
                delete pEnemy;         // �� ����
                it = m_pEnemies.erase(it);
            }
            else // �ǵ� ����������
            {
                m_pPlayer->SetPosition(0.0f, 0.0f, 0.0f); // �÷��̾� �ʱ�ȭ
                break;
            }
        }
        else
        {
            ++it;
        }
    }
}

void CLevel2::CheckBulletEnemyCollision()
{
    CBulletObject** ppBullets = m_pPlayer->m_ppBullets;

    for (auto enemyIt = m_pEnemies.begin(); enemyIt != m_pEnemies.end(); )
    {
        CTankEnemy* pEnemy = *enemyIt;
        bool bHit = false;

        if (pEnemy->m_bActive) 
        {
            for (int i = 0; i < BULLETS; ++i)
            {
                if (ppBullets[i] && ppBullets[i]->m_bActive)
                {
                    // �Ѿ˰� ���� OBB �浹 �˻�
                    if (pEnemy->m_xmOOBB.Intersects(ppBullets[i]->m_xmOOBB))
                    {
                        // �� ���� ó��
                        //pEnemy->m_bBlowingUp = true;

                        pEnemy->SetActive(false);

                        // �Ѿ� ����
                        ppBullets[i]->Reset();

                        bHit = true;
                        m_pLockedObject = nullptr;
                        break; // �ϳ� ������ �ٷ� Ż��
                    }
                }
            }
        }

        ++enemyIt;
    }
}


void CLevel2::CheckObstacleCollision()
{
    // 1. �÷��̾� vs ��ֹ�
    if (m_pPlayer)
    {
        // ���� ��ġ ����
        XMFLOAT3 prevPos = m_pPlayer->getPrePos();

        // �̵� �õ�
        m_pPlayer->Animate(0.0f); // Ȥ�� �̵��� �ݿ��Ǿ��� �� �����Ƿ� ������Ʈ

        for (const auto& pObstacle : m_pObstacles)
        {
            if (m_pPlayer->m_xmOOBB.Intersects(pObstacle->m_xmOOBB))
            {
                // �浹 �߻� �� ���� ��ġ�� �ǵ�����
                m_pPlayer->SetPosition(prevPos.x, prevPos.y, prevPos.z);
                break;
            }
        }
    }

    // 2. ����ũ vs ��ֹ�
    for (auto& pEnemy : m_pEnemies)
    {
        if (!pEnemy || !pEnemy->m_bActive) continue;

        for (const auto& pObstacle : m_pObstacles)
        {
            if (pEnemy->m_xmOOBB.Intersects(pObstacle->m_xmOOBB))
            {
                pEnemy->ReverseDirection(); // �ݴ� �������� ȸ��
                break;
            }
        }
    }

    // 3. �Ѿ� vs ��ֹ�
    CBulletObject** ppBullets = m_pPlayer->m_ppBullets;

    for (int i = 0; i < BULLETS; ++i)
    {
        CBulletObject* pBullet = ppBullets[i];
        if (!pBullet || !pBullet->m_bActive) continue;

        for (const auto& pObstacle : m_pObstacles)
        {
            if (pBullet->m_xmOOBB.Intersects(pObstacle->m_xmOOBB))
            {
                pBullet->Reset();
                break;
            }
        }
    }
}

