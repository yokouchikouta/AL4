#pragma once

#include "MathUtilityForText.h"
#include "Model.h"
#include "WorldTransform.h"
#include "input.h"

class Player {
public:
	void Initialize(Model* model);

	void Update();

	void Draw(const ViewProjection& viewProjection);

	const WorldTransform& GetWorldTransform() { return worldTransform_; }

	void SetViewProjection(const ViewProjection* viewProjection) {
		viewProjection_ = viewProjection;
	}

private:
	WorldTransform worldTransform_;

	Model* model_ = nullptr;

	uint32_t textureHandle_ = 0u;
	Input* input_ = nullptr;

	const ViewProjection* viewProjection_ = nullptr;
};
