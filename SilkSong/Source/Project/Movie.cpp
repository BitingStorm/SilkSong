#include "Movie.h"
#include "Components/VideoPlayer.h"
#include "GameplayStatics.h"


Movie::Movie()
{
	player = ConstructComponent<VideoPlayer>();
	player->AttachTo(root);

	player->AddPosition({0, -1});
	player->SetRect(FRect({}, WIN_WIDTH, WIN_HEIGHT - 50));
	player->Load("video_start");
	player->Play();

	player->OnVideoEnd.AddLambda([]() {GameplayStatics::OpenLevel("RuinHouse"); });
}
