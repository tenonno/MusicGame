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

				// コメントは無視
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


	// レーンの位置情報 0 ~ 1 を 0 ~ LANE_QUALITY に正規化する
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

