#pragma once


#include <iterator>


namespace LaneParser
{

	// 0 ~ 1 のキーと位置情報
	// 例 :
	// {
	//     0.0: Vec3,
	//     1.0: Vec3
	// }
	using TimePointMap = std::map<double, Vec3>;

	// 0 ~ LANE_QUALITY のキーと位置情報
	// 例 :
	// {
	//     1: Vec3,
	//     50: Vec3
	// }
	using FramePointMap = std::map<int, Vec3>;


	std::array<FramePointMap, LANE_COUNT> Parse2(const JSONArray &lanes)
	{

		std::array<FramePointMap, LANE_COUNT> map;

		int index = 0;

		for (auto &lane : lanes)
		{

			FramePointMap timePointMap;

			auto points = lane[L"points"].getArray();

			auto size = points.size();

			for (auto i : step(size))
			{

				// 0.0 ~ 1.0
				double position = 1.0 / size * i;

				int positionInt = Math::Floor(position * LANE_QUALITY);

				timePointMap[positionInt] = JSONArrayToVec3(points[i].getArray());

			}

			map[index++] = timePointMap;
			
		}

		return map;

	}


	LanePoints ToPoints(const FramePointMap &_laneTimeState)
	{
		LanePoints result;


		auto laneTimeState = _laneTimeState;

		
		// size + 1 の位置に最後の要素を複製
		// nextIterator が範囲外を差さないようにするため
		laneTimeState[LANE_QUALITY] = (--laneTimeState.end())->second;

		auto currentIterator = laneTimeState.begin();
		auto nextIterator = ++laneTimeState.begin();

		for (auto i : step(LANE_QUALITY))
		{

			// 始点
			auto begin = currentIterator->second;
			// 終点
			auto end = nextIterator->second;

			// 次のキーフレームまでの要素数
			// auto dist = std::distance(currentIterator, nextIterator);
			auto dist = nextIterator->first - currentIterator->first;

			auto toVec = end - begin;

			// 0 ~ 1 ( begin -> end の位置 ) 
			auto t = (i - currentIterator->first) / (double)dist;

				
			// 座標
			auto pos = begin + toVec * t;


			result[i] = pos;


			// 次のキーフレームに
			if (i == nextIterator->first)
			{
				++currentIterator;
				++nextIterator;
			}


		}

		return result;
	}

}

