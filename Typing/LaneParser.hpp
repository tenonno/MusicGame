#pragma once


#include <iterator>


namespace LaneParser
{


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

			std::reverse(points.begin(), points.end());


			auto size = points.size();

			auto length = 0.0;

			// レーンの長さ
			for (auto i : step(size - 1))
			{
				length += (JSONValueToVec3(points[i + 1]) - JSONValueToVec3(points[i])).length();
			}


			auto beforePos = JSONValueToVec3(points[0]);

			auto pos = 0.0;

			for (auto i : step(size))
			{

				// 0.0 ~ 1.0
				double position = 1.0 / size * i;

				pos += (beforePos - JSONValueToVec3(points[i])).length();
				beforePos = JSONValueToVec3(points[i]);

				position = (pos / length);

				int positionInt = Math::Floor(position * LANE_QUALITY);

				timePointMap[positionInt] = JSONArrayToVec3(points[i].getArray());

			}

			map[index++] = timePointMap;
			
		}

		return map;

	}


	Array<Vec3> ToPoints(const FramePointMap &_laneTimeState)
	{
		Array<Vec3> result(LANE_QUALITY);

		auto laneTimeState = _laneTimeState;




		
		// 最初と最後に必ず情報があるようにする
		laneTimeState[0] = laneTimeState.begin()->second;
		laneTimeState[LANE_QUALITY - 1] = (--laneTimeState.end())->second;


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

			if (dist == 0) MessageBox::Show(L"w");

			auto toVec = end - begin;

			// 0 ~ 1 ( begin -> end の位置 ) 
			auto t = (i - currentIterator->first) / (double)(dist);

				
			// 座標
			auto pos = begin + toVec * t;


			result[i] = pos;


			// 次のキーフレームに
			if (i == nextIterator->first)
			{
				++currentIterator;
				++nextIterator;

				if (nextIterator == laneTimeState.end())
				{
					--nextIterator;
				}

			}


		}




		return result;
	}

}

