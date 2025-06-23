#pragma once
#include "Objects/Actor.h"


class Movie :public Actor
{
public:
	Movie();
protected:
	class VideoPlayer* player;
};
