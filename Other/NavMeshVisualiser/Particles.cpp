#include "Particles.h"
#include <cmath>
#include <execution>
#include <fstream>

Particles::Particles(int numParticles) : numParticles(numParticles)
{
	drawPositions.resize(numParticles);

	for (int i = 0; i < numParticles; ++i) {
		drawPositions[i] = Vector4();
	}

	directionPositions.resize(numParticles);
	for (int i = 0; i < numParticles; ++i) {
		directionPositions[i] = Vector4();
	}
}

Particles::~Particles()
{

}
float OffsetCalcX(float time) {
	if (time <= 0) {
		return 1;
	}
	float offset = sin(time) + cos(2 * time) + cos(time);
	return offset;
}

float OffsetCalcY(float time) {
	if (time <= 0) {
		return 0;
	}

	float offset = sin(time * 2) + sin(time);
	return offset;
}

void Particles::UpdatePositions(float dt) {
	Vector3 centrePos(0, 0, 0);

	for (int i = 0; i < numParticles; ++i) {
		Vector3 currentPos = directionPositions[i];

		Vector3 dir = centrePos - currentPos;
		dir.Normalise();
		
		Vector3 distanceToMove =  dir * -0.1;
		Vector3 newPos = currentPos + distanceToMove;
		directionPositions[i] = Vector4(newPos.x, newPos.y, newPos.z, 0.0f);

		Vector3 perpDirX = Vector3::Cross(dir, Vector3(0, 1, 0));
		if (perpDirX.y > 0.999) {
			perpDirX = Vector3::Cross(dir, Vector3(1, 0, 0));
		}
		float newW = drawPositions[i].w - dt;

		
		float offSetSinX = OffsetCalcX(newW);
		Vector3 offsetX = perpDirX * offSetSinX;

		float offsetSinY = OffsetCalcY(newW/2);
		Vector3 perpDirY = Vector3::Cross(perpDirX, Vector3(0, 0, 1));
		if (perpDirY.z > 0.9999) {
			perpDirY = Vector3::Cross(perpDirX, Vector3(0, 1, 0));
		}
		Vector3 offsetY = perpDirY * offsetSinY;

		newPos = newPos + offsetX * 5 + offsetY * 5;
		
		drawPositions[i] = Vector4(newPos.x, newPos.y, newPos.z, newW);
	}
	
}
