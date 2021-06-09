#include "Enemy.h"
#include "EnemyManager.h"
#include "../Player/Player.h"
#include "../../ExternalLibrary/ImGui/Include/imgui.h"


// 生成
std::shared_ptr<Enemy> Enemy::Create()
{
	return std::make_shared<Enemy>();
}

// コンストラクタ
void Enemy::Construct(Phoenix::Graphics::IGraphicsDevice* graphicsDevice)
{
	// モデル読み込み
	{
		model = std::make_unique<Phoenix::FrameWork::ModelObject>();
		model->Initialize(graphicsDevice);
		model->Load(graphicsDevice, "..\\Data\\Assets\\Model\\Enemy\\Enemy\\Idle\\Ready_Idle.fbx"); // "..\\Data\\Assets\\Model\\Enemy\\Idle\\Idle.fbx"  // "..\\Data\\Assets\\Model\\Boss\\Mutant\\Idle\\Mutant_Roaring.fbx"
		model->SetHipID("Hips");
	}

	// アニメーション読み込み
	Phoenix::s32 beginIndex, endIndex;
	{
		model->LoadAnimation("..\\Data\\Assets\\Model\\Enemy\\Enemy\\Walk\\Forward\\Walk_Forward.fbx", -1);
		model->LoadAnimation("..\\Data\\Assets\\Model\\Enemy\\Enemy\\Walk\\Back\\Walk_Backward.fbx", -1);
		model->LoadAnimation("..\\Data\\Assets\\Model\\Enemy\\Enemy\\Walk\\Right\\Walk_Right.fbx", -1);
		model->LoadAnimation("..\\Data\\Assets\\Model\\Enemy\\Enemy\\Walk\\Left\\Walk_Left.fbx", -1);

		model->LoadAnimation("..\\Data\\Assets\\Model\\Enemy\\Enemy\\Run\\Running.fbx", -1);

		model->LoadAnimation("..\\Data\\Assets\\Model\\Enemy\\Enemy\\Dedge\\Back_Step.fbx", -1);

		model->LoadAnimation("..\\Data\\Assets\\Model\\Enemy\\Enemy\\Damage\\Head_Hit.fbx", -1);
		model->LoadAnimation("..\\Data\\Assets\\Model\\Enemy\\Enemy\\Damage\\Head_Hit_Big.fbx", -1);
		model->LoadAnimation("..\\Data\\Assets\\Model\\Enemy\\Enemy\\Damage\\Flying_Back_Death.fbx", -1);

		model->LoadAnimation("..\\Data\\Assets\\Model\\Enemy\\Enemy\\Idle\\Getting_Up.fbx", -1);

		model->LoadAnimation("..\\Data\\Assets\\Model\\Enemy\\Enemy\\Death\\Dying_Backwards.fbx", -1);

		beginIndex = model->LoadAnimation("..\\Data\\Assets\\Model\\Enemy\\Enemy\\Attack\\Cross_Punch.fbx", -1);
					 model->LoadAnimation("..\\Data\\Assets\\Model\\Enemy\\Enemy\\Attack\\Punching.fbx", -1);
		endIndex   = model->LoadAnimation("..\\Data\\Assets\\Model\\Enemy\\Enemy\\Attack\\Hook_Punch.fbx", -1);

		Phoenix::s32 layerNum = 0;
		{
			layerNum = model->AddAnimationLayer();
			layerIndexList.insert(std::make_pair(LayerType::Base, layerNum));

			layerNum = model->AddAnimationLayer(56, 65);
			layerIndexList.insert(std::make_pair(LayerType::LowerBody, layerNum));
		}

		// ステート追加
		Phoenix::s32 stateNum = 0;
		{
			auto AddState = [&](StateType type, Phoenix::u32 animationIndex, Phoenix::u32 layerIndex)
			{
				stateNum = model->AddAnimationStateToLayer(animationIndex, layerIndex);
				stateIndexList.insert(std::make_pair(type, stateNum));
			};

			// ベースレイヤーにステート追加
			layerNum = layerIndexList.at(LayerType::Base);
			{
				for (Phoenix::s32 i = beginIndex; i <= endIndex; ++i)
				{
					model->AddAnimationStateToLayer(i, layerNum);
				}

				AddState(StateType::Idle, 0, layerNum);
				AddState(StateType::Run, 5, layerNum);
				AddState(StateType::DamageSmall, 7, layerNum);
				AddState(StateType::DamageBig, 8, layerNum);
				AddState(StateType::KnockBack, 9, layerNum);
				AddState(StateType::GettingUp, 10, layerNum);
				AddState(StateType::Dedge, 6, layerNum);
				AddState(StateType::Death, 11, layerNum);
			}

			// 下半身レイヤーにブレンドツリー追加
			layerNum = layerIndexList.at(LayerType::LowerBody);
			{
				Phoenix::s32 blendTreeIndex = model->AddBlendTreeToLayer(layerNum);
				model->AddBlendAnimationStateToBlendTree(0, Phoenix::Math::Vector3( 0.0f,  0.0f, 0.0f), layerNum, blendTreeIndex);
				model->AddBlendAnimationStateToBlendTree(1, Phoenix::Math::Vector3( 0.0f,  1.0f, 0.0f), layerNum, blendTreeIndex);
				model->AddBlendAnimationStateToBlendTree(2, Phoenix::Math::Vector3( 0.0f, -1.0f, 0.0f), layerNum, blendTreeIndex);
				model->AddBlendAnimationStateToBlendTree(3, Phoenix::Math::Vector3( 1.0f,  0.0f, 0.0f), layerNum, blendTreeIndex);
				model->AddBlendAnimationStateToBlendTree(4, Phoenix::Math::Vector3(-1.0f,  0.0f, 0.0f), layerNum, blendTreeIndex);
			}
		}
	}

	// コリジョン初期化
	{
		collisionDatas.resize(5);

		collisionDatas.at(0).pos = Phoenix::Math::Vector3(0.0f, 0.0f, 0.0f);
		collisionDatas.at(0).radius = 0.5f;
		collisionDatas.at(0).boneIndex = model->GetBoneIndex("Hips");

		collisionDatas.at(1).pos = Phoenix::Math::Vector3(0.0f, 0.0f, 0.0f);
		collisionDatas.at(1).radius = 0.25f;
		collisionDatas.at(1).boneIndex = model->GetBoneIndex("RightHandIndex1");

		collisionDatas.at(2).pos = Phoenix::Math::Vector3(0.0f, 0.0f, 0.0f);
		collisionDatas.at(2).radius = 0.25f;
		collisionDatas.at(2).boneIndex = model->GetBoneIndex("LeftHandIndex1");

		collisionDatas.at(3).pos = Phoenix::Math::Vector3(0.0f, 0.0f, 0.0f);
		collisionDatas.at(3).radius = 0.25f;
		collisionDatas.at(3).boneIndex = model->GetBoneIndex("RightFoot");

		collisionDatas.at(4).pos = Phoenix::Math::Vector3(0.0f, 0.0f, 0.0f);
		collisionDatas.at(4).radius = 0.25f;
		collisionDatas.at(4).boneIndex = model->GetBoneIndex("HeadTop_End");
	}

	// アタックデータ生成
	{
		auto SetAttackData = [&]
		(
			EnemyAttackState animState,
			Phoenix::s32 animIndex,

			Phoenix::f32 playSpeed,
			Phoenix::f32 playBeginTime,
			Phoenix::f32 playEndTime,

			Phoenix::s32 collisionNum,
			Phoenix::f32 collisionBeginTime,
			Phoenix::f32 collisionEndTime
		)
		{
			AttackData<EnemyAttackState> data;

			data.animState = animState;
			data.animIndex = animIndex;

			data.playSpeed = playSpeed;
			data.playBeginTime = playBeginTime == -1.0f ? -1.0f : playBeginTime / 60.0f;
			data.playEndTime = playEndTime == -1.0f ? -1.0f : playEndTime / 60.0f;

			data.collisionNum = collisionNum;
			data.collisionBeginTime = collisionBeginTime == -1.0f ? -1.0f : collisionBeginTime / 60.0f;
			data.collisionEndTime = collisionEndTime == -1.0f ? -1.0f : collisionEndTime / 60.0f;

			return data;
		};

		// 弱攻撃
		{
			// 右ストレート
			{
				AttackDatas<EnemyAttackState> datas;

				datas.AddData(SetAttackData(EnemyAttackState::WeakRight, 7, 1.5f, -1.0f, -1.0f, 1, 56.0f, 71.0f));

				attackDatasList.emplace_back(datas);
			}

			// 左ストレート
			{
				AttackDatas<EnemyAttackState> datas;

				datas.AddData(SetAttackData(EnemyAttackState::WeakLeft, 8, 1.5f, -1.0f, -1.0f, 2, 20.0f, 40.0f));

				attackDatasList.emplace_back(datas);
			}

			// 右フック
			{
				AttackDatas<EnemyAttackState> datas;

				datas.AddData(SetAttackData(EnemyAttackState::StrongRight, 9, 1.5f, -1.0f, -1.0f, 1, 60.0f, 72.0f));

				attackDatasList.emplace_back(datas);
			}
		}
	}

	// トランスフォームの初期化
	{
		transform = std::make_unique<Phoenix::FrameWork::Transform>();
	}

	// UI生成
	{
		ui = EnemyUI::Create();
	}

	// AIの初期化
	{
		currentMode = EnemyMode::Battle;

		// 通常モードAI
		//ordinaryAI = OrdinaryAIEnemyAI::Create();
		{
			//ordinaryAI->SetOwner(shared_from_this());
		}

		// バトルモードAI
		/*battleAI = BattleEnemyAI::Create();
		{
			std::weak_ptr<Enemy> owner = shared_from_this();
			attackState = BattleEnemy::Attack<EnemyAttackState, Enemy>::Create(owner);

			battleAI->SetOwner(owner);
			battleAI->SetUp();

			battleAI->AddState(BattleEnemy::Idle::Create());
			battleAI->AddState(BattleEnemy::Walk::Create(owner));
			battleAI->AddState(BattleEnemy::Run::Create(owner));
			battleAI->AddState(BattleEnemy::Dedge::Create(owner));
			battleAI->AddState(BattleEnemy::DamageSmall::Create(owner));
			battleAI->AddState(BattleEnemy::DamageBig::Create(owner));
			battleAI->AddState(BattleEnemy::KnockBack::Create(owner));
			battleAI->AddState(BattleEnemy::GettingUp::Create(owner));
			battleAI->AddState(BattleEnemy::Guard::Create());
			battleAI->AddState(BattleEnemy::Death::Create());
			battleAI->AddState(attackState);

			attackState->AddAttack(EnemyAttackState::WeakRight);
			attackState->AddAttack(EnemyAttackState::WeakLeft);
			attackState->AddAttack(EnemyAttackState::StrongRight);
		}*/

		// HTN用データ
		{
			htnDatas = std::make_shared<HTNDatas>();
			htnDatas->Initialize();
		}

		// HTN用ワールドステート
		{
			worldState = std::make_shared<EnemyWorldState>();
		}

		// AIコントローラー
		{
			controller = EnemyAIController::Create();
			controller->Construct(shared_from_this());
			controller->SetWorldState(worldState);
		}
	}

	// その他のパラメータ初期化
	{
		Initialize();
	}
}

