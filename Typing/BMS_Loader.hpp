#pragma once


namespace BMS::Loader
{


	inline Array<Note> Load(const FilePath &path)
	{


		Array<Note> notes;


		TextReader reader(path);

		String line;

		bool a = false;



		while (reader.readLine(line))
		{
			Graphics2D::ClearTransform();




			if (line == L"###") a = true;

			if (!line.length) continue;


			if (a)
			{

				auto aa = line.split(L':');

				if (aa.size() < 2) continue;

				auto bb = aa[1].split(L'/');


				Note note;

				note.pos = Parse<uint8>(aa[0]);
				note.time = Parse<int>(bb[0]) * 0.001;
				note.se = Parse<uint16>(bb[1]);


				notes.emplace_back(note);

			}
			else
			{

				/*

				String dir = L"Assets/FREEDOM DiVE/";

				auto aa = line.split(L':');

				const Sound sound(dir + aa[1]);


				mp[Parse<int>(aa[0])] = sound;

				*/

			}

		}


		return notes;


	}



}