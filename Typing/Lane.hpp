#pragma once


struct LaneLine
{
	Vec3 begin;
	Vec3 end;
};


#define LERP(key) key = (key * 30 + lane. key) / 31.0



struct LaneTemplate
{

	String name;

	std::array<Vec3, LANE_COUNT> points;


};



using LanePoints = std::array<Vec3, LANE_QUALITY>;

class Lane
{


	LanePoints points;

	Quaternion m_quaternion;




	Plane createPlane(const Vec3 &begin, const Vec3 &end, const double size)
	{


		// begin -> end のベクトル
		auto toVec = end - begin;


		auto length = toVec.length();

		m_quaternion = Quaternion(Vec3::Forward, toVec.normalized());

		Plane plane(size, length, m_quaternion);


		plane.setPos(begin + toVec / 2.0);


		return plane;

	}

	bool m_drawPoints = false;



public:
	Lane()
	{}


	Lane(const LanePoints &points)
		: points(points)
	{
		m_drawPoints = true;
	}




	double aTime = 1.0;

	double w;
	Color color = Palette::White;

	double centerPlaneOpacity = 0.0;



	// ノーツの透明度を位置に合わせて変化させるか
	double fadeOpacity = 0.0;



	void update()
	{


	}

	Quaternion getQuaternion() const
	{
		return m_quaternion;
	}



	void draw()
	{


		drawPoints();

	}

	Vec3 point(int index) const
	{
		return points[index];
	}

	LanePoints getPoints() const
	{
		return points;
	}


	void transform(const Lane &lane)
	{

		w = (w * 30 + lane.w) / 31.0;



		color.r = (color.r * 30 + lane.color.r) / 31.0;
		color.g = (color.g * 30 + lane.color.g) / 31.0;
		color.b = (color.b * 30 + lane.color.b) / 31.0;

		LERP(centerPlaneOpacity);

		LERP(fadeOpacity);



		for (auto i : step(LANE_QUALITY))
		{

			points[i] = (points[i] * 30 + lane.point(i)) / 31.0;

		}


	}

	void drawPoints()
	{


		Vec3 sum(0, 0, 0);

		for (auto &point : points)
		{
			sum += point;
		}

		auto center = sum / LANE_QUALITY;





		// jubeat 対策
		// レーンのサイズが一定以下になったら代わりの Plane を描画する
		{

			auto length = GetBoundLength(points);

			if (length < 1.0)
			{

				ColorF color2 = (ColorF)color;

				color2.a = 0.5;

				Plane(center, w, w).drawForward(color2);


			}

		}

		auto before = points[0];

		for (auto &point : points)
		{

			// Sphere(point, 0.05).draw(Palette::Pink);

			auto lanePlane = createPlane(point, before, w);

			lanePlane.scaled(1.1).draw(Palette::Black);

			lanePlane.draw(color);






			before = point;

			if (m_drawPoints)
			{
				const auto v = Graphics3D::ToScreenPos(point);
				// FontAsset(L"font1").drawAt(Format(point), Vec2(v.x, v.y));
			}




		}

		/*

		if (centerPlaneOpacity > 0.0)
		{

			ColorF color2 = (ColorF)color;

			color2.a = centerPlaneOpacity;

			Plane(center, w, w).drawForward(color2);

		}
		*/


		/*

		Array<Particle> particles;

		particles.emplace_back(center, 1, ColorF(1.0, 0.0, 0.0, 0.8));

		Graphics3D::DrawParticlesForward(particles);

		*/

	}


};