// 初期化
void Enemy::Initialize()
{
	// 待機モーション開始
	{
		model->PlayAnimation(layerIndexList.at(LayerType::Base), stateIndexList.at(StateType::Idle), 1);
		model->UpdateTransform(1 / 60.0f);
		model->SetLoopAnimation(true);
	}

	// AI
	{
		//battleAI->GoToState(BattleEnemyState::Idle);
		htnDatas->Initialize();
		controller->Initialize();
	}

	// パラメーター
	{
		typeTag = TypeTag::Small;

		enable = false;
		alive = false;
		death = false;
		inBattle = false;
		changeOfCourse = false;
		detour = false;

		newRotate = Phoenix::Math::Quaternion::Zero;

		life = LifeRange;
		lifeMax = LifeRange;
		radius = 0.75f;

		attackReceptionTimeCnt = 0.0f;
		attackAnimationSpeed = 0.0f;

		distanceToPlayer = 0.0f;

		moveSpeed = 0.0f;

		moveX = 0.0f;
		moveY = 0.0f;

		currentAttackState = EnemyAttackState::NoneState;
	}
}

// 終了化
void Enemy::Finalize()
{
	//battleAI->CleanUp();
	//battleAI.reset();

	collisionDatas.clear();
	transform.reset();
	model.reset();
}

