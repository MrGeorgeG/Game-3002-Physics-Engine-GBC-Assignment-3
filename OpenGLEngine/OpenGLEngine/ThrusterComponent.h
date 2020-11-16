#pragma once
#include "ECSConfig.h"

namespace Reality
{
	struct ThrusterComponent
	{
		ThrusterComponent(ECSEntity _targetEntity = ECSEntity(),
			Vector3 _localThrustDirection = Vector3(0, 0, 1),
			float _thrust = 200,
			const std::vector<int>& _positiveKeys = {}, const std::vector<int>& _negetiveKeys = {})
			:targetEntity(_targetEntity),
			localThrustDirection(_localThrustDirection),
			thrust(_thrust),
			positiveKeys(_positiveKeys),
			negetiveKeys(_negetiveKeys)
		{

		}
		ECSEntity targetEntity;
		Vector3 localThrustDirection;
		float thrust;
		float timer = 0;
		std::vector<int> positiveKeys;
		std::vector<int> negetiveKeys;
		float controlSpeed = 30.0f;
	};
}
