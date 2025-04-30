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

    // 플레이어 생성
    m_pPlayer = new CTankPlayer(); // 추후 CTankPlayer로 확장 가능
    m_pPlayer->SetPosition(0.0f, 0.0f, 0.0f);
    m_pPlayer->SetCamera(pCamera);
    m_pPlayer->SetCameraOffset(XMFLOAT3(0.0f, 20.0f, -75.0f));

    float fBottomHeight = 2.0f;
    float fUpperHeight = 2.0f;
    float fUpperWidth = 6.0f;

    m_pPlayer->m_fBottomHeight = fBottomHeight;
    m_pPlayer->m_fUpperHeight = fUpperHeight;
    m_pPlayer->m_fUpperWidth = fUpperWidth;

    CCubeMesh* pLowerBodyMesh = new CCubeMesh(8.0f, m_pPlayer->m_fBottomHeight, 12.0f);   // 밑몸통: 길고 낮게
    CCubeMesh* pUpperBodyMesh = new CCubeMesh(6.0f, m_pPlayer->m_fUpperHeight, 6.0f);    // 위몸통(포탑): 정사각형
    CCubeMesh* pTurretMesh = new CCubeMesh(1.0f, 1.0f, m_pPlayer->m_fUpperWidth);    // 포신: 얇고 긴 총알통

    // 2. CTankPlayer에 메쉬 연결
    m_pPlayer->SetTankMesh(pLowerBodyMesh, pUpperBodyMesh, pTurretMesh);

    // 3. 색깔 설정 (선택)
    m_pPlayer->SetColor(RGB(0, 100, 0)); // 기본 탱크 색

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

    // 장애물 생성
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
    // 승리 메시지 출력
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
            m_pPlayer->RotateCameraOffset(dx * 0.5f); // ← dx로 회전, 회전 속도 조정
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
        m_pPlayer->Rotate(-2.0f);  // 왼쪽 (Yaw -)
        bMoved = true;
    }
    if (pKeyBuffer[VK_RIGHT] & 0xF0) {
        m_pPlayer->Rotate(2.0f);   // 오른쪽 (Yaw +)
        bMoved = true;
    }

    if (bMoved) m_pPlayer->Update(0.0f); // 이동 또는 회전이 발생했으면 업데이트
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

    // 플레이어의 OBB 갱신
    m_pPlayer->UpdateBoundingBox();
    BoundingOrientedBox playerOBB = m_pPlayer->m_xmOOBB;

    for (auto it = m_pEnemies.begin(); it != m_pEnemies.end(); )
    {
        CTankEnemy* pEnemy = (*it);

        // 적의 OBB도 갱신
        pEnemy->UpdateBoundingBox();
        BoundingOrientedBox enemyOBB = pEnemy->m_xmOOBB;

        // 충돌 검사
        if (playerOBB.Intersects(enemyOBB))
        {
            if (m_bShield) // 실드 켜져있으면
            {
                delete pEnemy;         // 적 삭제
                it = m_pEnemies.erase(it);
            }
            else // 실드 꺼져있으면
            {
                m_pPlayer->SetPosition(0.0f, 0.0f, 0.0f); // 플레이어 초기화
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
                    // 총알과 적의 OBB 충돌 검사
                    if (pEnemy->m_xmOOBB.Intersects(ppBullets[i]->m_xmOOBB))
                    {
                        // 적 폭발 처리
                        //pEnemy->m_bBlowingUp = true;

                        pEnemy->SetActive(false);

                        // 총알 리셋
                        ppBullets[i]->Reset();

                        bHit = true;
                        m_pLockedObject = nullptr;
                        break; // 하나 맞으면 바로 탈출
                    }
                }
            }
        }

        ++enemyIt;
    }
}


void CLevel2::CheckObstacleCollision()
{
    // 1. 플레이어 vs 장애물
    if (m_pPlayer)
    {
        // 현재 위치 저장
        XMFLOAT3 prevPos = m_pPlayer->getPrePos();

        // 이동 시도
        m_pPlayer->Animate(0.0f); // 혹시 이동이 반영되었을 수 있으므로 업데이트

        for (const auto& pObstacle : m_pObstacles)
        {
            if (m_pPlayer->m_xmOOBB.Intersects(pObstacle->m_xmOOBB))
            {
                // 충돌 발생 시 이전 위치로 되돌리기
                m_pPlayer->SetPosition(prevPos.x, prevPos.y, prevPos.z);
                break;
            }
        }
    }

    // 2. 적탱크 vs 장애물
    for (auto& pEnemy : m_pEnemies)
    {
        if (!pEnemy || !pEnemy->m_bActive) continue;

        for (const auto& pObstacle : m_pObstacles)
        {
            if (pEnemy->m_xmOOBB.Intersects(pObstacle->m_xmOOBB))
            {
                pEnemy->ReverseDirection(); // 반대 방향으로 회전
                break;
            }
        }
    }

    // 3. 총알 vs 장애물
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

