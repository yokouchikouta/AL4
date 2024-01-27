#include "Player.h"
#include "ImGuiManager.h"
#include"MathUtilityForText.h"
#include"Vector3.h"
#include <cassert>



void Player::Initialize(const std::vector<Model*>& models) {
	////Nullポインタチェック
	//assert(modelBody);
	//assert(modelHead);
	//assert(modelL_arm);
	//assert(modelR_arm);
	//assert(model);

	// textureHandle_ = textureHandle;

	/*modelBody_ = modelBody;
	modelHead_ = modelHead;
	modelL_arm_ = modelL_arm;
	modelR_arm_ = modelR_arm;*/
	BaseCharacter::Initialize(models);
	//ワールド変換の初期化
	worldTransform_.Initialize();
	//worldTransformBase_.Initialize();

	worldTransformBody_.Initialize();
	worldTransformBody_.parent_ = &worldTransform_;

	worldTransformHead_.Initialize();
	worldTransformHead_.parent_ = &worldTransformBody_;
	worldTransformHead_.translation_.y = 1.5038f;

	worldTransformL_arm_.Initialize();
	worldTransformL_arm_.parent_ = &worldTransformBody_;
	worldTransformL_arm_.translation_.x = -0.5275f;
	worldTransformL_arm_.translation_.y = 1.2619f;

	worldTransformR_arm_.Initialize();
	worldTransformR_arm_.parent_ = &worldTransformBody_;
	worldTransformR_arm_.translation_.x = +0.5275f;
	worldTransformR_arm_.translation_.y = 1.2619f;
	// インプット
	input_ = Input::GetInstance();
	//浮遊ギミックの初期化
	InitializeFloatingGimmick();
}
//
//void Player::Initialize(const std::vector<Model*>& models) {}

void Player::Update() {
	worldTransform_.TransferMatrix();
	//浮遊ギミックの更新
	UpdateFloatingGimmick();

	const float speed = 0.3f;
	Vector3 move = {0.0f, 0.0f, 0.0f};
	if (input_->PushKey(DIK_RIGHT)) {
		move.x = 1.0f;
	}
	if (Input::GetInstance()->PushKey(DIK_LEFT)) {
		move.x = -1.0f;
	}
	if (Input::GetInstance()->PushKey(DIK_UP)) {
		move.z = 1.0f;
	}
	if (Input::GetInstance()->PushKey(DIK_DOWN)) {
		move.z = -1.0f;
	}

	move = Normalize(move) * speed;

	if (viewProjection_) {
		// カメラの回転行列
		Matrix4x4 matRotX = MakeRotateXMatrix(viewProjection_->rotation_.x);
		Matrix4x4 matRotY = MakeRotateYMatrix(viewProjection_->rotation_.y);
		Matrix4x4 matRotZ = MakeRotateZMatrix(viewProjection_->rotation_.z);
		// 回転行列の合成
		Matrix4x4 matRot = matRotZ * matRotX * matRotY;

		move = TransformNormal(move, matRot);
	}
	worldTransform_.translation_ += move;
	// 移動ベクトルのY軸周り角度
	worldTransform_.rotation_.y = std::atan2(move.x, move.z);

	//// 変換行列を更新
	//worldTransform_.matWorld_ = MakeAffineMatrix(
	//    worldTransform_.scale_, worldTransform_.rotation_, worldTransform_.translation_);
	//worldTransform_.TransferMatrix();
	worldTransform_.UpdateMatrix();
	worldTransformBody_.UpdateMatrix();
	worldTransformHead_.UpdateMatrix();
	worldTransformL_arm_.UpdateMatrix();
	worldTransformR_arm_.UpdateMatrix();

	ImGui::Begin("Player");
	ImGui::SliderFloat3(
	    "Head Translation", reinterpret_cast<float*>(&worldTransformHead_.translation_), -10.0f,
	    10.0f);
	ImGui::SliderFloat3(
	    "ArmL Translation", reinterpret_cast<float*>(&worldTransformL_arm_.translation_), -10.0f,
	    10.0f);
	ImGui::SliderFloat3(
	    "ArmR Translation", reinterpret_cast<float*>(&worldTransformR_arm_.translation_), -10.0f,
	    10.0f);
	ImGui::SliderInt("floatingCycle", reinterpret_cast<int*>(&floatingCycle_), 1, 200);
	ImGui::SliderFloat("floatingAmplitude", &floatingAmplitude_, 0.0f, 10.0f);
	ImGui::SliderFloat("idleArmAngleMax_", &idleArmAngleMax_, 0.0f, 180.0f);
	ImGui::End();
}

void Player::Draw(const ViewProjection& viewProjection) {
	//3Dモデルを描画
	models_[kModelIndexBody]->Draw(worldTransformBody_, viewProjection);
	models_[kModelIndexHead]->Draw(worldTransformHead_, viewProjection);
	models_[kModelIndexL_arm]->Draw(worldTransformL_arm_, viewProjection);
	models_[kModelIndexR_arm]->Draw(worldTransformR_arm_, viewProjection);

}

void Player::InitializeFloatingGimmick() { floatingParameter_ = 0.0f; }

void Player::UpdateFloatingGimmick() {

	floatingParameter_ += 0.1f;
	worldTransformBody_.translation_.y = std::sin(floatingParameter_) * 0.2f;
	worldTransformL_arm_.rotation_.x = std::sin(floatingParameter_) * 0.5f;
	worldTransformR_arm_.rotation_.x = std::sin(floatingParameter_) * 0.5f;
	//const float floatingStep = PI * 2.0f / floatingCycle_;
	////浮遊ギミックを更新
	//floatingParameter_ += floatingStep;
	////2πを超えたら０に戻す
	//floatingParameter_ = fmodf(floatingParameter_, PI * 2.0f);
	////浮遊を座標に反映
	//worldTransformBody_.translation_.y = std::sin(floatingParameter_) * floatingAmplitude_;

	//worldTransformL_arm_.rotation_.x =
	//    std::sin(floatingParameter_) * idleArmAngleMax_ / 180.0f * PI;
	//worldTransformR_arm_.rotation_.x =
	//    std::sin(floatingParameter_) * idleArmAngleMax_ / 180.0f * PI;
	       
}