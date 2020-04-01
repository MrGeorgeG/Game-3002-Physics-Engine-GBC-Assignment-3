#pragma once
#include "ECSConfig.h"

namespace Reality
{
	struct BoatComponent
	{
		BoatComponent(Vector3 _energy = Vector3(0.0f, 0.0f, 1000.0f))
			:energy(_energy)
		{

		}
		Vector3 energy;
	};
}