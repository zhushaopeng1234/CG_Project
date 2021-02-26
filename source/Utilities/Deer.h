#pragma once
#include <vector>
#include "Applications/Application.h"
class Deer
{
public:
	CGVec4 position;
	CGVec4 direction;
	CGVec4 target;
	float speedBoost;
	float size;

	Deer();
	~Deer();

	bool updateAnimation(const float timeDelta);
	void render(CGContext& context) const;
	CGMatrix4x4 getModelMatrix() const;
};

class DeerHerd
{
public:
	std::vector<Deer> deer;
	int targetIndex;

	DeerHerd(size_t count=3);
	~DeerHerd();

	void updateAnimation(const float timeDelta);
};

