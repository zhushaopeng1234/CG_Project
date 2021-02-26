#include "Snowflakes.h"

Snowflake::Snowflake()
{
	reset();
}

Snowflake::~Snowflake()
{
}

void Snowflake::reset()
{
	const float x = (float)(rand() % 2000) / 100.0f - 10.0f;
	const float z = (float)(rand() % 2000) / 100.0f - 10.0f;
	const float y = (float)(rand() % 1000) / 100.0f;
	position.set(x, 10.0f + y, z, 1.0f);
	timeOnGround = 3.0f;
}

void Snowflake::updateAnimation(const float timeDelta)
{
        if (position.y <= 0.1f){
		timeOnGround -= timeDelta;
		if (timeOnGround <= 0.0f) {
			reset();
		}
	} else {
		// let it snow	
            	position.y -= 2.0f * timeDelta;
        }
}

CGMatrix4x4 Snowflake::getModelMatrix() const
{
        return CGMatrix4x4::getTranslationMatrix(position[0], position[1], position[2]);
}

void Snowflake::render(CGContext& context) const
{
	static const float snowflakes[18*3] ={
	    -0.05f, 0, 0,
	    0.05f, 0.0, 0,
	    0, -0.05f, 0,
	    0, 0.05f, 0,
	    0, 0, -0.05f,
	    0, 0, 0.05f,
	    -0.04f, -0.04f, 0,
	    0.04f, 0.04f, 0,
	    -0.04f, 0.04f, 0,
	    0.04f, -0.04f, 0,
	    -0.04f, 0, -0.04f,
	    0.04f, 0, 0.04f
	    - 0.04f, 0, 0.04f,
	    0.04f, 0, -0.04f,
	    0, -0.04f, -0.04f,
	    0, 0.04f, 0.04f,
	    0, -0.04f, 0.04f,
	    0, 0.04f, -0.04f
	};

	static const float snowflakesColors[18*4] ={
	    1, 1, 1, 1,
	    1, 1, 1, 1,
	    1, 1, 1, 1,
	    1, 1, 1, 1,
	    1, 1, 1, 1,
	    1, 1, 1, 1,
	    1, 1, 1, 1,
	    1, 1, 1, 1,
	    1, 1, 1, 1,
	    1, 1, 1, 1,
	    1, 1, 1, 1,
	    1, 1, 1, 1,
	    1, 1, 1, 1,
	    1, 1, 1, 1,
	    1, 1, 1, 1,
	    1, 1, 1, 1,
	    1, 1, 1, 1,
	    1, 1, 1, 1
	};

        context.vertexAttributePointer.position =  snowflakes;
        context.vertexAttributePointer.color = snowflakesColors;
        context.drawArrays(CG_LINES, 0, 10);
}

//------------------------------------------------------------------------------

Snowflakes::Snowflakes(size_t count)
{
	snowflake.resize(count);
        for (size_t i = 0; i < count/ 2; i++){
            snowflake[i].position[1] = 0.0f;
        }
}

Snowflakes::~Snowflakes()
{
}

void Snowflakes::updateAnimation(const float timeDelta)
{
	for (size_t i=0; i<snowflake.size(); i++) {
		snowflake[i].updateAnimation(timeDelta);
	}
}

void Snowflakes::render(CGContext& context, CGMatrix4x4& modelView, const CGMatrix4x4& baseTrans) const
{
	for (size_t i=0; i<snowflake.size(); i++) {
		modelView = baseTrans * snowflake[i].getModelMatrix();
		snowflake[i].render(context);
	}
}
