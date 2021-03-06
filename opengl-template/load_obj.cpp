#include "load_obj.h"

Obj obj1;

Obj::Obj(string filename) {
	this->filename = filename;
	LoadObjFile();
}

void Obj::Init(string filename) {
	this->filename = filename;
	LoadObjFile();
}

void fun(string &str, int &x, int &y, int &z)
{
	string s1, s2, s3;
	int i = 0;
	while (str[i] != '/') {
		s1.push_back(str[i]);
		i++;
	}
	i++;
	while (str[i] != '/') {
		s2.push_back(str[i]);
		i++;
	}
	i++;
	while (i<str.size()) {
		s3.push_back(str[i]);
		i++;
	}

	x = atoi(s1.c_str());
	y = atoi(s2.c_str());
	z = atoi(s3.c_str());

 }


 bool Obj::LoadObjFile()
 {
	 string line;
	 ifstream f;
	 f.open(filename);

	 while (getline(f, line))
	 {	
		 string str_type;

		 // remove backspace
		while (line[0] == ' ') line.erase(0, 1);

		// determine the type
		for (int i = 0; i < line.size() && line[i] !=' '; i++)
		{
			str_type.push_back(line[i]);
		}
		line.erase(0, str_type.size());
		while (line[0] == ' ') line.erase(0, 1);
		// deal with different type
		
		if (str_type == "f")
		{		
			Patch patch_temp;
			while (line.size()>0)
			{
				// find vertex_id/texture_id/normal_id
				string str_vtn;
				int i;
				for ( i= 0; i < line.size() && line[i] != ' '; i++)
				{
					str_vtn.push_back(line[i]);
				}
				
				// delete str_vtn in line
				if (i != line.size())
					line.erase(0, str_vtn.size() + 1);
				else
					line.erase(0, str_vtn.size());

				// deal with str_vtn
				string str_int;				
				// vertex_id
				for (i = 0; i < str_vtn.size() && str_vtn[i] != '/'; i++)
				{
					str_int.push_back(str_vtn[i]);
				}
				/// add vertex_id
				patch_temp.vertex_id_.push_back(atoi(str_int.c_str()) - 1);
				/// delete vertex_id from str_vtn
				str_vtn.erase(0, str_int.size() + 1);
				/// clear str_int
				str_int.clear();

				// '/' is exist
				if (i < str_vtn.size())
				{
					string temp;
					/// find texture_id
					for (int j = 0; str_vtn[j] != '/'; j++) { str_int.push_back(str_vtn[j]); }
					/// add texture_id
					patch_temp.texture_id_.push_back(atoi(str_int.c_str()) - 1);
					/// delete texture_id
					str_vtn.erase(0, str_int.size() + 1);
					/// str_int clear
					str_int.clear();

					/// add normal_id
					patch_temp.normal_id_.push_back(atoi(str_vtn.c_str()) - 1);
				}											
			}
			// add patch
			faces_.push_back(patch_temp);
		}
		else if( str_type=="v" || str_type == "vn" || str_type == "vt")
		{
			int i = 0;
			Vec3f p;
			string str;
			stringstream ss(line);
			vector<float> temp_xyz;
			while (ss >> str)
				temp_xyz.push_back(atof(str.c_str()));

			switch (temp_xyz.size())
			{
			case 0:
				cout << "error1" << endl;
				break;
			case 1:
				p.x = temp_xyz[0];
				p.y = 0;
				p.z = 0;
				break;
			case 2:
				p.x = temp_xyz[0];
				p.y = temp_xyz[1];
				p.z = 0;
				break;
			case 3:
				p.x = temp_xyz[0];
				p.y = temp_xyz[1];
				p.z = temp_xyz[2];
				break;
			}



			// add to vertex
			if (str_type == "v")  points_.push_back(p);
			if (str_type == "vn") vn_.push_back(p);
			if (str_type == "vt") vt_.push_back(p);			
		}
	 }

	 //cacluate initial scale
	 get_radius();
	 return true;
 }

void Obj::get_radius()
{
	Vec3f s0;
	radius_ = 0.0f;
	for (auto &p : points_)
	{
		float dist = s0.DistanceXY(p);
		radius_ = radius_ > dist ? radius_ : dist;
	}

	for (auto& f:faces_)
	{
		Vec3f v1 = points_[f.vertex_id_[0]] - points_[f.vertex_id_[1]];
		Vec3f v2 = points_[f.vertex_id_[1]] - points_[f.vertex_id_[2]];
		Vec3f v3 = v1.Cross(v2);
		f.normal_ = v3.Normalize();
	}
}