// 更新
void Enemy::Update(bool onControl, Phoenix::f32 elapsedTime)
{
	// HTN用データに経過時間を設定
	{
		htnDatas->SetElapsedTime(elapsedTime);
	}

	// ライフが０ならマネージャーの生存エネミーカウントを下げる
	if (life <= 0 && alive)
	{
		alive = false;
		if (std::shared_ptr<EnemyManager> manager = owner.lock())
		{
			manager->SubAliveEnemyCount(1);
		}

		SetAnimation(BattleEnemyState::Death);
		ChangeAnimation();
	}

	// 死亡アニメーション終了時に存在を消す
	if (!alive)
	{
		if (!model->IsPlaying())
		{
			death = true;
		}
		else
		{
			model->UpdateTransform(elapsedTime / 60.0f);
		}
	}

	if (!enable) return;
	if (!alive) return;
	if (death) return;
	if (!onControl)
	{
		if (!model->IsPlaying())
		{
			SetState(BattleEnemyState::Idle);
		}
		model->UpdateTransform(elapsedTime / 60.0f);

		DistanceMeasurement();
		UpdateRotate(elapsedTime);
		UpdateTranslate(elapsedTime);
		UpdateTransform();
		UpdateCollision();

		return;
	}

	// 遠回り
	{
		if (detour)
		{
			Phoenix::f32 dis = Phoenix::Math::Vector3Length(targetPos - transform->GetTranslate());

			if (dis <= 1.5f)
			{
				detour = false;
			}
		}
		else
		{
			targetPos = playerPos;
		}
	}

	// AI更新
	{
		UpdateAI(elapsedTime);
	}

	// 回転
	{
		UpdateRotate(elapsedTime);
	}

	// 移動
	{
		UpdateTranslate(elapsedTime);
	}

	// アニメーションの切り替え
	{
		ChangeAnimation();
	}

	// 攻撃アニメーションの切り替え
	{
		ChangeAttackAnimation();
	}

	// アニメーション更新
	{
		UpdateAnimation(elapsedTime);
	}

	// トランスフォーム更新
	{
		UpdateTransform();
	}

	// コリジョン更新
	{
		UpdateCollision();
	}

	// プレイヤーとの距離計測
	{
		DistanceMeasurement();
	}

	// アタック判定中
	{
		AttackJudgment();
	}
}

