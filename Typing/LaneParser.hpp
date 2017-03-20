#pragma once


#include <iterator>


namespace LaneParser
{

	using R = std::map<double, Vec3>;

	using R2 = std::map<int, Vec3>;

	Array<R> Parse(const JSONArray &lanes)
	{

		return Map<R>(lanes, [](JSONValue _jsonValue) {

			auto jsonValue = _jsonValue[L"points"];

			R mmp;


			auto obj = jsonValue.getObject();


			for (auto &pair : obj)
			{

				// �R�����g�͖���
				if (pair.first == L"#") continue;

				auto position = FromString<double>(pair.first);

				auto pos3D = pair.second.getArray();

				Vec3 pos;

				if (pos3D.size() == 2)
				{
					pos = Vec3(pos3D[0].getNumber(), 0, pos3D[1].getNumber());
				}

				if (pos3D.size() == 3)
				{
					pos = Vec3(pos3D[0].getNumber(), pos3D[1].getNumber(), pos3D[2].getNumber());
				}



				mmp[Clamp(position, 0.0, 1.0)] = pos;

			}


			return mmp;

		});



	}


	// ���[���̈ʒu��� 0 ~ 1 �� 0 ~ LANE_QUALITY �ɐ��K������
	Array<R2> Normalize(const Array<R> &lanes)
	{
		Array<R2> result;

		result.reserve(lanes.size());


		for (auto &lane : lanes)
		{

			R2 r2;


			for (auto &value : lane)
			{

				
				auto pos = Math::Floor(value.first * LANE_QUALITY);


				r2[(int)pos] = value.second;

			}

			result.emplace_back(r2);

		}

		return result;

	}


	LanePoints ToPoints(const R2 &_laneTimeState)
	{
		LanePoints result;


		auto laneTimeState = _laneTimeState;

		
		// size + 1 �̈ʒu�ɍŌ�̗v�f�𕡐�
		// nextIterator ���͈͊O�������Ȃ��悤�ɂ��邽��
		laneTimeState[LANE_QUALITY] = (--laneTimeState.end())->second;

		auto currentIterator = laneTimeState.begin();
		auto nextIterator = ++laneTimeState.begin();

		for (auto i : step(LANE_QUALITY))
		{

			// �n�_
			auto begin = currentIterator->second;
			// �I�_
			auto end = nextIterator->second;

			// ���̃L�[�t���[���܂ł̗v�f��
			// auto dist = std::distance(currentIterator, nextIterator);
			auto dist = nextIterator->first - currentIterator->first;

			auto toVec = end - begin;

			// 0 ~ 1 ( begin -> end �̈ʒu ) 
			auto t = (i - currentIterator->first) / (double)dist;

				
			// ���W
			auto pos = begin + toVec * t;


			result[i] = pos;


			// ���̃L�[�t���[����
			if (i == nextIterator->first)
			{
				++currentIterator;
				++nextIterator;
			}


		}

		return result;
	}

}

