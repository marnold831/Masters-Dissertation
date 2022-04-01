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

	void SetDirectionPositions(std::unique_ptr<Vector4> pos) { directionPositions = std::move(pos); }
	void SetDrawPositions(std::unique_ptr<Vector4> pos) { drawPositions = std::move(pos); }
	std::vector<Vector4> GetDrawPositions() const { return drawPositions->; }

	int GetNumberParticles() const { return numParticles; }

private:
	std::unique_ptr<Vector4>	drawPositions;
	std::unique_ptr<Vector4>	directionPositions;
	int						numParticles;


};

