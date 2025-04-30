#include "Scene.h"

CScene::CScene()
{
	m_pPlayer = NULL;
}

CScene::~CScene()
{
	ReleaseObjects();
}

void CScene::BuildObjects()
{
	CCamera* pCamera = new CCamera();
	pCamera->SetViewport(0, 0, FRAMEBUFFER_WIDTH, FRAMEBUFFER_HEIGHT);
	pCamera->GeneratePerspectiveProjectionMatrix(1.01f, 500.0f, 60.0f);
	pCamera->SetFOVAngle(60.0f);

	pCamera->GenerateOrthographicProjectionMatrix(1.01f, 50.0f, FRAMEBUFFER_WIDTH, FRAMEBUFFER_HEIGHT);

	CAirplaneMesh* pAirplaneMesh = new CAirplaneMesh(6.0f, 6.0f, 1.0f);

	m_pPlayer = new CAirplanePlayer();
	m_pPlayer->SetPosition(0.0f, 0.0f, 0.0f);
	m_pPlayer->SetMesh(pAirplaneMesh);
	m_pPlayer->SetColor(RGB(0, 0, 255));
	m_pPlayer->SetCamera(pCamera);
	m_pPlayer->SetCameraOffset(XMFLOAT3(0.0f, 5.0f, -15.0f));
}

void CScene::ReleaseObjects()
{
	if (m_pPlayer)
	{
		delete m_pPlayer;
		m_pPlayer = nullptr;
	}
}

void CScene::OnProcessingMouseMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam)
{
	switch (nMessageID)
	{
	default:
		break;
	}

}

void CScene::OnProcessingKeyboardMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam)
{
	switch (nMessageID)
	{
	case WM_KEYDOWN:
		switch (wParam)
		{
		case VK_ESCAPE:
			m_nNextSceneID = 1;
			m_bSceneFinished = true;
			break;
		default:
			break;
		}
		break;
	default:
		break;
	}
}

void CScene::ProcessInput(UCHAR* pKeyBuffer)
{

}

void CScene::ProcessMouseInput(float dx, float dy, bool bRightButton)
{

}

void CScene::UpdateCamera(float fElapsedTime)
{
	if (m_pPlayer && m_pPlayer->GetCamera())
	{
		m_pPlayer->Update(fElapsedTime);
	}
}

void CScene::Animate(float fElapsedTime)
{
	if (m_pPlayer) m_pPlayer->Animate(fElapsedTime);
}

void CScene::Render(HDC hDCFrameBuffer)
{
	CCamera* pCamera = m_pPlayer->GetCamera();

	CGraphicsPipeline::SetViewport(&pCamera->m_Viewport);

	CGraphicsPipeline::SetViewPerspectiveProjectTransform(&pCamera->m_xmf4x4ViewPerspectiveProject);

	if (m_pPlayer) m_pPlayer->Render(hDCFrameBuffer, pCamera);
}