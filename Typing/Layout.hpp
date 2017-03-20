#pragma once


class Layout
{

	std::array<LaneLine, LANE_COUNT> laneLines;

	String m_name;
	Camera m_camera;

	Array<Lane> m_lanes;

	void load(const FilePath &path)
	{


		JSONReader json(path);

		m_name = json[L"name"].to_str();



		auto json_lanes = json[L"lanes"].getArray();




		// JSON ‚©‚çƒŒ[ƒ“‚ÌˆÊ’uî•ñ‚ğæ“¾
		auto _lanes = LaneParser::Parse2(json_lanes);

		auto index = 0;


		Array<Lane> lanes(LANE_COUNT);

		for (auto &lane_framePoint : _lanes)
		{


			auto points = LaneParser::ToPoints(lane_framePoint);


			auto data = json[L"lanes"].getArray()[index];




			Lane _lane(points);

			_lane.w = data[L"w"].getNumber();

			_lane.centerPlaneOpacity = json[L"centerPlane"][L"opacity"].getNumber();

			_lane.color = data[L"color"].isNull() ? Palette::White : Color(data[L"color"].getString());


			_lane.fadeOpacity = json[L"fadeOpacity"].getOr<bool>(false) ? 1.0 : 0.0;




			lanes[index] = _lane;

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

};