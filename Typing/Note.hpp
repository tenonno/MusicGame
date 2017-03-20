#pragma once


class Note
{

	uint8 m_index = 0;

public:

	BMS::Note m_bmsNote;


	Note(const BMS::Note &note)
		: m_bmsNote(note)
	{




	}

	uint8 index() const
	{
		return m_bmsNote.pos;
	}

	// レーンに映ってから判定ラインに乗るまで何秒かかるか
	double aTime = 0.2;// 1.0;


	double position(const double currentTime)
	{
		return (m_bmsNote.time - currentTime) / aTime;
	}


	void update(const double currentTime)
	{

	}


	void draw3D(const Lane &lane, const double currentTime)
	{


		// 再生位置
		const double pos = position(currentTime);


		if (pos > 1) return;
		if (pos < 0) return;


		auto v = pos * LANE_QUALITY;

		auto vv = Math::Floor(v);


		auto vvv = v - vv;

		




		auto begin = lane.point(vv);

		auto next = vv + 1 >= LANE_QUALITY ? vv : vv + 1;

		auto end = lane.point(next);

		auto pos3D = begin + (end - begin) * vvv;

		// Plane(pos3D, 1, 1, q).draw(Palette::Green);


		// 仮
		auto color = (ColorF)Palette::Green;

	

		color.a -= (pos * lane.fadeOpacity);


		// ノーツの向き
		Vec3 forward;



		// 始点と終点がほぼ同じ位置にならノーツの向きを Vec3::Forward にする
		// if (begin == end)
		if ((end - begin).lengthSq() < 0.001)
		{
			forward = Vec3::Forward;
		}
		else
		{
			forward = (end - begin).normalized();
		}

		CreatePlane(pos3D, forward, lane.w).drawForward(color);


		
		// Sphere(pos3D, 0.1).draw();


	}


};