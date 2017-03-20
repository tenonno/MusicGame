#pragma once




template<class T>
double GetBoundLength(const T &points)
{

	Vec3 min = points[0];
	Vec3 max = points[0];

	for (Vec3 pos : points)
	{

		if (min.x > pos.x) min.x = pos.x;
		if (min.y > pos.y) min.y = pos.y;
		if (min.z > pos.z) min.z = pos.z;

		if (max.x < pos.x) max.x = pos.x;
		if (max.y < pos.y) max.y = pos.y;
		if (max.z < pos.z) max.z = pos.z;

	}

	return (max - min).lengthSq();

}





// 拡張子からファイル一覧を取得する
Array<FilePath> GetFileFromExtension(const FilePath &directory, const String &extension)
{
	Array<FilePath> result;

	auto contents = FileSystem::DirectoryContents(directory);

	for (const auto &content : contents)
	{

		// ファイル && *.拡張子 ではないファイルを弾く
		if (!FileSystem::IsFile(content)) continue;
		if (!content.endsWith(L"." + extension)) continue;

		result.emplace_back(content);

	}

	return result;

}









template<class T, class Lambda>
T Filter(const T &array, const Lambda lambda)
{

	T newArray;


	for (auto &value : array)
	{

		if (lambda(value))
		{
			newArray.emplace_back(value);
		}

	}

	return newArray;

}

template<class Result, class T, class Lambda>
Array<Result> Map(const T &array, const Lambda lambda)
{

	Array<Result> newArray;

	for (auto &value : array)
	{

		newArray.emplace_back(lambda(value));

	}

	return newArray;

}





Plane CreatePlane(const Vec3 &position, const Vec3 &forward, const double size)
{

	Plane plane(position, size, size, Quaternion(Vec3::Forward, forward));

	return plane;

}



Plane CreatePlaneFromLine(const Vec3 &begin, const Vec3 &end, const double size)
{

	// begin -> end のベクトル
	auto toVec = end - begin;


	auto length = toVec.length();


	Plane plane(size, length, Quaternion(Vec3::Forward, toVec.normalized()));


	plane.setPos(begin + toVec / 2.0);


	return plane;

}

Plane CreatePlaneFromPoints(const Vec3 &begin, const Vec3 &end, const double size, const Quaternion &quaternion)
{

	// begin -> end のベクトル
	auto toVec = end - begin;


	auto length = toVec.length();


	Plane plane(size, length, quaternion);


	plane.setPos(begin + toVec / 2.0);


	return plane;

}














