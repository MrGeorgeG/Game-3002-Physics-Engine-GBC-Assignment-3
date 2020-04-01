#pragma once
#include "ECSConfig.h"
#include "BoatComponent.h"
#include "RigidBodyComponent.h"
#include "TransformComponentV2.h"

namespace Reality
{
	class BoatSystem : public ECSSystem
	{
	public:
		BoatSystem();
		void Update(float deltaTime);
	private:
		float timer = 0;
		float N = 0;
		Reality::Vector3 radius = glm::vec3(glm::radians(0.0f), glm::radians(0.0f), glm::radians(0.0f));
	};
}
