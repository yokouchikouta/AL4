﻿#include "FollowCamera.h"
#include "Input.h"
#include "MathUtilityForText.h"

void FollowCamera::Initialize() {
	viewProjection_.farZ = 2000.0f;
	viewProjection_.Initialize();
}
void FollowCamera::Update() {

	if (Input::GetInstance()->PushKey(DIK_SPACE)) {
		viewProjection_.rotation_.y += 0.03f;
	}
	// 追従対象がいれば
	if (target_) {
		// 追従対象からカメラまでオフセット
		Vector3 offset = {0.0f, 2.0f, -10.0f};

		Matrix4x4 matRotX = MakeRotateXMatrix(viewProjection_.rotation_.x);
		Matrix4x4 matRotY = MakeRotateYMatrix(viewProjection_.rotation_.y);
		Matrix4x4 matRotZ = MakeRotateZMatrix(viewProjection_.rotation_.z);
		// 回転行列の合成
		Matrix4x4 matRot = matRotX * matRotY;

		// オフセットわカメラの回転に合わせて回転させる
		offset = TransformNormal(offset, matRot);
		// 座標をコピーしてオフセット分ずらす
		viewProjection_.translation_ = target_->translation_ + offset;
	}
	viewProjection_.UpdateMatrix();
}