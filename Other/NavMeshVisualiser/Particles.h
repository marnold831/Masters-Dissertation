#pragma once
#include "../../Common/Vector4.h"
#include <vector>

using namespace NCL::Maths;
class Particles
{
	
public:
	Particles(int numParticles);
	~Particles();

	std::vector<Vector4> GetPositions() const { return positions; }
	void SetPositions(std::vector<Vector4> _positions) { positions.clear();  positions = _positions; }

private:
	std::vector<Vector4>	positions;
	std::vector<Vector4>	velocities;
	int						numParticles;


};