// トランスフォーム更新
void Enemy::UpdateTransform()
{
	transform->Update();
}

// 座標更新
void Enemy::UpdateTranslate(Phoenix::f32 elapsedTime)
{
	if (0.0f < moveSpeed) // 0以下の時に計算をしても意味が無いので分岐している。
	{
		Phoenix::f32 cameraY = 0.0f;
		{
			Phoenix::Math::Vector3 dir = transform->GetTranslate() - targetPos;
			dir.y = 0.0f;
			dir = Phoenix::Math::Vector3Normalize(dir);

			cameraY = atan2f(dir.x, dir.z);
		}

		Phoenix::f32 rotateY = 0.0f;
		{
			float len = sqrtf(moveX * moveX + moveY * moveY);

			if (len <= 0)
			{
				moveX = 0;
				moveY = 0;
			}

			float mag = 1 / len;

			moveX *= mag;
			moveY *= mag;

			rotateY = cameraY + atan2f(moveX, moveY);
		}

		Phoenix::Math::Vector3 pos = transform->GetTranslate();
		{
			pos.x += sinf(rotateY) * (moveSpeed * elapsedTime);
			pos.z += cosf(rotateY) * (moveSpeed * elapsedTime);
		}

		transform->SetTranslate(pos);
	}
}

// 回転更新
void Enemy::UpdateRotate(Phoenix::f32 elapsedTime)
{
	BattleEnemyState currentState = controller->GetState();
	if (currentState == BattleEnemyState::Idle || currentState == BattleEnemyState::Walk || currentState == BattleEnemyState::Run)
	{
		UpdateNewRotate();
	}

	Phoenix::Math::Quaternion rotate = transform->GetRotate();
	rotate = Phoenix::Math::QuaternionSlerp(rotate, newRotate, 0.17f * elapsedTime);

	transform->SetRotate(rotate);
}

