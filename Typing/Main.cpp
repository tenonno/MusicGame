
# include <Siv3D.hpp>

// レーン数
#define LANE_COUNT 16

#define LANE_QUALITY 24


#include <unordered_map>

#include "Utils.hpp"
#include "BMS_Note.hpp"


#include "BMS_Loader.hpp"

#include "Lane.hpp"

#include "LaneParser.hpp"


#include "Note.hpp"


#include <array>



class ILayout
{

	std::array<LaneLine, LANE_COUNT> laneLines;


};



#include <map>



void Main()
{
	
	Window::Resize(1280, 720);


	FontAsset::Register(L"font1", 20, L"Consolas");





	const Sound sound(L"Assets/dive_fb.wav");

	const Sound se(L"Assets/G2R_B_SYNTH_001.wav");


	GUI gui(GUIStyle::Default);
	gui.setTitle(L"タイトル");



	Array<Lane> lanes(16);



	Array<String> names;


	// JSON を読み込む
	auto jsonList = GetFileFromExtension(L"Assets/test", L"json");



	std::unordered_map<String, std::array<Lane, LANE_COUNT>> laneTemplates;
	std::unordered_map<String, Camera> cameraTemplates;


	for (const auto &path : jsonList)
	{
		JSONReader json(path);

		const auto name = json[L"name"].to_str();

		names.emplace_back(name);


		auto jsonArrayLanes = json[L"lanes"].getArray();

		// JSON からレーンの位置情報を取得
		auto _lanes = LaneParser::Parse(jsonArrayLanes);

		// レーンの位置情報を正規化
		auto nLanes = LaneParser::Normalize(_lanes);


		auto index = 0;


		std::array<Lane, LANE_COUNT> lanes;

		for (auto &lane : nLanes)
		{


			auto points = LaneParser::ToPoints(lane);


			auto data = json[L"lanes"].getArray()[index];
			




			Lane _lane(points);

			_lane.w = data[L"w"].getNumber();

			_lane.centerPlaneOpacity = json[L"centerPlane"][L"opacity"].getNumber();

			_lane.color = data[L"color"].isNull() ? Palette::White : Color(data[L"color"].getString());


			_lane.fadeOpacity = json[L"fadeOpacity"].getOr<bool>(false) ? 1.0 : 0.0;




			lanes[index] = _lane;

			++index;
		}

		laneTemplates[name] = lanes;


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

		cameraTemplates[name] = camera;

	}


	// ラジオボタン
	gui.add(L"rb1", GUIRadioButton::Create(names, 0, true));




	sound.setVolume(0.0);

	sound.play();

	const auto path = L"C:/Users/tis-teno/Desktop/sketch_160709b/sketch_160709b/fdfd.txt";


	Array<Note> notes;

	for (auto &bmsNote : BMS::Loader::Load(path))
	{

		if (RandomBool())
		{
			bmsNote.pos += 8;
		}


		Note note(bmsNote);


		notes.emplace_back(note);

	}




	Graphics3D::SetDepthStateForward(DepthState::None);
	Graphics3D::SetAmbientLightForward(ColorF(1));

	Graphics3D::SetAmbientLight(ColorF(1));



	Graphics3D::SetDepthState(DepthState::None);


	auto activeLaneName = names[0];


	for (auto i : step(LANE_COUNT))
	{
		lanes[i] = laneTemplates[activeLaneName][i];
	}



	auto lookAt = cameraTemplates[activeLaneName].lookat;
	auto position = cameraTemplates[activeLaneName].pos;




	while (System::Update())
	{

		ClearPrint();



		auto radio = gui.radioButton(L"rb1");

		if (radio.hasChanged || System::FrameCount() == 1)
		{



			activeLaneName = radio.itemNames[radio.checkedItem.value()];



		}



		position = (position * 30 + cameraTemplates[activeLaneName].pos) / 31.0;
		lookAt = (lookAt * 30 + cameraTemplates[activeLaneName].lookat) / 31.0;


		Camera camera(position, lookAt, Vec3::Up, 45.0, 0.1);

		Graphics3D::SetCamera(camera);




		for (auto i : step(LANE_COUNT))
		{
			lanes[i].transform(laneTemplates[activeLaneName][i]);
		}

		Graphics3D::FreeCamera();



		Println(L"Sec: ", sound.streamPosSec());
		Println(L"LookAt: ", lookAt);


		const double time = sound.streamPosSec();



		for (auto &lane : lanes)
		{
			lane.draw();
		}




		for (auto note : notes)
		{

			// if (note.m_bmsNote.played) continue;


			auto lane = lanes[note.index()];


			note.draw3D(lane, time);

			// note.draw(time);

			if (note.m_bmsNote.time <= time)
			{

				note.m_bmsNote.played = true;

				// se.play();

			}


		}

		Cylinder(Vec3(0, 0, 0), 0.05, 10).draw(ColorF(0, 1, 0));


	}
}
