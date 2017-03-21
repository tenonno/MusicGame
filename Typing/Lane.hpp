#pragma once




using LanePoints = Array<Vec3>;

class Lane
{

	LanePoints points;


	DynamicMesh m_mesh;


	Array<MeshVertex> m_vertices;


public:

	Lane()
	{}


	Lane(const LanePoints &points)
		: points(points)
	{

		m_vertices.resize(LANE_VERTEX_SIZE);


		const uint32 vertexSize = LANE_QUALITY * 2;
		const uint32 indexSize = (LANE_QUALITY - 1) * 6;

		MeshData m_meshData = MeshData(vertexSize, indexSize);


		for (auto i : step(vertexSize))
		{

			m_meshData.vertices[i].position = RandomVec3() * 10.0;

		}


		for (auto i : step(LANE_QUALITY - 1))
		{

			m_meshData.indices[i * 6 + 0] = i * 2 + 0;
			m_meshData.indices[i * 6 + 1] = i * 2 + 1;
			m_meshData.indices[i * 6 + 2] = i * 2 + 2;

			m_meshData.indices[i * 6 + 3] = i * 2 + 1;
			m_meshData.indices[i * 6 + 4] = i * 2 + 2;
			m_meshData.indices[i * 6 + 5] = i * 2 + 3;

		}

		m_mesh = DynamicMesh(m_meshData);


	}




	double aTime = 1.0;

	double w;
	ColorF color = Palette::White;

	double centerPlaneOpacity = 0.0;

	double noteSize;

	// ノーツの透明度を位置に合わせて変化させるか
	double fadeOpacity = 0.0;

	ColorF noteColor;

	double noteTime = 1.0;


	void update()
	{


	}



	

	Vec3 point(int index) const
	{
		return points[index];
	}

	LanePoints getPoints() const
	{
		return points;
	}

	void transform(const LaneTemplate &_template)
	{

		for (auto i : step(_template.vertices.size()))
		{

			m_vertices[i].position = L::L(m_vertices[i].position, _template.vertices[i].position);

		}


		noteColor.r = L::L(noteColor.r, _template.noteColor.r);
		noteColor.g = L::L(noteColor.g, _template.noteColor.g);
		noteColor.b = L::L(noteColor.b, _template.noteColor.b);
		noteColor.a = L::L(noteColor.a, _template.noteColor.a);


		noteTime = L::L(noteTime, _template.noteTime);


		noteSize = L::L(noteSize, _template.noteSize);


		color.r = L::L(color.r, _template.backgroundColor.r);
		color.g = L::L(color.g, _template.backgroundColor.g);
		color.b = L::L(color.b, _template.backgroundColor.b);
		color.a = L::L(color.a, _template.backgroundColor.a);



		m_mesh.fillVertices(m_vertices);// _template.vertices);

	}


	void transform(const Lane &lane)
	{

		w = (w * 30 + lane.w) / 31.0;







		LERP(centerPlaneOpacity);

		LERP(fadeOpacity);



		for (auto i : step(LANE_QUALITY))
		{

			points[i] = L::L(points[i], lane.point(i));

		}


	}

	void draw()
	{

		m_mesh.drawForward(color);

		// drawPoints();

	}

	void drawPoints()
	{


		for (auto vertex : m_vertices)
		{


			Circle(Graphics3D::ToScreenPos(vertex.position).xy(), 6.0)
				.draw(Palette::Black)
				.scaled(0.9).draw(Palette::Yellow);

		}




		for (auto &point : points)
		{

			// Sphere(point, 0.05).draw(Palette::Pink);




			Circle(Graphics3D::ToScreenPos(point).xy(), 6.0)
				.draw(Palette::Black)
				.scaled(0.9).draw(Palette::White);




		}



	}


};