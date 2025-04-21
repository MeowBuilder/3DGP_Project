#include "StartScene.h"

CStartScene::CStartScene(CPlayer* pPlayer)
	: CScene(nullptr)
{

}

CStartScene::~CStartScene()
{
}

void CStartScene::BuildObjects()
{	
	m_pSceneCamera = new CCamera();
	m_pSceneCamera->SetViewport(0, 0, FRAMEBUFFER_WIDTH, FRAMEBUFFER_HEIGHT);
	m_pSceneCamera->GeneratePerspectiveProjectionMatrix(1.0f, 5000.0f, 60.0f);

	// 여러 텍스트 오브젝트 생성
	CTextObject3D* pText1 = new CTextObject3D(L"3D 게임 프로그래밍 1");
	pText1->SetPosition(XMFLOAT3(-50.0f, 25.0f, 75.0f));
	m_pTextObjects.push_back(pText1);

	CTextObject3D* pText2 = new CTextObject3D(L"김태순");
	pText2->SetPosition(XMFLOAT3(-10.0f, -15.0f, 50.0f));
	pText2->SetColor(RGB(0, 0, 255));
	m_pTextObjects.push_back(pText2);

#ifdef _WITH_DRAW_AXIS
	m_pWorldAxis = new CGameObject();
	CAxisMesh* pAxisMesh = new CAxisMesh(0.5f, 0.5f, 0.5f);
	m_pWorldAxis->SetMesh(pAxisMesh);
#endif
}

void CStartScene::ReleaseObjects()
{
	for (auto pText : m_pTextObjects)
		delete pText;
	m_pTextObjects.clear();

#ifdef _WITH_DRAW_AXIS
	if (m_pWorldAxis) delete m_pWorldAxis;
#endif
}

void CStartScene::OnProcessingMouseMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam)
{
	switch (nMessageID)
	{
	default:
		break;
	}

}

void CStartScene::OnProcessingKeyboardMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam)
{
	switch (nMessageID)
	{
	case WM_KEYDOWN:
		switch (wParam)
		{
		case VK_ESCAPE:
			PostQuitMessage(0);
			break;
		default:
			break;
		}
		break;
	default:
		break;
	}
}

CGameObject* CStartScene::PickObjectPointedByCursor(int xClient, int yClient, CCamera* pCamera)
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
	for (int i = 0; i < m_nObjects; i++)
	{
		float fHitDistance = FLT_MAX;
		nIntersected = m_ppObjects[i]->PickObjectByRayIntersection(xmvPickPosition, xmmtxView, &fHitDistance);
		if ((nIntersected > 0) && (fHitDistance < fNearestHitDistance))
		{
			fNearestHitDistance = fHitDistance;
			pNearestObject = m_ppObjects[i];
		}
	}
	return(pNearestObject);
}

void CStartScene::Animate(float fElapsedTime)
{
	for (auto pText : m_pTextObjects)
		pText->Animate(fElapsedTime);
}

void CStartScene::Render(HDC hDCFrameBuffer, CCamera* pCamera)
{
	CCamera* pActualCamera = m_pSceneCamera ? m_pSceneCamera : pCamera;

	CGraphicsPipeline::SetViewport(&pActualCamera->m_Viewport);
	CGraphicsPipeline::SetViewPerspectiveProjectTransform(&pActualCamera->m_xmf4x4ViewPerspectiveProject);

	for (auto pText : m_pTextObjects)
		pText->Render(hDCFrameBuffer, pActualCamera);

#ifdef _WITH_DRAW_AXIS
	CGraphicsPipeline::SetViewOrthographicProjectTransform(&pCamera->m_xmf4x4ViewOrthographicProject);
	m_pWorldAxis->SetRotationTransform(&m_pPlayer->m_xmf4x4World);
	m_pWorldAxis->Render(hDCFrameBuffer, pCamera);
#endif
}

void CStartScene::UpdateCamera(float fElapsedTime)
{
	if (m_pSceneCamera)
	{
		XMFLOAT3 eye = XMFLOAT3(0.0f, 0.0f, -10.0f);
		XMFLOAT3 look = XMFLOAT3(0.0f, 0.0f, 0.0f);
		XMFLOAT3 up = XMFLOAT3(0.0f, 1.0f, 0.0f);
		m_pSceneCamera->SetLookAt(eye, look, up);
		m_pSceneCamera->GenerateViewMatrix();
	}
}