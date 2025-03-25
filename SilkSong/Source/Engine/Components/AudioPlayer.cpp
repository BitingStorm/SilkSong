#include "AudioPlayer.h"
#include "Core/World.h"
#include "Components/Camera.h"



using std::string;


AudioPlayer::~AudioPlayer()
{
	for (auto& obj : paths)
	{
		mciSendString((string("stop ") + obj).c_str(), 0, 0, 0);
	}
}

void AudioPlayer::Update(float deltaTime)
{
	Super::Update(deltaTime);

	if (!bIsEnabled || !bSpacial)return;

	int32 vol = volume;
	float dis = FVector2D::Distance(mainWorld.mainCamera->GetWorldPosition(), GetWorldPosition());
	if (dis <= innerRadius) {  }
	else if (dis > outerRadius)vol = 0;
	else vol = int32((outerRadius - dis) * volume / (outerRadius - innerRadius));
	vol = FMath::Clamp(vol, 0, 1000);
	if (currentVol == vol)return;
	currentVol = vol;
	for (auto& obj : paths)
	{
		string file = string("setaudio ") + obj + string(" volume to ") + std::to_string(Reflect(vol));
		mciSendString(file.c_str(), NULL, 0, NULL);
	}
}

void AudioPlayer::SetVolume(int32 vol)
{
	volume = FMath::Clamp(vol, 0, 1000);
	if (bSpacial)return;
	for (auto& obj : paths)
	{
		string file = string("setaudio ") + obj + string(" volume to ") + std::to_string(Reflect(vol));
		mciSendString(file.c_str(), NULL, 0, NULL);
	}
}

void AudioPlayer::Play(string name, bool repeat)
{
	if (paths.find(name) == paths.end())paths.insert(name);

	if (repeat && IsPlaying(name)) {}
	else mciSendString((string("seek ") + name + string(" to start")).c_str(), 0, 0, 0);
	mciSendString((string("play ") + name + (repeat ? string(" repeat") : "")).c_str(), 0, 0, 0);

	if (bSpacial)return;
	string file = string("setaudio ") + name + string(" volume to ") + std::to_string(Reflect(volume));
	mciSendString(file.c_str(), NULL, 0, NULL);
}

void AudioPlayer::Stop(string name)
{
	mciSendString((string("stop ") + name).c_str(), 0, 0, 0);
}

void AudioPlayer::Close(string name)
{
	paths.erase(name);
	mciSendString((string("close ") + name).c_str(), 0, 0, 0);
}

void AudioPlayer::Pause(string name)
{
	mciSendString((string("pause ") + name).c_str(), 0, 0, 0);
}

void AudioPlayer::Resume(string name)
{
	mciSendString((string("resume ") + name).c_str(), 0, 0, 0);
}

bool AudioPlayer::IsPlaying(std::string name)
{
	char statusString[128];
	mciSendString((string("status ") + name + " mode").c_str(), statusString, sizeof(statusString), NULL);
	return strcmp(statusString, "playing") == 0;
}

int32 AudioPlayer::Reflect(int32 x)
{
	return 10 * FMath::Pow(float(x) / 100.f, 2.f);
}
