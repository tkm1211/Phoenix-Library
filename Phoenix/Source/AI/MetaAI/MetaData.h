#pragma once

#include "Phoenix/Math/PhoenixMath.h"


namespace Meta
{
	// ゲームシーンから必要最低限のデータ
	struct MetaData
	{
		Phoenix::s32 playerScore;
		Phoenix::Math::Vector3 playerPos;
		Phoenix::s32 playerState = -1;
		Phoenix::s32 playerAttackState = -1;
		bool playerIsInvincible = false;

		std::vector<Phoenix::Math::Vector3> enemiesPos;
		std::vector<Phoenix::s32> enemiesState;
		std::vector<Phoenix::s32> enemiesType;
	};
}