// アニメーション更新
void Enemy::UpdateAnimation(Phoenix::f32 elapsedTime)
{
	if (controller->GetState() == BattleEnemyState::Walk)
	{
		model->SetBlendRate(Phoenix::Math::Vector3(-moveX, -moveY, 0.0f));
	}

	model->UpdateTransform(elapsedTime / 60.0f);
	attackReceptionTimeCnt += attackAnimationSpeed * elapsedTime / 60.0f;
}

// AI更新
void Enemy::UpdateAI(Phoenix::f32 elapsedTime)
{
	controller->Update(elapsedTime);
}

// 当たり判定更新
void Enemy::UpdateCollision()
{
	Phoenix::Math::Matrix systemUnitTransform = { 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1 };
	systemUnitTransform._11 = systemUnitTransform._22 = systemUnitTransform._33 = 0.01f;

	auto nodes = model->GetNodes();
	for (auto& data : collisionDatas)
	{
		Phoenix::Math::Matrix bone = nodes->at(data.boneIndex).worldTransform;
		bone *= systemUnitTransform * transform->GetWorldTransform();
		data.pos = Phoenix::Math::Vector3(bone._41, bone._42, bone._43);
	}
}

// UI更新
void Enemy::UpdateUI(Phoenix::Math::Vector2 pos, Phoenix::f32 elapsedTime)
{
	Phoenix::f32 hp = static_cast<Phoenix::f32>(life);
	hp = hp <= 0 ? 0 : hp;

	ui->Update((hp / lifeMax) * 100.0f, elapsedTime);
	ui->SetExit(alive);
	ui->SetPos(pos);
}

// 攻撃判定
void Enemy::AttackJudgment()
{
	if (controller->GetState() == BattleEnemyState::Attack)
	{
		auto Judgment = [&](Phoenix::s32 index)
		{
			if (isHit)
			{
				isAttackJudgment = false;
				return;
			}

			isAttackJudgment = true;
			attackCollisionIndex = index;
		};
		auto NoJudgment = [&]()
		{
			isAttackJudgment = false;
			isHit = false;
			attackCollisionIndex = -1;
		};

		//Phoenix::u32 index = static_cast<Phoenix::u32>(attackState);

		//// 当たり判定
		//if (attackDatasList.at(index).datas.at(attackComboState).collisionBeginTime <= attackReceptionTimeCnt && attackReceptionTimeCnt <= attackDatasList.at(index).datas.at(attackComboState).collisionEndTime)
		//{
		//	Judgment(attackDatasList.at(index).datas.at(attackComboState).collisionNum);
		//	attackPower = attackDatasList.at(index).receptionKey == AttackKey::WeakAttack ? 0 : 1;
		//}
		//else
		//{
		//	NoJudgment();
		//}

		bool hit = false;
		for (const auto& data : attackDatasList)
		{
			if (currentAttackState == data.datas.at(0).animState)
			{
				if (data.datas.at(0).collisionBeginTime <= attackReceptionTimeCnt && attackReceptionTimeCnt <= data.datas.at(0).collisionEndTime)
				{
					hit = true;
					Judgment(data.datas.at(0).collisionNum);
					if (currentAttackState == EnemyAttackState::WeakRight || currentAttackState == EnemyAttackState::WeakLeft)
					{
						attackPower = 0;
					}
					else if (currentAttackState == EnemyAttackState::StrongRight || currentAttackState == EnemyAttackState::StrongLeft)
					{
						attackPower = 1;
					}
				}
			}
		}
		if (!hit)
		{
			NoJudgment();
		}
	}
	else
	{
		isAttackJudgment = false;
	}
}

// プレイヤーとの距離計測
void Enemy::DistanceMeasurement()
{
	distanceToPlayer = Phoenix::Math::Vector3Length(playerPos - transform->GetTranslate());
}

