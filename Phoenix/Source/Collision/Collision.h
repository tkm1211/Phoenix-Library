#pragma once

#include "Phoenix/Math/PhoenixMath.h"


/// <summary>
/// 球体 vs 球体
/// </summary>
/// <param name="pos1"> 座標１ </param>
/// <param name="pos2"> 座標２ </param>
/// <param name="r1"> 半径１ </param>
/// <param name="r2"> 半径２ </param>
/// <returns> 当たっているか？ </returns>
static bool SphereVsSphere(Phoenix::Math::Vector3 pos1, Phoenix::Math::Vector3 pos2, float r1, float r2)
{
	if ((pos2.x - pos1.x) * (pos2.x - pos1.x) + (pos2.y - pos1.y) * (pos2.y - pos1.y) + (pos2.z - pos1.z) * (pos2.z - pos1.z) <= (r1 + r2) * (r1 + r2))
	{
		return true;
	}
	else return false;
}

/// <summary>
/// 円 vs 円
/// </summary>
/// <param name="pos1"> 座標１ </param>
/// <param name="pos2"> 座標２ </param>
/// <param name="r1"> 半径１ </param>
/// <param name="r2"> 半径２ </param>
/// <returns> 当たっているか？ </returns>
static bool CircleVsCircle(Phoenix::Math::Vector2 pos1, Phoenix::Math::Vector2 pos2, float r1, float r2)
{
	if ((pos2.x - pos1.x) * (pos2.x - pos1.x) + (pos2.y - pos1.y) * (pos2.y - pos1.y) <= (r1 + r2) * (r1 + r2))
	{
		return true;
	}
	else return false;
}

/// <summary>
/// 球の中心とカプセルの線分の距離（の二乗）を計算
/// </summary>
/// <param name="point"> 球体の座標 </param>
/// <param name="seg_start"> カプセルの座標１ </param>
/// <param name="seg_end"> カプセルの座標２ </param>
/// <returns> 距離 </returns>
static float GetSqDistancePoint2Segment(Phoenix::Math::Vector3 point, Phoenix::Math::Vector3 segStart, Phoenix::Math::Vector3 segEnd)
{
	const float eqsilon = 1.0e-5f;	// 誤差吸収用の微小な値

	// 線分の始点から終点へのベクトル
	Phoenix::Math::Vector3 segment_sub = Phoenix::Math::Vector3(segEnd.x - segStart.x, segEnd.y - segStart.y, segEnd.z - segStart.z);

	// 線分の始点から点へのベクトル
	Phoenix::Math::Vector3 segment_point = Phoenix::Math::Vector3(point.x - segStart.x, point.y - segStart.y, point.z - segStart.z);

	// 射影ベクトル
	Phoenix::Math::Vector3 cross_point;

	// 2ベクトルの内積
	float dot = (segment_sub.x * segment_point.x) + (segment_sub.y * segment_point.y) + (segment_sub.z * segment_point.z);
	if (dot < eqsilon)
	{// 内積が負なら、線分の始点が最近傍
		return ((segment_point.x * segment_point.x) + (segment_point.y * segment_point.y) + (segment_point.z * segment_point.z));
	}

	// 点から線分の終点へのベクトル
	segment_point = Phoenix::Math::Vector3(segEnd.x - point.x, segEnd.y - point.y, segEnd.z - point.z);

	// 2ベクトルの内積
	dot = (segment_sub.x * segment_point.x) + (segment_sub.y * segment_point.y) + (segment_sub.z * segment_point.z);
	if (dot < eqsilon)
	{// 内積が負なら、線分の終点が最近傍
		return ((segment_point.x * segment_point.x) + (segment_point.y * segment_point.y) + (segment_point.z * segment_point.z));
	}

	// 上記のどちらにも該当しない場合、線分上に落とした射影が最近傍
	// (本来ならサインで求めるが、外積の大きさ/線分のベクトルの大きさで求まる)
	cross_point.x = segment_sub.y * segment_point.z - segment_sub.z * segment_point.y;
	cross_point.y = segment_sub.z * segment_point.x - segment_sub.x * segment_point.z;
	cross_point.z = segment_sub.x * segment_point.y - segment_sub.y * segment_point.x;
	return (((cross_point.x * cross_point.x) + (cross_point.y * cross_point.y) + (cross_point.z * cross_point.z))
		/ ((segment_sub.x * segment_sub.x) + (segment_sub.y * segment_sub.y) + (segment_sub.z * segment_sub.z)));
}

/// <summary>
/// 球体 vs カプセル
/// </summary>
/// <param name="sphere"> 球体の座標 </param>
/// <param name="capsule1"> カプセルの座標１ </param>
/// <param name="capsule2"> カプセルの座標２ </param>
/// <param name="sphereRadius"> 球体の半径 </param>
/// <param name="capsuleRadius"> カプセルの半径 </param>
/// <returns> 当たっているか？ </returns>
static bool SphereVsCapsule(Phoenix::Math::Vector3 sphere, Phoenix::Math::Vector3 capsule1, Phoenix::Math::Vector3 capsule2, float sphereRadius, float capsuleRadius)
{
	// 球の中心とカプセルの線分の距離（の二乗）を計算
	float dis = GetSqDistancePoint2Segment(sphere, capsule1, capsule2);

	// 距離（の二乗）が半径の和（の二乗）より小さければ当たっている
	float radius = sphereRadius + capsuleRadius;

	if (radius * radius < dis) return false;

	return true;
}