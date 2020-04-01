#include "BoatSystem.h"
#include "LifeTimeComponent.h"

namespace Reality
{
	BoatSystem::BoatSystem()
	{
		requireComponent<BoatComponent>();
		requireComponent<RigidBodyComponent>();
		requireComponent<TransformComponentV2>();
	}

	void BoatSystem::Update(float deltaTime)
	{
		GLFWwindow* window = getWorld().data.renderUtil->window->glfwWindow;
		timer += deltaTime;
		for (auto e : getEntities())
		{
			auto& rigidbody = e.getComponent<RigidBodyComponent>();
			auto& transform = e.getComponent<TransformComponentV2>();
			auto& boat = e.getComponent<BoatComponent>();

			if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
			{
				rigidbody.AddPower(transform.LocalToWorldDirection(boat.energy + Vector3(0, N * 10, 0)));
				if (N > -10)
				{
					transform.SetAngX(false);
					N -= 0.5;
				}
			}

			if (glfwGetKey(window, GLFW_KEY_W) == GLFW_RELEASE)
			{
				if (N < 0)
				{
					transform.SetAngX(true);
					N += 0.5;
				}
			}

			if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
			{
				rigidbody.AddPower(transform.LocalToWorldDirection(-(boat.energy + Vector3(0, N * 10, 0))));
				if (N < 10)
				{
					transform.SetAngX(true);
					N += 0.5;
				}
			}

			if (glfwGetKey(window, GLFW_KEY_S) == GLFW_RELEASE)
			{
				if (N > 0)
				{
					transform.SetAngX(false);
					N -= 0.5;
				}
			}

			if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
			{
				transform.SetAngY(true);

			}

			if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
			{
				transform.SetAngY(false);
			}

		}
		if (getEntities().size() > 0)
		{
			float width = getWorld().data.renderUtil->window->width;
			float height = getWorld().data.renderUtil->window->height;

		}

	}
}