// プレイヤーの攻撃視野にいるか判定
bool Enemy::JudgePlayerAttackRange()
{
	Phoenix::Math::Vector3 dir = GetPosition() - playerPos;
	dir = Phoenix::Math::Vector3Normalize(dir);
	dir.y = 0.0f;

	Phoenix::Math::Quaternion rotate = playerRotate;
	Phoenix::Math::Matrix m = Phoenix::Math::MatrixRotationQuaternion(&rotate);
	Phoenix::Math::Vector3 forward = Phoenix::Math::Vector3(m._31, m._32, m._33);
	forward.y = 0.0f;

	Phoenix::f32 angle;
	angle = acosf(Phoenix::Math::Vector3Dot(dir, forward));

	if (1e-8f < fabs(angle))
	{
		angle /= 0.01745f;

		if (angle <= 45.0f)
		{
			return true;
		}
	}

	return false;
}

// 新たな回転値の更新
void Enemy::UpdateNewRotate()
{
	Phoenix::Math::Vector3 dir = Phoenix::Math::Vector3Normalize(targetPos - GetPosition());
	float len = sqrtf(dir.x * dir.x + dir.z * dir.z);

	if (len <= 0)
	{
		dir.x = 0;
		dir.z = 0;
	}

	float mag = 1 / len;

	dir.x *= mag;
	dir.z *= mag;

	Phoenix::f32 angleY = atan2f(dir.x, dir.z);

	newRotate = Phoenix::Math::QuaternionRotationAxis(Phoenix::Math::Vector3(0.0f, 1.0f, 0.0f), angleY);
}

// 描画
void Enemy::Draw() {}

// GUI
void Enemy::GUI(Phoenix::s32 index)
{
	std::string name = std::string("Enemy") + std::to_string(index);
	if (ImGui::TreeNode(name.c_str()))
	{
		transform->GUI();
		ImGui::DragFloat3("targetPos", &targetPos.x);
		ImGui::TreePop();
	}
}

// 有効化設定
void Enemy::SetEnable(bool enable)
{
	this->enable = enable;
}

// 存在フラグ設定
void Enemy::SetAlive(bool alive)
{
	this->alive = alive;
}

// 死亡フラグ設定
void Enemy::SetDeath(bool death)
{
	this->death = death;
}

// 戦闘中フラグ設定
void Enemy::SetInBattle(bool inBattle)
{
	this->inBattle = inBattle;
	currentMode = inBattle ? EnemyMode::Battle : EnemyMode::Ordinary;
}

// トランスフォームの設定
void Enemy::SetTransform(Phoenix::FrameWork::Transform transform)
{
	this->transform->SetTranslate(transform.GetTranslate());
	this->transform->SetRotate(transform.GetRotate());
	this->transform->SetScale(transform.GetScale());
}

// トランスレートの設定
void Enemy::SetTranslate(Phoenix::Math::Vector3 translate)
{
	this->transform->SetTranslate(translate);
}

// 新たな回転値を設定
void Enemy::SetNewRotate(Phoenix::Math::Quaternion newRotate)
{
	this->newRotate = newRotate;
}

// エネミーマネージャーの設定
void Enemy::SetOwner(std::shared_ptr<EnemyManager> owner)
{
	this->owner = owner;
}

// ステートを変更
void Enemy::SetState(BattleEnemyState state, bool forcedChange)
{
	changeState = state;
	controller->SetNextState(state);
}

// アニメーションを変更
void Enemy::SetAnimation(BattleEnemyState state)
{
	if (controller->GetState() == BattleEnemyState::KnockBack && state != BattleEnemyState::KnockBack)
	{
		state = state;
	}

	changeAnimation = true;
	changeState = state;
}

// 攻撃権を発行
bool Enemy::SetAttackRight(bool stackAttackRight)
{
	if (controller->GetState() == BattleEnemyState::Idle)
	{
		SetState(BattleEnemyState::Attack, true);
		this->stackAttackRight = true;
		return true;
	}
	else if (stackAttackRight)
	{
		this->stackAttackRight = stackAttackRight;
	}

	return false;
}

// 攻撃ステートを変更
void Enemy::SetAttackState(EnemyAttackState state)
{
	changeAttackAnimation = true;
	changeAttackState = state;
	currentAttackState = state;

	for (const auto& data : attackDatasList)
	{
		if (state == data.datas.at(0).animState)
		{
			attackAnimationSpeed = data.datas.at(0).playSpeed;
		}
	}
	attackReceptionTimeCnt = 0.0f;
}

