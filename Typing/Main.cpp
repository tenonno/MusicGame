
# include <Siv3D.hpp>

// レーン数
#define LANE_COUNT 16

#define LANE_QUALITY 50


#include <unordered_map>
#include <map>
#include <array>

#include "Utils.hpp"
#include "BMS_Note.hpp"


#include "BMS_Loader.hpp"

#include "LaneTemplate.hpp"


#include "Lane.hpp"

#include "LaneParser.hpp"


#include "Note.hpp"



#include "Layout.hpp"






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
	auto jsonFiles = GetFileFromExtension(L"Assets/test", L"json");



	std::unordered_map<String, Layout> layouts;


	for (const auto &path : jsonFiles)
	{

		Layout layout(path);


		auto name = layout.name();

		layouts[name] = layout;


		names.emplace_back(name);

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
		lanes[i] = layouts[activeLaneName].lanes()[i];
	}


	

	auto lookAt = layouts[activeLaneName].camera().lookat;
	auto position = layouts[activeLaneName].camera().pos;



	Graphics3D::SetRasterizerState(RasterizerState(FillMode::Solid, CullMode::None));

	while (System::Update())
	{

		ClearPrint();


		auto radio = gui.radioButton(L"rb1");

		if (radio.hasChanged || System::FrameCount() == 1)
		{



			activeLaneName = radio.itemNames[radio.checkedItem.value()];



		}

		const auto layout = layouts[activeLaneName];



		position = (position * 30 + layout.camera().pos) / 31.0;
		lookAt = (lookAt * 30 + layout.camera().lookat) / 31.0;


		Camera camera(position, lookAt, Vec3::Up, 45.0, 0.1);

		Graphics3D::SetCamera(camera);



		for (auto i : step(LANE_COUNT))
		{
			lanes[i].transform(layouts[activeLaneName].lanes()[i]);


			lanes[i].transform(layouts[activeLaneName].lanes2()[i]);

		}

		// Graphics3D::FreeCamera();


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



		Line3D(Vec3(0, 0, 0), Vec3(0, 10, 0)).drawForward(ColorF(0, 1, 0));


		Sphere(Vec3(0, 0, 0), 0.01).draw(Palette::Aqua);

		/*


		meshData.vertices[Random(meshData.vertices.size())].position += RandomVec3();

		mesh.fillVertices(meshData.vertices);

		mesh.draw(Palette::Pink);

		*/

	}
}
