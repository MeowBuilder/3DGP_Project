#include "Level2.h"


CLevel2::CLevel2()
{
    m_pPlayer = nullptr;
}

CLevel2::~CLevel2()
{
    ReleaseObjects();
}

void CLevel2::BuildObjects()
{
    CExplosiveObject::PrepareExplosion();
    
    CCamera* pCamera = new CCamera();
    pCamera->SetViewport(0, 0, FRAMEBUFFER_WIDTH, FRAMEBUFFER_HEIGHT);
    pCamera->GeneratePerspectiveProjectionMatrix(1.01f, 500.0f, 60.0f);
    pCamera->SetFOVAngle(60.0f);

    pCamera->GenerateOrthographicProjectionMatrix(1.01f, 50.0f, FRAMEBUFFER_WIDTH, FRAMEBUFFER_HEIGHT);

    m_pPlayer = new CTankPlayer();
    m_pPlayer->SetPosition(0.0f, 0.0f, 0.0f);
    m_pPlayer->SetCamera(pCamera);
    m_pPlayer->SetCameraOffset(XMFLOAT3(0.0f, 20.0f, -75.0f));

    float fBottomHeight = 2.0f;
    float fUpperHeight = 2.0f;
    float fUpperWidth = 6.0f;

    m_pPlayer->setSize(fBottomHeight, fUpperHeight, fUpperWidth);

    CCubeMesh* pLowerBodyMesh = new CCubeMesh(8.0f, fBottomHeight, 12.0f);
    CCubeMesh* pUpperBodyMesh = new CCubeMesh(6.0f, fUpperHeight, 6.0f);
    CCubeMesh* pTurretMesh = new CCubeMesh(1.0f, 1.0f, fUpperWidth);

    m_pPlayer->SetTankMesh(pLowerBodyMesh, pUpperBodyMesh, pTurretMesh);

    m_pPlayer->SetColor(RGB(200, 100, 0));

    for (int i = 0; i < 10; ++i)
    {
        CTankEnemy* pEnemy = new CTankEnemy();
        pEnemy->setSize(fBottomHeight, fUpperHeight, fUpperWidth);


        float ex = (rand() % 501) - 250;
        float ez = (rand() % 501) - 250;
        pEnemy->SetPosition(ex, 0.0f, ez);

        float yaw = ((rand() % 360) - 180);
        pEnemy->Rotate(yaw);

        pEnemy->SetTankMesh(pLowerBodyMesh, pUpperBodyMesh, pTurretMesh);
        m_pEnemies.push_back(pEnemy);
    }

    for (int i = 0; i < m_pEnemies.size(); ++i)
    {
        CExplosiveObject* pExplosion = new CExplosiveObject();
        pExplosion->SetActive(false);
        m_pExplosions.push_back(pExplosion);
    }

    CCubeMesh* ObstacleMesh = new CCubeMesh(16.0f, 16.0f, 16.0f);

    // 장애물 생성
    for (int i = 0; i < 5; ++i)
    {
        CGameObject* pObstacle = new CGameObject();
        float ox = (rand() % 501) - 250;
        float oz = (rand() % 501) - 250;
        pObstacle->SetPosition(ox, 8.0f, oz);

        pObstacle->SetMesh(ObstacleMesh);
        pObstacle->SetColor(RGB(255, 50, 10));
        m_pObstacles.push_back(pObstacle);
    }

    // 텍스트 생성
    m_pWinText = new CTextObject(L"YOU WIN!");
    m_pWinText->SetPosition(XMFLOAT3(0.0f, 50.0f, 0.0f));
    m_pWinText->SetColor(RGB(255, 0, 0));
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

    if (m_pWinText) {
        delete m_pWinText;
        m_pWinText = nullptr;
    }
}

void CLevel2::Animate(float fElapsedTime)
{
    if (m_pPlayer)
    {
        m_pPlayer->Animate(fElapsedTime);
        m_pPlayer->AutoFire(fElapsedTime, m_pLockedObject);
    }
    for (auto& enemy : m_pEnemies)
        enemy->Animate(fElapsedTime);

    for (auto& obstacle : m_pObstacles)
        obstacle->Animate(fElapsedTime);

    for (auto& explosion : m_pExplosions)
    {
        if (explosion->m_bActive)
            explosion->Animate(fElapsedTime);
    }

    if (m_pWinText && m_bShowWinMessage) {
        m_pWinText->Animate(fElapsedTime);
    }

    // 충돌 검사
    CheckPlayerEnemyCollision();
    CheckBulletEnemyCollision();
    CheckObstacleCollision();

    // 승리 조건 검사
    CheckWinCondition();
}