// プレイヤーパラメーターを設定
void Enemy::SetPlayerParameter(Phoenix::Math::Vector3 pos, Phoenix::Math::Quaternion rotate)
{
	playerPos = pos;
	playerRotate = rotate;
}

// 移動スピード設定
void Enemy::SetMoveSpeed(Phoenix::f32 speed)
{
	moveSpeed = speed;
}

// 移動方向の指定
void Enemy::SetMoveInput(Phoenix::f32 moveX, Phoenix::f32 moveY)
{
	this->moveX = moveX;
	this->moveY = moveY;
}

// ステージ端にいるか設定
void Enemy::SetHitWall(bool hitWall)
{
	this->hitWall = hitWall;
}

// 方向転換の設定
void Enemy::SetChangeOfCourse(bool changeOfCourse)
{
	this->changeOfCourse = changeOfCourse;
}

// 遠回りの設定
void Enemy::SetDetour(bool detour)
{
	this->detour = detour;
}

// 目的地の設定
void Enemy::SetTargetPos(Phoenix::Math::Vector3 targetPos)
{
	this->targetPos = targetPos;
}

// アニメーションを移行
void Enemy::ChangeAnimation()
{
	if (!changeAnimation) return;

	Phoenix::s32 baseLayerIndex = layerIndexList.at(LayerType::Base);
	Phoenix::s32 lowerBodyLayerIndex = layerIndexList.at(LayerType::LowerBody);

	switch (changeState)
	{
	case BattleEnemyState::Idle:
		model->PlayAnimation(baseLayerIndex, stateIndexList.at(StateType::Idle), 1, 0.75f);
		model->SetLoopAnimation(true);
		break;

	case BattleEnemyState::Walk:
		model->PlayAnimation(baseLayerIndex, stateIndexList.at(StateType::Idle), 1, 0.2f);
		model->SimultaneousPlayBlendTreeAniamation(lowerBodyLayerIndex, 0, 1, 0.2f);
		model->SetLoopAnimation(true);
		model->SetBlendLoopAnimation(true);
		break;

	case BattleEnemyState::Run:
		model->PlayAnimation(baseLayerIndex, stateIndexList.at(StateType::Run), 1, 0.2f);
		model->SetLoopAnimation(true);
		break;

	case BattleEnemyState::Attack: break;

	case BattleEnemyState::Dedge:
		model->PlayAnimation(baseLayerIndex, stateIndexList.at(StateType::Dedge), 0, 0.2f);
		model->SetLoopAnimation(false);
		model->SetSpeed(2.0f);
		break;

	case BattleEnemyState::DamageSmall:
		model->PlayAnimation(baseLayerIndex, stateIndexList.at(StateType::DamageSmall), 1, 0.2f);
		model->SetLoopAnimation(false);
		model->SetEndTime(43.0f / 60.0f);
		break;

	case BattleEnemyState::DamageBig:
		model->PlayAnimation(baseLayerIndex, stateIndexList.at(StateType::DamageBig), 1, 0.2f);
		model->SetLoopAnimation(false);
		break;

	case BattleEnemyState::KnockBack:
		model->PlayAnimation(baseLayerIndex, stateIndexList.at(StateType::KnockBack), 0, 0.2f);
		model->SetLoopAnimation(false);
		model->SetBeginTime(20.0f / 60.0f);
		model->SetCurrentTime(20.0f / 60.0f);
		break;

	case BattleEnemyState::GettingUp:
		model->PlayAnimation(baseLayerIndex, stateIndexList.at(StateType::GettingUp), 1, 7.5f);
		model->SetLoopAnimation(false);
		model->SetEndTime(350.0f / 60.0f);
		model->SetSpeed(2.0f);
		break;

	case BattleEnemyState::Guard:
		model->PlayAnimation(baseLayerIndex, 0, 1, 0.2f);
		model->SetLoopAnimation(false);
		break;

	case BattleEnemyState::Death:
		model->PlayAnimation(baseLayerIndex, stateIndexList.at(StateType::Death), 1, 0.2f);
		model->SetLoopAnimation(false);
		model->SetBeginTime(55.0f / 60.0f);
		model->SetEndTime(195.0f / 60.0f);
		model->SetCurrentTime(42.0f / 60.0f);
		break;

	case BattleEnemyState::NoneState: break;

	default: break;
	}

	changeAnimation = false;
	changeState = BattleEnemyState::NoneState;
}

