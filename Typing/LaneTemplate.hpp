#pragma once


#define LERP(key) key = (key * 30 + lane. key) / 31.0


#define LANE_VERTEX_SIZE LANE_QUALITY * 2


struct LaneTemplate
{

	String name;

	Array<Vec3> points;

	Array<MeshVertex> vertices;

	Color backgroundColor;

	// ƒŒ[ƒ“‚Ì‘å‚«‚³
	double size;

	// ƒŒ[ƒ“‚ÌŒü‚«
	Optional<Vec3> forward;

	LaneTemplate() :
		points(LANE_COUNT),
		vertices(LANE_VERTEX_SIZE)
	{






	}

};
