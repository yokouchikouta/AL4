#include "Ground.h"
#include <cassert>

void Ground::Initialize(Model* model) {
	assert(model);

	model_ = model;

	worldTransform_.Initialize();
}

void Ground::Update() {}

void Ground::Draw(const ViewProjection& viewProjection) {
	model_->Draw(worldTransform_, viewProjection);
}