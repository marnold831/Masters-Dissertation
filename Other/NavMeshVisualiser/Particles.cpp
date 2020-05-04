#include "Particles.h"
#include <cmath>
#include <execution>

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
float OffsetCalcSin(float time) {
	float RadConvert = 3.14159265358979323846 / 180;
	float offset = sin(0.5 * time * RadConvert) + pow(cos(time * 2 * RadConvert), 0.3333333333);
	//if (offset < 0.1) {
	//	std::cout << "input : " << (0.5 * time * RadConvert) << ", giving : " << "offset : " << offset << std::endl;
	//}
	std::cout << "offset : " << offset << std::endl;
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

		//float offSetSin = std::sin(newW * (3.141592653589 / 180) * 10);
		float offSetSin = OffsetCalcSin(newW);
		Vector3 offsetX = perpDirX * offSetSin;

		Vector3 perpDirY = Vector3::Cross(perpDirX, Vector3(0, 1, 0));
		Vector3 offsetY = perpDirY * offSetSin;

		newPos = newPos + offsetX * 100/* + offsetY * 100*/;
		

		drawPositions[i] = Vector4(newPos.x, newPos.y, newPos.z, newW);
	}
}
