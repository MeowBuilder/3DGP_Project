#include "Level1.h"

XMFLOAT3 CatmullRom(const XMFLOAT3& p0, const XMFLOAT3& p1, const XMFLOAT3& p2, const XMFLOAT3& p3, float t)
{
    float t2 = t * t;
    float t3 = t2 * t;

    XMFLOAT3 result;
    result.x = 0.5f * ((2.0f * p1.x) +
        (-p0.x + p2.x) * t +
        (2.0f * p0.x - 5.0f * p1.x + 4.0f * p2.x - p3.x) * t2 +
        (-p0.x + 3.0f * p1.x - 3.0f * p2.x + p3.x) * t3);

    result.y = 0.5f * ((2.0f * p1.y) +
        (-p0.y + p2.y) * t +
        (2.0f * p0.y - 5.0f * p1.y + 4.0f * p2.y - p3.y) * t2 +
        (-p0.y + 3.0f * p1.y - 3.0f * p2.y + p3.y) * t3);

    result.z = 0.5f * ((2.0f * p1.z) +
        (-p0.z + p2.z) * t +
        (2.0f * p0.z - 5.0f * p1.z + 4.0f * p2.z - p3.z) * t2 +
        (-p0.z + 3.0f * p1.z - 3.0f * p2.z + p3.z) * t3);

    return result;
}

CLevel1::CLevel1()
{
}

CLevel1::~CLevel1()
{
    ReleaseObjects();
}

void CLevel1::BuildObjects()
{
    CCamera* pCamera = new CCamera();
    pCamera->SetViewport(0, 0, FRAMEBUFFER_WIDTH, FRAMEBUFFER_HEIGHT);
    pCamera->GeneratePerspectiveProjectionMatrix(1.01f, 500.0f, 60.0f);
    pCamera->SetFOVAngle(60.0f);

    pCamera->GenerateOrthographicProjectionMatrix(1.01f, 50.0f, FRAMEBUFFER_WIDTH, FRAMEBUFFER_HEIGHT);

    CCubeMesh* pPlayerMesh = new CCubeMesh(4.0f, 4.0f, 4.0f);

    m_pPlayer = new CPlayer();
    m_pPlayer->SetMesh(pPlayerMesh);
    m_pPlayer->SetColor(RGB(0, 0, 255));
    m_pPlayer->SetCamera(pCamera);
    m_pPlayer->SetCameraOffset(XMFLOAT3(0.0f, 30.0f, -30.0f));

    BuildRailSegments();
    BuildRailObjects();

    m_pPlayer->SetPosition(m_RailSegments[0].position.x, m_RailSegments[0].position.y, m_RailSegments[0].position.z);
}

void CLevel1::ReleaseObjects()
{
    for (auto pRail : m_pRailObjects)
    {
        delete pRail;
    }
    m_pRailObjects.clear();

    if (m_pPlayer)
    {
        delete m_pPlayer;
        m_pPlayer = nullptr;
    }
}   

void CLevel1::BuildRailSegments()
{
    m_ControlPoints.clear();
    m_RailSegments.clear();

    // 기준 점 설정
    m_ControlPoints.push_back(XMFLOAT3(100.0f, 0.0f, 0.0f));
    m_ControlPoints.push_back(XMFLOAT3(0.0f, 0.0f, 100.0f));
    m_ControlPoints.push_back(XMFLOAT3(-100.0f, 0.0f, 0.0f));
    m_ControlPoints.push_back(XMFLOAT3(0.0f, 0.0f, -100.0f));
    m_ControlPoints.push_back(XMFLOAT3(0.0f, 100.0f, -100.0f));
    m_ControlPoints.push_back(XMFLOAT3(0.0f, 200.0f, 0.0f));
    m_ControlPoints.push_back(XMFLOAT3(0.0f, 100.0f, 100.0f));
    m_ControlPoints.push_back(XMFLOAT3(100.0f, 0.0f, 0.0f));

    // 보간용 가상 점 추가 (양 끝 연결 자연스럽게)
    std::vector<XMFLOAT3> tempPoints = m_ControlPoints;
    tempPoints.insert(tempPoints.begin(), m_ControlPoints[m_ControlPoints.size() - 2]);
    tempPoints.push_back(m_ControlPoints[1]);

    const int interpolateSteps = 10;

    for (size_t i = 0; i < tempPoints.size() - 3; ++i)
    {
        for (int step = 0; step < interpolateSteps; ++step)
        {
            float t = (float)step / (float)interpolateSteps;
            XMFLOAT3 pos = CatmullRom(
                tempPoints[i],
                tempPoints[i + 1],
                tempPoints[i + 2],
                tempPoints[i + 3],
                t
            );

            XMFLOAT3 nextPos = CatmullRom(
                tempPoints[i],
                tempPoints[i + 1],
                tempPoints[i + 2],
                tempPoints[i + 3],
                t + (1.0f / interpolateSteps)
            );

            XMFLOAT3 tangent = Vector3::Normalize(Vector3::Subtract(nextPos, pos));
            m_RailSegments.push_back({ pos, tangent, 0.0f });
        }
    }
}