void CLevel2::Render(HDC hDCFrameBuffer)
{
    if (m_pPlayer) m_pPlayer->Render(hDCFrameBuffer, m_pPlayer->GetCamera());
    for (auto& enemy : m_pEnemies)
        enemy->Render(hDCFrameBuffer, m_pPlayer->GetCamera());
    for (auto& obstacle : m_pObstacles)
        obstacle->Render(hDCFrameBuffer, m_pPlayer->GetCamera());
    for (auto& explosion : m_pExplosions)
    {
        if (explosion->m_bActive)
            explosion->Render(hDCFrameBuffer, m_pPlayer->GetCamera());
    }

    if (m_pWinText && m_bShowWinMessage) {
        m_pWinText->Render(hDCFrameBuffer, m_pPlayer->GetCamera());
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
            m_pPlayer->ToggleAutoFire();
            break;
        case 'S': case 's':
            m_pPlayer->ToggleShield();
            break;
        case 'W': case 'w':
            m_bShowWinMessage = true;
            break;
        case VK_ESCAPE:
            m_nNextSceneID = 1;
            m_bSceneFinished = true;
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
            m_pPlayer->RotateCameraOffset(dx * 0.5f);
        }
    }
}

void CLevel2::ProcessInput(UCHAR* pKeyBuffer)
{
    if (!m_pPlayer) return;
    
    m_pPlayer->SetPrePos();

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
        m_pPlayer->Rotate(-2.0f);
        bMoved = true;
    }
    if (pKeyBuffer[VK_RIGHT] & 0xF0) {
        m_pPlayer->Rotate(2.0f);
        bMoved = true;
    }

    if (bMoved) m_pPlayer->Update(0.0f);
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

    m_pPlayer->UpdateBoundingBox();
    BoundingOrientedBox playerOBB = m_pPlayer->m_xmOOBB;

    for (auto it = m_pEnemies.begin(); it != m_pEnemies.end(); )
    {
        CTankEnemy* pEnemy = (*it);

        pEnemy->UpdateBoundingBox();
        BoundingOrientedBox enemyOBB = pEnemy->m_xmOOBB;

        if (playerOBB.Intersects(enemyOBB))
        {
            if (m_pPlayer->GetbShield())
            {
                delete pEnemy;
                it = m_pEnemies.erase(it);
            }
            else
            {
                m_pPlayer->SetPosition(0.0f, 0.0f, 0.0f);
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
                    if (pEnemy->m_xmOOBB.Intersects(ppBullets[i]->m_xmOOBB))
                    {
                        TriggerExplosion(pEnemy->GetPosition(), pEnemy->m_dwColor);

                        delete pEnemy;
                        enemyIt = m_pEnemies.erase(enemyIt);

                        ppBullets[i]->Reset();

                        bHit = true;
                        m_pLockedObject = nullptr;
                        break;
                    }
                }
            }
        }
        if (!bHit) ++enemyIt;
    }
}


void CLevel2::CheckObstacleCollision()
{
    // 플레이어 vs 장애물
    if (m_pPlayer)
    {
        XMFLOAT3 prevPos = m_pPlayer->GetPrePos();

        for (const auto& pObstacle : m_pObstacles)
        {
            if (m_pPlayer->m_xmOOBB.Intersects(pObstacle->m_xmOOBB))
            {
                m_pPlayer->SetPosition(prevPos.x, prevPos.y, prevPos.z);
                break;
            }
        }

        // 총알 vs 장애물
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

    // 적탱크 vs 장애물
    for (auto& pEnemy : m_pEnemies)
    {
        if (!pEnemy || !pEnemy->m_bActive) continue;

        for (const auto& pObstacle : m_pObstacles)
        {
            if (pEnemy->m_xmOOBB.Intersects(pObstacle->m_xmOOBB))
            {
                pEnemy->ReverseDirection();
                break;
            }
        }
    }
}

void CLevel2::TriggerExplosion(XMFLOAT3 position, COLORREF color)
{
    for (auto& explosion : m_pExplosions)
    {
        if (!explosion->m_bActive)
        {
            explosion->SetPosition(position);
            explosion->SetColor(color);
            explosion->SetActive(true);
            explosion->m_bBlowingUp = true;
            break;
        }
    }
}