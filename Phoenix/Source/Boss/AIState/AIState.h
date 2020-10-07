#pragma once


enum class AIStateType
{
	Wait,			// 待機
	Move,			// 移動
	Avoid,			// 回避
	SwingAttack01,	// 右殴り攻撃（近距離攻撃）
	SwingAttack02,	// 左殴り攻撃（近距離攻撃）
	RotationAttack,	// 回転攻撃（近距離攻撃）
	JumpAttack,		// ジャンプ攻撃（遠距離攻撃）
	Damage,			// ひるみ
	None			// 何でもない
};

class Boss;
class Player;
class AIState
{
protected:
	AIStateType stateType = AIStateType::None; // 各継承クラスで適切なタイプを設定してください。
	AIStateType nextStateType = AIStateType::None;
	bool isChangeState = false;

public:
	AIState() {}
	virtual ~AIState() {}

public:
	virtual void Init() = 0;
	virtual void Update(Boss* boss, Player* player) = 0;

public:
	AIStateType GetStateType() { return stateType; }
	AIStateType GetNextStateType() { return nextStateType; }
	bool IsChangeState()
	{
		bool isChange = isChangeState;
		isChangeState = false;

		return isChange;
	}
};