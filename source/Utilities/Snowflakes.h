#pragma once

#include <vector>
#include "Applications/Application.h"

class Snowflake
{
public:
	CGVec4 position;
	float timeOnGround;

	Snowflake();
	~Snowflake();

	void reset();
	void updateAnimation(const float timeDelta);
	CGMatrix4x4 getModelMatrix() const;
	void render(CGContext& context) const;
};

class Snowflakes
{
public:
	std::vector<Snowflake> snowflake;

	Snowflakes(size_t count);
	~Snowflakes();

	void updateAnimation(const float timeDelta);
	void render(CGContext& context, CGMatrix4x4& modelView, const CGMatrix4x4& baseTrans) const;
};
