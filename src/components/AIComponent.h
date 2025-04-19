#pragma once

enum class Behaviours
{
	BounceSimple,
	SquashStretch
};

struct AIComponent
{
	Behaviours behaviour;
};