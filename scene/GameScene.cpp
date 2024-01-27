#include "GameScene.h"
#include "AxisIndicator.h"
#include"MathUtilityForText.h"
#include "TextureManager.h"
#include <cassert>
#include "ImGuiManager.h"

// #include"Player.h"

GameScene::GameScene() {}

GameScene::~GameScene() {}

void GameScene::Initialize() {

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();

	viewProjection_.farZ = 2000.0f;
	viewProjection_.translation_ = {0.0f, 2.0f, -10.0f};
	viewProjection_.Initialize();

	textureHandle_ = TextureManager::Load("mario.jpg");
	// タイトル
	textureHandleTitle_ = TextureManager::Load("title.png");
	spriteTitle_ = Sprite::Create(textureHandleTitle_, {0, 0});

	// タイトル文字
	textureHandleEnter_ = TextureManager::Load("enter.png");
	spriteEnter_ = Sprite::Create(textureHandleEnter_, {400, 500});

	// 説明
	textureHandleEnter_ = TextureManager::Load("setumei.png");
	spritesetumei_ = Sprite::Create(textureHandleEnter_, {0, 0});

	// ゲームオーバー
	textureHandlegameover_ = TextureManager::Load("gameover.png");
	spritegameover_ = Sprite::Create(textureHandlegameover_, {0, 0});

	// ゲームクリア
	textureHandlegameclear_ = TextureManager::Load("gameclear.png");
	spritegameclear_ = Sprite::Create(textureHandlegameclear_, {0, 0});

	textureHandleBlack_ = TextureManager::Load("Black.png");
	spriteBlack_ = Sprite::Create(textureHandleBlack_, {0, 0});


	modelSkydome_.reset(Model::CreateFromOBJ("skydome", true));
	modelGround_.reset(Model::CreateFromOBJ("ground", true));
	//modelFighter_.reset(Model::CreateFromOBJ("float", true));
	modelFighterBody_.reset(Model::CreateFromOBJ("float_Body", true));
	modelFighterHead_.reset(Model::CreateFromOBJ("float_Head", true));
	modelFighterL_arm_.reset(Model::CreateFromOBJ("float_L_arm", true));
	modelFighterR_arm_.reset(Model::CreateFromOBJ("float_R_arm", true));
	modelEnemyBody_.reset(Model::CreateFromOBJ("needle_Body", true));
	modelEnemyL_arm_.reset(Model::CreateFromOBJ("needle_L_arm", true));
	modelEnemyR_arm_.reset(Model::CreateFromOBJ("needle_R_arm", true));
	
	
	
	//クラスの生成
	player_ = std::make_unique<Player>();
	ground_ = std::make_unique<Ground>();
	skydome_ = std::make_unique<Skydome>();
	debugCamera_ = std::make_unique<DebugCamera>(WinApp::kWindowWidth, WinApp::kWindowHeight);
	followCamera_ = std::make_unique<FollowCamera>();


	std::vector<Model*> playerModels{
	    modelFighterBody_.get(), modelFighterHead_.get(), modelFighterL_arm_.get(),
	    modelFighterR_arm_.get()};

	ground_->Initialize(modelGround_.get());
	skydome_->Initialize(modelSkydome_.get());
	debugCamera_->SetFarZ(2000.0f);
	followCamera_->Initialize();


	player_->Initialize(playerModels);
	player_->SetViewProjection(&followCamera_->GetViewProjection());
	followCamera_->SetTarget(&player_->GetWorldTransform());
	// 軸方向表示の表示を有効にする
	AxisIndicator::GetInstance()->SetVisible(true);
	// 軸方向表示が参照するビュープロジェクションを指定する
	AxisIndicator::GetInstance()->SetTargetViewProjection(&debugCamera_->GetViewProjection());

	std::vector<Model*> enemyModels = {
	    modelEnemyBody_.get(),
	    modelEnemyL_arm_.get(),
	    modelEnemyR_arm_.get(),
	};
	enemy_ = std::make_unique<Enemy>();
	enemy_->Initialize(enemyModels);
	enemy_->SetLocalPosition(Vector3(10,0,20));
	enemy_->SetLocalRotation(Vector3(0, PI, 0));

}



void GameScene::Update() 
{
	// 各シーンの更新処理
	switch (sceneMode_) 
	{
	case 0:
		
		TitleUpdate();
		gameTimer_++;
		break;
	case 1:
		timer--;
		// 衝突判定
		dx = abs(player_->GetX() - enemy_->GetX());
		dz = abs(player_->GetZ() - enemy_->GetZ());
		if (dx < 2 && dz < 2) {
			// 衝突
			ImGui::Begin("Hit");
			ImGui::End();
			playerLife--;
		}

	// デバッグカメラの更新
		if (input_->TriggerKey(DIK_0)) {
			// フラグをトグル
			isDebugCameraActive_ = !isDebugCameraActive_;
		}
		if (isDebugCameraActive_ == true) {
			debugCamera_->Update();
			viewProjection_.matView = debugCamera_->GetViewProjection().matView;
			viewProjection_.matProjection = debugCamera_->GetViewProjection().matProjection;
		} else {
			followCamera_->Update();
			viewProjection_.matView = followCamera_->GetViewProjection().matView;
			viewProjection_.matProjection = followCamera_->GetViewProjection().matProjection;
			// ビュープロジェクションの転送
			viewProjection_.TransferMatrix();

			player_->Update();
			enemy_->Update();
		}
		if (playerLife <= 0)
		{
			sceneMode_ = 2;
		}
		if (timer <= 0)
		{
			sceneMode_ = 3;
		}
		if (sceneMode_ == 3)
		{
			timer = 500;
			if (input_->TriggerKey(DIK_RETURN)) {
				sceneMode_ = 0;
			}
		}
		// timer--;
		gameTimer_++;
		break;
	case 2:
		GameClearUpdate();
		gameTimer_++;
		break;
	case 3:
		GameOverUpdate();
		gameTimer_++;
		break;
	case 4:
		SetumeiUpdate();
			
		break;
	
	}
}

