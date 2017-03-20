#pragma once


class Layout
{

	String m_name;
	Camera m_camera;

	Array<Lane> m_lanes;
	// m_lanes を m_laneTemplates に置換予定
	Array<LaneTemplate> m_laneTemplates;

	void load(const FilePath &path)
	{


		JSONReader json(path);

		m_name = json[L"name"].to_str();



		auto json_lanes = json[L"lanes"].getArray();



		// JSON からレーンの位置情報を取得
		auto _lanes = LaneParser::Parse2(json_lanes);

		auto index = 0;


		Array<Lane> lanes(LANE_COUNT);

		for (auto &lane_framePoint : _lanes)
		{


			auto points = LaneParser::ToPoints(lane_framePoint);


			auto json_lane = json[L"lanes"].getArray()[index];



			// 削除予定
			{
				Lane _lane(points);
				_lane.w = json_lane[L"w"].getNumber();
				_lane.centerPlaneOpacity = json[L"centerPlane"][L"opacity"].getNumber();
				_lane.color = json_lane[L"color"].isNull() ? Palette::White : Color(json_lane[L"color"].getString());
				_lane.fadeOpacity = json[L"fadeOpacity"].getOr<bool>(false) ? 1.0 : 0.0;
				lanes[index] = _lane;
			}





			LaneTemplate _template;


			_template.size = json_lane[L"w"].getNumber();
			_template.backgroundColor = Color(json_lane[L"color"].getOr<String>(L"#fff"));



			auto forward = json_lane[L"forward"];
			if (!forward.isNull()) _template.forward.reset(JSONArrayToVec3(forward.getArray()));


			// ノーツ通過点の数
			auto pointSize = json_lane[L"points"].getArray().size();

			// ノーツが動かないなら
			// 複数の点からレーンを生成できない
			if (pointSize == 1)
			{

				// points を center を中心にした Plane の形に書き換える

				auto center = JSONArrayToVec3(json_lane[L"points"].getArray()[0].getArray());

				auto forward = _template.forward.value_or(Vec3::Forward).normalize();


				// 
				for (auto i : step(LANE_QUALITY))
				{

					auto vec = forward * (_template.size / (LANE_QUALITY - 1) * i);

					// vec -= forward * _template.size / 2.0;

					points[i] = center - forward * _template.size / 2.0 + vec;

				}

			}



			// LaneTemplate#vertices を設定する
			for (auto i : step(LANE_QUALITY - 1))
			{

				auto point1 = points[i];
				auto point2 = points[i + 1];


				auto toVec = point2 - point1;



				auto Q = Quaternion(Vec3::Forward, toVec.normalized());



				MeshVertex v1, v2, v3, v4;

				v1.position = point1 + Q * Vec3::Left * _template.size / 2;
				v2.position = point1 + Q * Vec3::Right * _template.size / 2;

				v3.position = point2 + Q * Vec3::Left * _template.size / 2;
				v4.position = point2 + Q * Vec3::Right * _template.size / 2;


				/*
				v1.position = point1;
				v2.position = point1;
				v3.position = point2;
				v4.position = point2;
				*/

				// 頂点を調整する（仮）
				if (i != 0 && i != LANE_QUALITY - 1)
				{

					v1.position = (_template.vertices[i * 2 + 0].position + v1.position) / 2.0;
					v2.position = (_template.vertices[i * 2 + 1].position + v2.position) / 2.0;

					/*
					v1.position = point1 + (v1.position - point1).normalize() * _template.size / 2.0;
					v2.position = point1 + (v2.position - point1).normalize() * _template.size / 2.0;
					*/
				}

				_template.vertices[i * 2 + 0] = v1;
				_template.vertices[i * 2 + 1] = v2;

				_template.vertices[i * 2 + 2] = v3;
				_template.vertices[i * 2 + 3] = v4;




			}


			// MessageBox::Show(ToString(_template.vertices.size()));


			m_laneTemplates.emplace_back(_template);



			++index;
		}

		m_lanes = lanes;

		auto lookAt = json[L"camera"][L"lookAt"].getArray();
		auto position = json[L"camera"][L"position"];


		Camera camera(Vec3(
			position[0].getNumber(),
			position[1].getNumber(),
			position[2].getNumber()
		), Vec3(
			lookAt[0].getNumber(),
			lookAt[1].getNumber(),
			lookAt[2].getNumber()), Vec3(0, 1, 0), 45.0, 0.1);

		m_camera = camera;

	}


public:

	Layout()
	{}

	Layout(const FilePath &path)
	{

		load(path);

	}

	String name() const
	{
		return m_name;
	}

	Camera camera() const
	{
		return m_camera;
	}

	Array<Lane> lanes() const
	{
		return m_lanes;
	}

	Array<LaneTemplate> lanes2() const
	{
		return m_laneTemplates;
	}



};