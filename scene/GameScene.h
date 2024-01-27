#pragma once

#include "Audio.h"
#include "AxisIndicator.h"
#include "DebugCamera.h"
#include "DirectXCommon.h"
#include "FollowCamera.h"
#include "Ground.h"
#include "Input.h"
#include "Model.h"
#include "Player.h"
#include"Enemy.h"
#include "Skydome.h"
#include "Sprite.h"
#include "ViewProjection.h"
#include "WorldTransform.h"

#include "DebugText.h"

#include <memory>
/// <summary>
/// ゲームシーン
/// </summary>
class GameScene {

public: // メンバ関数
	/// <summary>
	/// コンストクラタ
	/// </summary>
	GameScene();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~GameScene();

	/// <summary>
	/// 初期化
	/// </summary>
	// タイトル(スプライト)
	uint32_t textureHandleTitle_ = 0;
	Sprite* spriteTitle_ = nullptr;

	// タイトル文字
	uint32_t textureHandleEnter_ = 0;
	Sprite* spriteEnter_ = nullptr;

	uint32_t textureHandleBlack_ = 0;
	Sprite* spriteBlack_ = nullptr;

	void Initialize();

	float dx;
	float dz;

	int timer = 500;
	int playerLife = 45;

	int gameTimer_ = 0;

	bool flag_ = false;

	float black_ = 1.0f;
	/// <summary>
	/// 毎フレーム処理
	/// </summary>
	void Update();

	DebugText* debugText_ = nullptr;

	void TitleUpdate();
	void GameClearUpdate(); // ゲームクリア更新
	void GameOverUpdate();  // ゲームオーバー更新
	void TitleDraw2DNear();

	void SetumeiUpdate();
	void SetumeiDraw2DNear();
	void GameOverDraw2DNear();
	void GameClearDraw2DNear();
	void GamePlayStart();
	int sceneMode_ = 0;
	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

private: // メンバ変数
	DirectXCommon* dxCommon_ = nullptr;
	Input* input_ = nullptr;
	Audio* audio_ = nullptr;

	/// <summary>
	/// ゲームシーン用
	/// </summary>
	ViewProjection viewProjection_;
	uint32_t textureHandle_ = 0u;

	// ゲームオーバー
	uint32_t textureHandlegameover_ = 0;
	Sprite* spritegameover_ = nullptr;

	// ゲームクリア
	uint32_t textureHandlegameclear_ = 0;
	Sprite* spritegameclear_ = nullptr;

	// 説明
	uint32_t textureHandlesetumei_ = 0;
	Sprite* spritesetumei_ = nullptr;
	//3Dモデル
	std::unique_ptr<Model> modelCube_;
	//std::unique_ptr<Model> modelFighter_;
	std::unique_ptr<Model> modelSkydome_;
	std::unique_ptr<Model> modelGround_;
	std::unique_ptr<Model> modelFighterBody_;
	std::unique_ptr<Model> modelFighterHead_;
	std::unique_ptr<Model> modelFighterL_arm_;
	std::unique_ptr<Model> modelFighterR_arm_;
	std::unique_ptr<Model> modelEnemyBody_;
	std::unique_ptr<Model> modelEnemyL_arm_;
	std::unique_ptr<Model> modelEnemyR_arm_;



	std::unique_ptr<Player> player_;
	std::unique_ptr<Enemy> enemy_;
	std::unique_ptr<Ground> ground_;
	std::unique_ptr<Skydome> skydome_;

	// デバッグカメラ
	std::unique_ptr<DebugCamera> debugCamera_;
	bool isDebugCameraActive_ = false;

	// 追従カメラ
	std::unique_ptr<FollowCamera> followCamera_;
};