// 攻撃ステートを移行
void Enemy::ChangeAttackAnimation()
{
	if (!changeAttackAnimation) return;

	Phoenix::s32 baseLayerIndex = layerIndexList.at(LayerType::Base);

	switch (changeAttackState)
	{
	case EnemyAttackState::WeakRight:
		model->PlayAnimation(baseLayerIndex, 0, 1, 0.2f);
		model->SetLoopAnimation(false);
		model->SetSpeed(attackAnimationSpeed);

		break;

	case EnemyAttackState::WeakLeft:
		model->PlayAnimation(baseLayerIndex, 1, 1, 0.2f);
		model->SetLoopAnimation(false);
		model->SetSpeed(attackAnimationSpeed);

		break;

	case EnemyAttackState::StrongRight:
		model->PlayAnimation(baseLayerIndex, 2, 1, 0.2f);
		model->SetLoopAnimation(false);
		model->SetSpeed(attackAnimationSpeed);

		break;

	case EnemyAttackState::StrongLeft:
		model->PlayAnimation(baseLayerIndex, 0, 1, 0.2f);
		model->SetLoopAnimation(false);
		model->SetSpeed(attackAnimationSpeed);

		break;

	case EnemyAttackState::NoneState: break;

	default: break;
	}

	changeAttackAnimation = false;
	changeAttackState = EnemyAttackState::NoneState;
}

// ダメージ
bool Enemy::Damage(Phoenix::s32 damage)
{
	if (controller->GetState() == BattleEnemyState::Dedge) return false;

	life -= damage;
	accumulationDamage += damage;

	if (damage <= 10)
	{
		SetMoveInput(0.0f, 0.0f);
		SetMoveSpeed(0.0f);
		SetState(BattleEnemyState::DamageSmall, true);
	}
	else if (damage <= 20)
	{
		// 蓄積ダメージの確認
		if (!AccumulationDamage(damage))
		{
			SetMoveInput(0.0f, 0.0f);
			SetMoveSpeed(0.0f);
			SetState(BattleEnemyState::DamageBig, true);
		}
	}

	// ライフが０ならマネージャーの生存エネミーカウントを下げる
	if (life <= 0 && alive)
	{
		alive = false;
		if (std::shared_ptr<EnemyManager> manager = owner.lock())
		{
			manager->SubAliveEnemyCount(1);
		}

		SetAnimation(BattleEnemyState::Death);
		ChangeAnimation();
	}

	return true;
}

// 蓄積ダメージ
bool Enemy::AccumulationDamage(Phoenix::s32 damage)
{
	if (AccumulationMaxDamage <= accumulationDamage)
	{
		accumulationDamage = 0;

		SetMoveInput(0.0f, 0.0f);
		SetMoveSpeed(0.0f);
		SetState(BattleEnemyState::KnockBack, true);

		return true;
	}

	return false;
}

// 有効フラグ取得
bool Enemy::GetEnable()
{
	return enable;
}

// 存在フラグ取得
bool Enemy::GetAlive()
{
	return alive;
}

// 死亡フラグ取得
bool Enemy::GetDeath()
{
	return death;
}

// 戦闘中フラグ取得
bool Enemy::GetInBattle()
{
	return inBattle;
}

// ステージ端にいるか取得
bool Enemy::GetHitWall()
{
	return hitWall;
}

// 方向転換の取得
bool Enemy::GetChangeOfCourse()
{
	return changeOfCourse;
}

// 遠回りの取得
bool Enemy::GetDetour()
{
	return detour;
}

// トランスフォームの取得
Phoenix::FrameWork::Transform Enemy::GetTransform()
{
	return *transform.get();
}

// バトルモードのステート取得
BattleEnemyState Enemy::GetBattleState()
{
	return controller->GetState();
}

// プレイヤーの戦闘エリアに侵入したか
bool Enemy::InBattleTerritory()
{
	if (distanceToPlayer <= 5.0f)
	{
		return true;
	}

	return false;
}

// プレイヤーに攻撃が当たる距離に入っているか？
bool Enemy::InDistanceHitByAttack()
{
	if (distanceToPlayer <= 1.5f)
	{
		return true;
	}

	return false;
}