#include "StartLevel.h"
#include "GameplayStatics.h"
#include "Movie.h"


StartLevel::StartLevel()
{
	GameplayStatics::CreateObject<Movie>({ 0,0 });
}