void GameScene::TitleUpdate() {
	
	/*if (input_->TriggerKey(DIK_RETURN)) {
		sceneMode_ = 4;
	}*/
	if (input_->TriggerKey(DIK_RETURN)) {
		flag_ = true;
		
		black_ = 0;
	}
	if (flag_ == true && black_ >= 1.0f) {
		flag_ = false;
		
		black_ = 1.0f;
		sceneMode_ = 4;
		GamePlayStart();
	}
}



void GameScene::TitleDraw2DNear() {
	spriteTitle_->Draw();
	if (gameTimer_ % 40 >= 20) {
		spriteEnter_->Draw();
	}
	
}
void GameScene::SetumeiUpdate() {
	//if (input_->TriggerKey(DIK_RETURN)) {
	//	sceneMode_ = 1;
	//	GamePlayStart();
	//	// //BGM切り替え
	//	// audio_->StopWave(voiceHandleBGM_);
	//	// voiceHandleBGM_ = audio_->PlayWave(soundDataHandleGamePlayBGM_, true);
	//}
	if (input_->TriggerKey(DIK_RETURN)) {
		flag_ = true;

		black_ = 0;
	}
	if (flag_ == true && black_ >= 1.0f) {
		flag_ = false;

		black_ = 1.0f;
		sceneMode_ = 1;
		GamePlayStart();
	}
}

// 説明表示
void GameScene::SetumeiDraw2DNear() { spritesetumei_->Draw(); }

void GameScene::GameOverUpdate() {

//if (sceneMode_ == 3) {
//		if (input_->TriggerKey(DIK_RETURN)) {
//			sceneMode_ = 0;
//		}
//	}
	if (input_->TriggerKey(DIK_RETURN)) {
		flag_ = true;

		black_ = 0;
	}
	if (flag_ == true && black_ >= 1.0f) {
		flag_ = false;

		black_ = 1.0f;

		sceneMode_ = 0;
		//GamePlayStart();
	}
}

// ゲームオーバー
void GameScene::GameOverDraw2DNear() {
	
	spritegameover_->Draw();
	if (gameTimer_ % 40 >= 20) {
		spriteEnter_->Draw();
	}
	
}
void GameScene::GameClearUpdate() {
	if (input_->TriggerKey(DIK_RETURN)) {
		flag_ = true;

		black_ = 0;
	}
	if (flag_ == true && black_ >= 1.0f) {
		flag_ = false;

		black_ = 1.0f;

		sceneMode_ = 0;
		// GamePlayStart();
	}
	/*if (sceneMode_ == 2) {
		if (input_->TriggerKey(DIK_RETURN)) {
			sceneMode_ = 0;
		}
	}*/

	
}

void GameScene::GameClearDraw2DNear()
{
	spritegameclear_->Draw();
	if (gameTimer_ % 40 >= 20) {
		spriteEnter_->Draw();
	}
	
}
void GameScene::GamePlayStart() {
	playerLife =45;
	timer = 500;
}
void GameScene::Draw() {

	// コマンドリストの取得
	ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();

#pragma region 背景スプライト描画
	// 背景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに背景スプライトの描画処理を追加できる
	/// </summary>

	// スプライト描画後処理
	Sprite::PostDraw();
	// 深度バッファクリア
	dxCommon_->ClearDepthBuffer();
#pragma endregion

#pragma region 3Dオブジェクト描画
	// 3Dオブジェクト描画前処理
	Model::PreDraw(commandList);

	/// <summary>
	/// ここに3Dオブジェクトの描画処理を追加できる
	/// </summary>
	player_->Draw(viewProjection_);
	ground_->Draw(viewProjection_);
	skydome_->Draw(viewProjection_);
	enemy_->Draw(viewProjection_);
	// 3Dオブジェクト描画後処理
	Model::PostDraw();
#pragma endregion

#pragma region 前景スプライト描画
	// 前景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに前景スプライトの描画処理を追加できる
	/// </summary>
	switch (sceneMode_) {
	case 0:
		TitleDraw2DNear();
		//debugText_->Print("Title", 10, 10, 2);
		break;
	case 1:
		//debugText_->Printf("Stage1", 10, 10, 2);
		break;
	case 2:
		//debugText_->Print("GameClear", 10, 10, 2);
		GameClearDraw2DNear();
		break;
	case 3:
		GameOverDraw2DNear();
		//debugText_->Print("GameOver", 10, 10, 2);
		break;
	case 4:
		SetumeiDraw2DNear();
		break;
	}
	if (flag_ == false) {
		black_ -= 0.01f;
	} else {
		black_ += 0.01f;
	}

	spriteBlack_->SetColor({1, 1, 1, black_});
	spriteBlack_->Draw();
	//debugText_->DrawAll();
	// player_->Draw(viewProjection_);
	//  スプライト描画後処理
	Sprite::PostDraw();

#pragma endregion
}
