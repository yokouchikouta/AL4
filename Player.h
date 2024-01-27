#pragma once

#include"BaseCharacter.h"
#include "MathUtilityForText.h"
#include "Model.h"
#include "WorldTransform.h"
#include "input.h"

class Player : public BaseCharacter {
public:
	enum {
		kModelIndexBody,
		kModelIndexHead,
		kModelIndexL_arm,
		kModelIndexR_arm,

	};

	// 座標の獲得
	float GetX() { return worldTransform_.matWorld_.m[3][0]; }
	float GetY() { return worldTransform_.matWorld_.m[3][1]; }
	float GetZ() { return worldTransform_.matWorld_.m[3][2]; }


	void Initialize(const std::vector<Model*>&models)override;

	void Update()override;

	void Draw(const ViewProjection& viewProjection)override;

	const WorldTransform& GetWorldTransform() { return worldTransform_; }

	void SetViewProjection(const ViewProjection* viewProjection) {
		viewProjection_ = viewProjection;
	}

private:
	//浮遊移動のサイクル<sec>
	int32_t floatingCycle_ = 90;
    //浮遊振幅
	float floatingAmplitude_ = 0.2f;
	//待機モーションの腕角度最大値
	float idleArmAngleMax_ = 30.0f;
	//ワールド変換データ
	//WorldTransform worldTransformBase_;
	WorldTransform worldTransformBody_;
	WorldTransform worldTransformHead_;
	WorldTransform worldTransformL_arm_;
	WorldTransform worldTransformR_arm_;
	//浮遊ギミックの媒介変数
	float floatingParameter_ = 0.0f;
	/// <summary>
	WorldTransform worldTransform_;
	/// </summary>
	Model* model_ = nullptr;
	//モデル
	Model* modelBody_ = nullptr;
	Model* modelHead_ = nullptr;
	Model* modelL_arm_ = nullptr;
	Model* modelR_arm_ = nullptr;
	uint32_t textureHandle_ = 0u;
	Input* input_ = nullptr;
	//カメラ
	const ViewProjection* viewProjection_ = nullptr;
	//ギミックの初期化
	void InitializeFloatingGimmick();
	//ギミック更新
	void UpdateFloatingGimmick();
};
