#include "Deer.h"
#include "DeerVertexData.h"
#include "CGContext/CGMath.h"

#include <algorithm>

Deer::Deer()
{
	position.set(0.0f, 0.0f, 0.0f, 1.0f);
	target.set(0.0f, 0.0f, 0.0f, 1.0f);
	direction.set(1.0f, 0.0f, 0.0f, 0.0f);
	speedBoost=1.0f;
	size=1.0f;
}

Deer::~Deer()
{
}

bool Deer::updateAnimation(const float timeDelta)
{
	const float rotateSpeed = 3.0f*timeDelta*speedBoost, moveSpeed = 6.0f*timeDelta*speedBoost;
	CGVec4 targetVec = target - position;

	float speedFactor = 0.0f;
	float len = CGMath::length(targetVec);
	if (len > 0.0f) {
		targetVec /= len;
		speedFactor = std::max(CGMath::dot(direction, targetVec), 0.0f);
		direction += targetVec*rotateSpeed;
	}

	direction = CGMath::normalize(direction);
	position += direction * moveSpeed * speedFactor;

	return (len < 0.5f);
}

void Deer::render(CGContext& context) const
{
	context.vertexAttributePointer.position =  verticesDeer;
	context.vertexAttributePointer.color = colorsDeer;
	context.vertexAttributePointer.normal = normalsDeer;
	context.vertexAttributePointer.texcoord = texcoordsDeer;
	context.drawArrays(CG_TRIANGLES, 0, NUM_VERTS_DEER);
}

CGMatrix4x4 Deer::getModelMatrix() const
{
	float angle = acosf(direction[2]) * 180.0f / 3.1415926f;
	if (direction[0] < 0) {
		angle = 360.0f - angle;
	}
	CGMatrix4x4 model =  CGMatrix4x4::getTranslationMatrix(position[0], position[1], position[2]) * CGMatrix4x4::getRotationMatrixY(angle) * CGMatrix4x4::getScaleMatrix(size,size,size);
	return model;
}

//------------------------------------------------------------------------------

DeerHerd::DeerHerd(size_t count)
{
	deer.resize(count);
	targetIndex=0;

	for (size_t i=0; i<deer.size(); i++) {
		deer[i].position=CGVec4(10.0f * cosf(float(i)), 0.0f, 10.0f * sinf(float(i)), 1.0f);
		deer[i].direction = CGVec4(1.0f, 0.0f, 0.0f, 0.0f);
		deer[i].size=10.0f/(float)(i+10);
	}
}

DeerHerd::~DeerHerd()
{
}

void DeerHerd::updateAnimation(const float timeDelta)
{
	for (size_t i=0; i<deer.size(); i++) {
		if (i == 0) {
			deer[i].target = CGVec4(float(targetIndex % 7) / 6.0f * 16.0f - 8.0f, 0.0f, float(targetIndex % 4) / 3.0f*16.0f - 8.0f ,1.0f);
			deer[i].speedBoost = 1.0f;
		} else {
			deer[i].target = deer[i-1].position;
			deer[i].speedBoost = std::min(1.0f, CGMath::distance(deer[i-1].position, deer[i].position));
		}
		if (deer[i].updateAnimation(timeDelta)) {
			targetIndex = (targetIndex + 1) % 10;
		}
	}
}

