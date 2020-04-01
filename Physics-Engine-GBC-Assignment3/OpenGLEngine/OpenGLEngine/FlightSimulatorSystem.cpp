#include "FlightSimulatorSystem.h"
#include "LifeTimeComponent.h"

namespace Reality
{
	FlightSimulatorSystem::FlightSimulatorSystem()
	{
		requireComponent<FlighSimulatorComponent>();
		requireComponent<RigidBodyComponent>();
		requireComponent<TransformComponentV2>();
	}

	void FlightSimulatorSystem::Update(float deltaTime)
	{
		timer += deltaTime;
		for (auto e : getEntities())
		{
			auto& rigidbody = e.getComponent<RigidBodyComponent>();
			auto& transform = e.getComponent<TransformComponentV2>();
			auto& flight = e.getComponent<FlighSimulatorComponent>();

			rigidbody.AddPower(transform.LocalToWorldDirection(flight.propulsion));

		}

		if (getEntities().size() > 0)
		{
			float width = getWorld().data.renderUtil->window->width;
			float height = getWorld().data.renderUtil->window->height;

		}

	}
}
