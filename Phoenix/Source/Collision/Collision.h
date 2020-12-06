#pragma once

#include "Phoenix/Math/PhoenixMath.h"


/// <summary>
/// ���� vs ����
/// </summary>
/// <param name="pos1"> ���W�P </param>
/// <param name="pos2"> ���W�Q </param>
/// <param name="r1"> ���a�P </param>
/// <param name="r2"> ���a�Q </param>
/// <returns> �������Ă��邩�H </returns>
static bool SphereVsSphere(Phoenix::Math::Vector3 pos1, Phoenix::Math::Vector3 pos2, float r1, float r2)
{
	if ((pos2.x - pos1.x) * (pos2.x - pos1.x) + (pos2.y - pos1.y) * (pos2.y - pos1.y) + (pos2.z - pos1.z) * (pos2.z - pos1.z) <= (r1 + r2) * (r1 + r2))
	{
		return true;
	}
	else return false;
}

/// <summary>
/// �~ vs �~
/// </summary>
/// <param name="pos1"> ���W�P </param>
/// <param name="pos2"> ���W�Q </param>
/// <param name="r1"> ���a�P </param>
/// <param name="r2"> ���a�Q </param>
/// <returns> �������Ă��邩�H </returns>
static bool CircleVsCircle(Phoenix::Math::Vector2 pos1, Phoenix::Math::Vector2 pos2, float r1, float r2)
{
	if ((pos2.x - pos1.x) * (pos2.x - pos1.x) + (pos2.y - pos1.y) * (pos2.y - pos1.y) <= (r1 + r2) * (r1 + r2))
	{
		return true;
	}
	else return false;
}

/// <summary>
/// ���̒��S�ƃJ�v�Z���̐����̋����i�̓��j���v�Z
/// </summary>
/// <param name="point"> ���̂̍��W </param>
/// <param name="seg_start"> �J�v�Z���̍��W�P </param>
/// <param name="seg_end"> �J�v�Z���̍��W�Q </param>
/// <returns> ���� </returns>
static float GetSqDistancePoint2Segment(Phoenix::Math::Vector3 point, Phoenix::Math::Vector3 segStart, Phoenix::Math::Vector3 segEnd)
{
	const float eqsilon = 1.0e-5f;	// �덷�z���p�̔����Ȓl

	// �����̎n�_����I�_�ւ̃x�N�g��
	Phoenix::Math::Vector3 segment_sub = Phoenix::Math::Vector3(segEnd.x - segStart.x, segEnd.y - segStart.y, segEnd.z - segStart.z);

	// �����̎n�_����_�ւ̃x�N�g��
	Phoenix::Math::Vector3 segment_point = Phoenix::Math::Vector3(point.x - segStart.x, point.y - segStart.y, point.z - segStart.z);

	// �ˉe�x�N�g��
	Phoenix::Math::Vector3 cross_point;

	// 2�x�N�g���̓���
	float dot = (segment_sub.x * segment_point.x) + (segment_sub.y * segment_point.y) + (segment_sub.z * segment_point.z);
	if (dot < eqsilon)
	{// ���ς����Ȃ�A�����̎n�_���ŋߖT
		return ((segment_point.x * segment_point.x) + (segment_point.y * segment_point.y) + (segment_point.z * segment_point.z));
	}

	// �_��������̏I�_�ւ̃x�N�g��
	segment_point = Phoenix::Math::Vector3(segEnd.x - point.x, segEnd.y - point.y, segEnd.z - point.z);

	// 2�x�N�g���̓���
	dot = (segment_sub.x * segment_point.x) + (segment_sub.y * segment_point.y) + (segment_sub.z * segment_point.z);
	if (dot < eqsilon)
	{// ���ς����Ȃ�A�����̏I�_���ŋߖT
		return ((segment_point.x * segment_point.x) + (segment_point.y * segment_point.y) + (segment_point.z * segment_point.z));
	}

	// ��L�̂ǂ���ɂ��Y�����Ȃ��ꍇ�A������ɗ��Ƃ����ˉe���ŋߖT
	// (�{���Ȃ�T�C���ŋ��߂邪�A�O�ς̑傫��/�����̃x�N�g���̑傫���ŋ��܂�)
	cross_point.x = segment_sub.y * segment_point.z - segment_sub.z * segment_point.y;
	cross_point.y = segment_sub.z * segment_point.x - segment_sub.x * segment_point.z;
	cross_point.z = segment_sub.x * segment_point.y - segment_sub.y * segment_point.x;
	return (((cross_point.x * cross_point.x) + (cross_point.y * cross_point.y) + (cross_point.z * cross_point.z))
		/ ((segment_sub.x * segment_sub.x) + (segment_sub.y * segment_sub.y) + (segment_sub.z * segment_sub.z)));
}

/// <summary>
/// ���� vs �J�v�Z��
/// </summary>
/// <param name="sphere"> ���̂̍��W </param>
/// <param name="capsule1"> �J�v�Z���̍��W�P </param>
/// <param name="capsule2"> �J�v�Z���̍��W�Q </param>
/// <param name="sphereRadius"> ���̂̔��a </param>
/// <param name="capsuleRadius"> �J�v�Z���̔��a </param>
/// <returns> �������Ă��邩�H </returns>
static bool SphereVsCapsule(Phoenix::Math::Vector3 sphere, Phoenix::Math::Vector3 capsule1, Phoenix::Math::Vector3 capsule2, float sphereRadius, float capsuleRadius)
{
	// ���̒��S�ƃJ�v�Z���̐����̋����i�̓��j���v�Z
	float dis = GetSqDistancePoint2Segment(sphere, capsule1, capsule2);

	// �����i�̓��j�����a�̘a�i�̓��j��菬������Γ������Ă���
	float radius = sphereRadius + capsuleRadius;

	if (radius * radius < dis) return false;

	return true;
}