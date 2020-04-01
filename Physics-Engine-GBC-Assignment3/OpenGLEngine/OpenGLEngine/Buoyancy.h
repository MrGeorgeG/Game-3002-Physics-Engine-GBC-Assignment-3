#pragma once
#include "RigidbodyGravityForceGeneratorSystem.h"
#include "ECSConfig.h"

namespace Reality {

	struct Buoyancy
	{
	public:

		Buoyancy(const Vector3& center = Vector3(0,0,0), float maxdepth = 100, float vol = 1000, float waterheight = 450, float liquiddensity = 0.8f):
			maxDepth(maxdepth), volume(vol), waterHeight(waterheight), liquidDensity(liquiddensity), centerOfBuoyancy(center)
		{

		}
		float GetWaterHeight()
		{
			return waterHeight;
		}
		float GetLiquidDensity()
		{
			return liquidDensity;
		}
		Vector3 GetCenterOfBuoyancy()
		{
			return centerOfBuoyancy;
		}
	private:

		// when sumbmerged to maxDepth buoyancy force is at maximum
		float maxDepth;
		
		// volum of the object
		float volume;

		// hight of the water surface
		float waterHeight;
		
		// density
		float liquidDensity;

		// center of the force
		Vector3 centerOfBuoyancy;

	};
}

