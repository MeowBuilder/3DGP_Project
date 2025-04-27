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

    m_pPlayer->m_fBottomHeight = 2.0f;
    m_pPlayer->m_fUpperHeight = 2.0f;
    m_pPlayer->m_fUpperWidth = 6.0f;

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
        pEnemy->SetPosition(i*100.0f,0.0f,0.0f);
        m_pEnemies.push_back(pEnemy);
    }

    // 장애물 생성
    for (int i = 0; i < 5; ++i)
    {
        CGameObject* pObstacle = new CGameObject();
        float x = (rand() % 200) - 100;
        float z = (rand() % 200) - 100;
        pObstacle->SetPosition(x, 0.0f, z);
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

    // 충돌 검사
    CheckBulletCollisions();

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
            m_bAutoAttack = !m_bAutoAttack;
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
    // 플레이어 회전 및 타겟팅 처리 추가 예정
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



void CLevel2::FirePlayerBullet()
{
    // 플레이어 총알 발사 로직 (추후 작성)
}

void CLevel2::CheckBulletCollisions()
{
    // 플레이어 총알 vs 적 탱크 충돌 검사 (추후 작성)
}

void CLevel2::CheckWinCondition()
{
    if (m_pEnemies.empty())
        m_bShowWinMessage = true;
}