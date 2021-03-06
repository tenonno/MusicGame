#pragma once


#define LERP(key) key = (key * 30 + lane. key) / 31.0


#define LANE_VERTEX_SIZE LANE_QUALITY * 2


struct LaneTemplate
{

	String name;

	Array<Vec3> points;

	Array<MeshVertex> vertices;

	ColorF backgroundColor;
	ColorF noteColor;

	double noteSize;

	// レーンの大きさ
	double size;

	// レーンの向き
	Optional<Vec3> forward;

	double noteTime;

	LaneTemplate() :
		points(LANE_COUNT),
		vertices(LANE_VERTEX_SIZE)
	{






	}

};