void CLevel1::BuildRailObjects()
{
    for (const auto& segment : m_RailSegments)
    {
        CRailObject* pRail = new CRailObject();
        pRail->SetPosition(segment.position.x, segment.position.y, segment.position.z);

        XMFLOAT3 up = { 0.0f, 1.0f, 0.0f };
        XMFLOAT3 tangentCopy = segment.tangent;
        pRail->LookTo(tangentCopy, up);

        m_pRailObjects.push_back(pRail);
    }
}

void CLevel1::Animate(float fElapsedTime)
{
    if (m_pPlayer) {
        UpdatePlayerOnRail(fElapsedTime);
    }

    for (auto& pRail : m_pRailObjects)
        pRail->Animate(fElapsedTime);
}

void CLevel1::UpdatePlayerOnRail(float fElapsedTime)
{
    float speed = 5.0f;
    m_fRailProgress += speed * fElapsedTime;

    if (m_fRailProgress >= 1.0f)
    {
        m_fRailProgress = 0.0f;
        m_nCurrentRailIndex++;

        if (m_nCurrentRailIndex >= m_RailSegments.size())
        {
            m_nNextSceneID = 3;
            m_bRailFinished = true;
            m_nCurrentRailIndex = 0;
            return;
        }
    }

    const RailSegment& seg0 = m_RailSegments[m_nCurrentRailIndex];
    const RailSegment& seg1 = m_RailSegments[(m_nCurrentRailIndex + 1) % m_RailSegments.size()];

    // ----------------------------
    // 1. 플레이어 이동 (보간)
    // ----------------------------
    XMFLOAT3 pos = Vector3::Lerp(seg0.position, seg1.position, m_fRailProgress);
    m_pPlayer->SetPosition(pos.x, pos.y, pos.z);

    // ----------------------------
    // 2. 플레이어 방향 보간
    // ----------------------------
    XMFLOAT3 railDirection = seg0.tangent;
    railDirection = Vector3::Normalize(railDirection);

    XMFLOAT3 currentLook = m_pPlayer->GetLook();
    float fRotateLerpSpeed = 5.0f;
    XMFLOAT3 newLook = Vector3::Lerp(currentLook, railDirection, fRotateLerpSpeed * fElapsedTime);
    newLook = Vector3::Normalize(newLook);

    XMFLOAT3 worldUp = XMFLOAT3(0.0f, 1.0f, 0.0f);
    XMFLOAT3 newRight = Vector3::Normalize(Vector3::CrossProduct(worldUp, newLook));
    XMFLOAT3 newUp = Vector3::Normalize(Vector3::CrossProduct(newLook, newRight));

    m_pPlayer->SetOrientation(newRight, newUp, newLook);

    // ----------------------------
    // 3. 카메라 위치를 '플레이어 뒤쪽'으로 재설정
    // ----------------------------
    XMFLOAT3 playerPos = m_pPlayer->GetPosition();
    XMFLOAT3 playerLook = m_pPlayer->GetLook();
    XMFLOAT3 playerUp = m_pPlayer->GetUp();
    XMFLOAT3 playerRight = m_pPlayer->GetRight();

    XMFLOAT3 offset = Vector3::Add(
        Vector3::ScalarProduct(playerUp, 30.0f, false),
        Vector3::ScalarProduct(playerLook, -30.0f, false)
    );

    XMFLOAT3 camPos = Vector3::Add(playerPos, offset);

    m_pPlayer->GetCamera()->SetLookAt(camPos, playerPos, playerUp);
    m_pPlayer->GetCamera()->GenerateViewMatrix();

    // ----------------------------
    m_pPlayer->Animate(fElapsedTime);
}



void CLevel1::Render(HDC hDCFrameBuffer)
{
    CCamera* pCamera = m_pPlayer->GetCamera();

    CGraphicsPipeline::SetViewport(&pCamera->m_Viewport);
    CGraphicsPipeline::SetViewPerspectiveProjectTransform(&pCamera->m_xmf4x4ViewPerspectiveProject);

    // 레일 렌더링
    for (auto& pRail : m_pRailObjects)
        pRail->Render(hDCFrameBuffer, pCamera);

    // [추가] 플레이어 렌더링
    if (m_pPlayer)
        m_pPlayer->Render(hDCFrameBuffer, pCamera);
}

void CLevel1::UpdateCamera(float fElapsedTime)
{
    if (m_pPlayer && m_pPlayer->GetCamera())
        m_pPlayer->Update(fElapsedTime);
}

void CLevel1::OnProcessingMouseMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam)
{

}

void CLevel1::OnProcessingKeyboardMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam)
{
    switch (nMessageID)
    {
    case WM_KEYDOWN:
        switch (wParam)
        {
        case 'n':
        case 'N':
            m_nNextSceneID = 3;
            m_bRailFinished = true;
            break;
        case VK_ESCAPE:
            m_nNextSceneID = 1;
            m_bRailFinished = true;
            break;
        default:
            break;
        }
        break;
    default:
        break;
    }
}


