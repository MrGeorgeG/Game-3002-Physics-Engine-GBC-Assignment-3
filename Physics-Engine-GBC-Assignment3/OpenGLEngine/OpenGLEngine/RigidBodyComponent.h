#pragma once
#include "ECSConfig.h"

namespace Reality
{
	struct RigidBodyComponent
	{
		RigidBodyComponent(float _quality = 1.0f, float _linearDamping = 0.4f, float _angularDamping = 0.3f, Vector3 _velocity = Vector3(0, 0, 0), Vector3 _angularVelocity = Vector3(0, 0, 0), float _gravity = 1)
			: reverseQuality(1.0f / _quality), rp3dId(-1), velocity(_velocity), angularVelocity(_angularVelocity), linearDamping(_linearDamping), angularDamping(_angularDamping), gravity(_gravity)
		{
			inertiaTensor = glm::mat3(0.0f);
			inertiaTensor[0][0] = 0.001f;
			inertiaTensor[1][1] = 0.001f;
			inertiaTensor[2][2] = 0.001f;
			energy = Vector3(0, 0, 0);
			accelaration = Vector3(0, 0, 0);
			angularAccelaration = Vector3(0, 0, 0);
			torqueAccumulator = Vector3(0, 0, 0);
		}
		int rp3dId;

		float reverseQuality;
		float gravity;
		float linearDamping;
		float angularDamping;
		
		Vector3 velocity;
		Vector3 angularVelocity;
		Vector3 accelaration;
		Vector3 angularAccelaration;
		Mat3 inertiaTensor;
		

		Mat3 worldInverseInertiaTensor(Mat3 localToWorld)
		{
			Mat3 worldInertiaTensor = localToWorld * inertiaTensor * glm::inverse(localToWorld);
			return glm::inverse(worldInertiaTensor);
		}

		inline void AddPower(Vector3 power)
		{
			energy += power;
		}
		inline Vector3 GetPower()
		{
			return energy;
		}
		inline void ResetForceAccumulator()
		{
			energy = Vector3(0, 0, 0);
		}
		inline void AddForceAtPoint(Vector3 power, Vector3 point, Vector3 origin)
		{
			AddPower(power);
			AddTorque(glm::cross(point - origin, power));
		}
		inline void AddTorque(Vector3 torque)
		{
			torqueAccumulator += torque;
		}
		inline Vector3 GetTorque()
		{
			return torqueAccumulator;
		}
		inline void ResetTorqueAccumulator()
		{
			torqueAccumulator = Vector3(0, 0, 0);
		}
	private:
		Vector3 energy;
		Vector3 torqueAccumulator;
	};
}
