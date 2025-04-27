#include "StartScene.h"

CStartScene::CStartScene()
{
	m_pPlayer = NULL;
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

	// ���� �ؽ�Ʈ ������Ʈ ����
	CTextObject3D* pText1 = new CTextObject3D(L"3D ���� ���α׷��� 1");
	pText1->SetPosition(XMFLOAT3(-50.0f, 25.0f, 75.0f));
	m_pTextObjects.push_back(pText1);

	CTextObject3D* pText2 = new CTextObject3D(L"���¼�");
	pText2->SetPosition(XMFLOAT3(-10.0f, -15.0f, 50.0f));
	pText2->SetTargetSceneID(1);
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

	for (auto pExplosive : m_pExplosions)
		delete pExplosive;
	m_pExplosions.clear();

	if (m_pPlayer) {
		delete m_pPlayer;
		m_pPlayer = nullptr;
	}

#ifdef _WITH_DRAW_AXIS
	if (m_pWorldAxis) delete m_pWorldAxis;
#endif
}

void CStartScene::OnProcessingMouseMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam)
{
	switch (nMessageID)
	{
	case WM_MOUSEMOVE:
	{
		// ���콺 ��ġ�� ���
        int xClient = LOWORD(lParam);
        int yClient = HIWORD(lParam);

        // ���콺 ��ġ�� ���� �� ��ǥ ������Ʈ
        m_xmf3PickPosition.x = (float)xClient;
        m_xmf3PickPosition.y = (float)yClient;

        // ���콺�� �÷��� �ؽ�Ʈ ��ü ã��
		m_pHitText = dynamic_cast<CTextObject3D*>(PickObjectPointedByCursor(LOWORD(lParam), HIWORD(lParam), m_pPlayer->m_pCamera));

        // ��� �ؽ�Ʈ ������Ʈ ���� �ʱ�ȭ (������)
        for (auto& pText : m_pTextObjects)
            pText->SetColor(RGB(255, 0, 0));  // �⺻ ������

        // ���콺�� �ؽ�Ʈ ���� ������ ������ �Ķ������� ����
		if (m_pHitText)
		{
			m_pHitText->SetColor(RGB(0, 0, 255));  // �Ķ���
		}
	}
	break;
	case WM_LBUTTONDOWN:
		m_pHitText = dynamic_cast<CTextObject3D*>(PickObjectPointedByCursor(LOWORD(lParam), HIWORD(lParam), m_pPlayer->m_pCamera));
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
			CTextObject3D* parent = dynamic_cast<CTextObject3D*>((*it)->m_pParentObject);
			m_nNextSceneID = parent->GetTargetSceneID();
			delete (*it);
			it = m_pExplosions.erase(it);
			m_bExplosionFinished = true;
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

	// �Ķ� �� �׸��� (���콺 ��ġ�� ���� �� �׸���)
	HBRUSH hBrush = CreateSolidBrush(RGB(0, 0, 255));  // �Ķ��� ��
	SelectObject(hDCFrameBuffer, hBrush);
	Ellipse(hDCFrameBuffer,
		(int)m_xmf3PickPosition.x - 5, (int)m_xmf3PickPosition.y - 5,
		(int)m_xmf3PickPosition.x + 5, (int)m_xmf3PickPosition.y + 5);  // 10px ũ���� ��

	// ���� �귯�� ����
	DeleteObject(hBrush);

#ifdef _WITH_DRAW_AXIS
	CGraphicsPipeline::SetViewOrthographicProjectTransform(&pCamera->m_xmf4x4ViewOrthographicProject);
	m_pWorldAxis->SetRotationTransform(&m_pPlayer->m_xmf4x4World);
	m_pWorldAxis->Render(hDCFrameBuffer, pCamera);
#endif
}

void CStartScene::UpdateCamera(float fElapsedTime)
{
	if (m_pPlayer && m_pPlayer->GetCamera())
	{
		m_pPlayer->Update(fElapsedTime);
	}
}