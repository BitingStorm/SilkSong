#include "VideoPlayer.h"
#include "easyx.h"
#pragma comment(lib, "strmiids.lib")
#include"Tools/ResourceManager.h"
#include"Core/World.h"



VideoPlayer::VideoPlayer()
{
	m_pGraph = nullptr;
	m_pControl = nullptr;
	m_pVW = nullptr;
	m_pEvent = nullptr;

	HRESULT hr = CoInitialize(nullptr);
	hr = CoCreateInstance(CLSID_FilterGraph, nullptr, CLSCTX_INPROC_SERVER, IID_IGraphBuilder, (void**)&m_pGraph);
	m_pGraph->QueryInterface(IID_IMediaControl, (void**)&m_pControl);
	m_pGraph->QueryInterface(IID_IVideoWindow, (void**)&m_pVW);
	m_pGraph->QueryInterface(IID_IMediaEventEx, (void**)&m_pEvent);

	RECT rct;
	GetClientRect(GetHWnd(), &rct);
	renderBox = FRect({}, rct.right - rct.left, rct.bottom - rct.top);

	OnVideoEnd.AddLambda([]() {mainWorld.videoLock = false; });
}

VideoPlayer::~VideoPlayer()
{
	m_pControl->Stop();
	m_pVW->put_Visible(OAFALSE);
	m_pVW->Release();
	m_pControl->Release();
	m_pGraph->Release();
	m_pEvent->Release();
	CoUninitialize();
}

void VideoPlayer::Update(float deltaTime)
{
	SceneComponent::Update(deltaTime);

	FVector2D leftTop = GetWorldPosition() - renderBox.GetHalf() + FVector2D(WIN_WIDTH, WIN_HEIGHT) * 0.5f;
	m_pVW->SetWindowPosition(leftTop.x, leftTop.y, renderBox.GetSize().x, renderBox.GetSize().y);

	long lEventCode; 
	LONG_PTR lParam1, lParam2;
	if (SUCCEEDED(m_pEvent->GetEvent(&lEventCode, &lParam1, &lParam2, 0))) 
	{
		if (lEventCode == EC_COMPLETE) 
		{
			OnVideoEnd.BroadCast();
		}
		m_pEvent->FreeEventParams(lEventCode, lParam1, lParam2); 
	}
}

void VideoPlayer::Load(std::string name)
{
	std::wstring pszFileName = mainWorld.resourcePool->FetchVideo(name);
	m_pGraph->RenderFile(pszFileName.c_str(), nullptr);

	HWND hWnd = GetHWnd();
	m_pVW->put_Owner((OAHWND)hWnd);
	m_pVW->put_WindowStyle(WS_CHILD | WS_CLIPSIBLINGS);

	FVector2D leftTop = GetWorldPosition() - renderBox.GetHalf() + FVector2D(WIN_WIDTH, WIN_HEIGHT) * 0.5f;
	m_pVW->SetWindowPosition(leftTop.x, leftTop.y, renderBox.GetSize().x, renderBox.GetSize().y);
}

void VideoPlayer::Play()
{
	m_pControl->Run();
	mainWorld.videoLock = true;
}

void VideoPlayer::Pause()
{
	m_pControl->Pause();
}

void VideoPlayer::Stop()
{
	m_pControl->Stop();
	m_pVW->put_Visible(OAFALSE);
}

void VideoPlayer::SetRect(FRect rct)
{
	renderBox = rct;
}
