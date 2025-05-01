#include "MenuScene.h"

CMenuScene::CMenuScene()
{
	m_pPlayer = nullptr;
}

CMenuScene::~CMenuScene()
{
	ReleaseObjects();
}

void CMenuScene::BuildObjects()
{
	CExplosiveObject::PrepareExplosion();

	CCamera* pCamera = new CCamera();
	pCamera->SetViewport(0, 0, FRAMEBUFFER_WIDTH, FRAMEBUFFER_HEIGHT);
	pCamera->GeneratePerspectiveProjectionMatrix(1.01f, 500.0f, 60.0f);
	pCamera->SetFOVAngle(60.0f);

	pCamera->GenerateOrthographicProjectionMatrix(1.01f, 50.0f, FRAMEBUFFER_WIDTH, FRAMEBUFFER_HEIGHT);

	m_pPlayer = new CPlayer();
	m_pPlayer->SetPosition(0.0f, 0.0f, 0.0f);
	m_pPlayer->SetMesh(NULL);
	m_pPlayer->SetColor(RGB(0, 0, 255));
	m_pPlayer->SetCamera(pCamera);
	m_pPlayer->SetCameraOffset(XMFLOAT3(0.0f, 0.0f, -10.0f));

	float textZpos = 100.0f;

	// TUTORIAL (가장 위 중앙)
	CTextObject* pText1 = new CTextObject(L"TUTORIAL");
	pText1->SetPosition(XMFLOAT3(-30.0f, 30.0f, textZpos));
	pText1->SetTargetSceneID(4);
	m_pTextObjects.push_back(pText1);

	// LEVEL-1 (왼쪽 아래)
	CTextObject* pText2 = new CTextObject(L"LEVEL-1");
	pText2->SetPosition(XMFLOAT3(-70.0f, 5.0f, textZpos));
	pText2->SetTargetSceneID(2);
	m_pTextObjects.push_back(pText2);

	// LEVEL-2 (오른쪽 아래)
	CTextObject* pText3 = new CTextObject(L"LEVEL-2");
	pText3->SetPosition(XMFLOAT3(20.0f, 5.0f, textZpos));
	pText3->SetTargetSceneID(3);
	m_pTextObjects.push_back(pText3);

	// START (왼쪽 맨 아래)
	CTextObject* pText4 = new CTextObject(L"START");
	pText4->SetPosition(XMFLOAT3(-60.0f, -20.0f, textZpos));
	pText4->SetTargetSceneID(2);
	m_pTextObjects.push_back(pText4);

	// END (오른쪽 맨 아래)
	CTextObject* pText5 = new CTextObject(L"END");
	pText5->SetPosition(XMFLOAT3(40.0f, -20.0f, textZpos));
	pText5->SetTargetSceneID(-2);
	m_pTextObjects.push_back(pText5);
}

void CMenuScene::ReleaseObjects()
{
	for (auto pText : m_pTextObjects)
		delete pText;
	m_pTextObjects.clear();

	for (auto pExplosive : m_pExplosions)
		delete pExplosive;
	m_pExplosions.clear();

	if (m_pPlayer) {
		delete m_pPlayer;
		m_pPlayer = nullptr;
	}
}

void CMenuScene::OnProcessingMouseMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam)
{
	switch (nMessageID)
	{
	case WM_MOUSEMOVE:
	{
		m_pHitText = dynamic_cast<CTextObject*>(PickObjectPointedByCursor(LOWORD(lParam), HIWORD(lParam), m_pPlayer->m_pCamera));

		for (auto& pText : m_pTextObjects)
			pText->SetColor(RGB(255, 0, 0));

		if (m_pHitText)
		{
			m_pHitText->SetColor(RGB(0, 0, 255));
		}
	}
	break;
	case WM_LBUTTONDOWN:
		m_pHitText = dynamic_cast<CTextObject*>(PickObjectPointedByCursor(LOWORD(lParam), HIWORD(lParam), m_pPlayer->m_pCamera));
		if (m_pHitText)
		{
			CExplosiveObject* pExplosion = new CExplosiveObject();
			pExplosion->SetMesh(nullptr);
			pExplosion->SetColor(m_pHitText->m_dwColor);
			pExplosion->SetPosition(Vector3::Add(m_pHitText->GetPosition(), m_pHitText->getCenter()));
			pExplosion->SetRotationAxis(m_pHitText->m_xmf3RotationAxis);
			pExplosion->SetRotationSpeed(90.06f);
			pExplosion->SetMovingDirection(XMFLOAT3(-0.0f, 0.0f, -1.0f));
			pExplosion->SetMovingSpeed(15.0f);
			pExplosion->m_pParentObject = m_pHitText;
			pExplosion->m_bBlowingUp = true;
			m_pExplosions.push_back(pExplosion);

			m_pHitText->SetActive(false);
		}
		break;
	default:
		break;
	}

}

void CMenuScene::OnProcessingKeyboardMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam)
{
	switch (nMessageID)
	{
	case WM_KEYDOWN:
		switch (wParam)
		{
		case VK_ESCAPE:
			m_nNextSceneID = 0;
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

CGameObject* CMenuScene::PickObjectPointedByCursor(int xClient, int yClient, CCamera* pCamera)
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

	for (auto& pTextObject : m_pTextObjects)
	{
		float fHitDistance = FLT_MAX;
		nIntersected = pTextObject->PickObjectByRayIntersection(xmvPickPosition, xmmtxView, &fHitDistance);
		if ((nIntersected > 0) && (fHitDistance < fNearestHitDistance))
		{
			fNearestHitDistance = fHitDistance;
			pNearestObject = pTextObject;
		}
	}

	return(pNearestObject);
}

void CMenuScene::Animate(float fElapsedTime)
{
	if (m_pPlayer) m_pPlayer->Animate(fElapsedTime);

	for (auto& pText : m_pTextObjects)
		pText->Animate(fElapsedTime);

	for (auto it = m_pExplosions.begin(); it != m_pExplosions.end(); )
	{
		(*it)->Animate(fElapsedTime);

		if (!(*it)->m_bBlowingUp) {
			CTextObject* parent = dynamic_cast<CTextObject*>((*it)->m_pParentObject);
			m_nNextSceneID = parent->GetTargetSceneID();

			if (m_nNextSceneID == -2) {
				PostQuitMessage(0);
				return;
			}

			delete (*it);
			it = m_pExplosions.erase(it);
			m_bSceneFinished = true;
		}
		else ++it;
	}
}

void CMenuScene::Render(HDC hDCFrameBuffer)
{
	CCamera* pCamera = m_pPlayer->GetCamera();

	CGraphicsPipeline::SetViewport(&pCamera->m_Viewport);
	CGraphicsPipeline::SetViewPerspectiveProjectTransform(&pCamera->m_xmf4x4ViewPerspectiveProject);

	for (auto pText : m_pTextObjects)
		pText->Render(hDCFrameBuffer, pCamera);

	for (auto pExplosion : m_pExplosions)
		pExplosion->Render(hDCFrameBuffer, pCamera);
}

void CMenuScene::UpdateCamera(float fElapsedTime)
{
	if (m_pPlayer && m_pPlayer->GetCamera())
	{
		m_pPlayer->Update(fElapsedTime);
	}
}