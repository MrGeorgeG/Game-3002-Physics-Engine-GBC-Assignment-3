#include "RigidbodyGravityForceGeneratorSystem.h"

namespace Reality
{
	RigidbodyGravityForceGeneratorSystem::RigidbodyGravityForceGeneratorSystem()
	{
		requireComponent<RigidBodyComponent>();
	}

	void RigidbodyGravityForceGeneratorSystem::Update(float deltaTime)
	{
		for (auto e : getEntities())
		{
			auto &body = e.getComponent<RigidBodyComponent>();
			if (body.reverseQuality > 0)
			{
				body.AddPower(gravity * body.gravity / body.reverseQuality);
			}
		}
	}
}
