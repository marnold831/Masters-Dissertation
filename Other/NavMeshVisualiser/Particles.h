#pragma once
#include "../../Common/Vector4.h"
#include "../../Common/Vector3.h"
#include <vector>

using namespace NCL::Maths;
class Particles
{
	
public:
	Particles(int numParticles);
	~Particles();

	void UpdatePositions(float dt);

	void SetDirectionPositions(std::vector<Vector4> pos) { directionPositions = pos; }
	void SetDrawPositions(std::vector<Vector4> pos) { drawPositions = pos; }
	std::vector<Vector4> GetDrawPositions() const { return drawPositions; }

	int GetNumberParticles() const { return numParticles; }

private:
	std::vector<Vector4>	drawPositions;
	std::vector<Vector4>	directionPositions;
	int						numParticles;


};

