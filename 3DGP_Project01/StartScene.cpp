#include "StartScene.h"

CStartScene::CStartScene()
{
	m_pPlayer = nullptr;
}

CStartScene::~CStartScene()
{
	ReleaseObjects();
}

void CStartScene::BuildObjects()
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

	CTextObject* pText1 = new CTextObject(L"3D 게임 프로그래밍 1");
	pText1->SetPosition(XMFLOAT3(-50.0f, 25.0f, 75.0f));
	m_pTextObjects.push_back(pText1);

	CTextObject* pText2 = new CTextObject(L"김태순");
	pText2->SetPosition(XMFLOAT3(-10.0f, -15.0f, 50.0f));
	pText2->SetTargetSceneID(1);
	m_pTextObjects.push_back(pText2);
}

void CStartScene::ReleaseObjects()
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

void CStartScene::OnProcessingMouseMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam)
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

void CStartScene::Animate(float fElapsedTime)
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
			delete (*it);
			it = m_pExplosions.erase(it);
			m_bSceneFinished = true;
		}
		else ++it;
	}
}

void CStartScene::Render(HDC hDCFrameBuffer)
{
	CCamera* pCamera = m_pPlayer->GetCamera();

	CGraphicsPipeline::SetViewport(&pCamera->m_Viewport);
	CGraphicsPipeline::SetViewPerspectiveProjectTransform(&pCamera->m_xmf4x4ViewPerspectiveProject);

	for (auto pText : m_pTextObjects)
		pText->Render(hDCFrameBuffer, pCamera);

	for (auto pExplosion : m_pExplosions)
		pExplosion->Render(hDCFrameBuffer, pCamera);

}

void CStartScene::UpdateCamera(float fElapsedTime)
{
	if (m_pPlayer && m_pPlayer->GetCamera())
	{
		m_pPlayer->Update(fElapsedTime);
	}
}