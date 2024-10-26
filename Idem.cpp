// Lic:
// Idem
// Checks for dupe files
// 
// 
// 
// (c) Jeroen P. Broks, 2023, 2024
// 
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
// 
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.
// 
// Please note that some references to data like pictures or audio, do not automatically
// fall under this licenses. Mostly this is noted in the respective files.
// 
// Version: 24.03.22
// EndLic
#include <Slyvina.hpp>
#include <SlyvString.hpp>
#include <SlyvDir.hpp>
#include <SlyvArgParse.hpp>
#include <SlyvStream.hpp>
#include <SlyvVolumes.hpp>
#include <SlyvMD5.hpp>
#include <SlyvBank.hpp>

using namespace std;
using namespace Slyvina;
using namespace Units;


namespace Idem {
	
	ParsedArg Args{};
	map<string, vector<string>> Result{};

	string dName(string f,uint32 siz=60) {
		string ret{};
		if (f.size() >= siz) {
			ret = f.substr(0, 10) + "....." + f.substr(f.size() - (siz-16));
		} else {
			ret = f;
		}
		while (ret.size() < siz) ret += " ";
		return ret;
	}

	int Process(string dir) {
		dir = ChReplace(dir, '\\', '/');
		auto tdir{ AVolPath(dir) };
		cout << "Analysing: " << dName(dir) << "\r";
		auto files{ GetTree(tdir) };
		for (auto file : *files) {
			auto tfile{ tdir + "/" + file };
			cout << "Checking:  " << dName(tfile) << "\r";
			auto Ok = true;
			for (size_t p = 0; p < tfile.size(); p++) {
				Ok = Ok && tfile[p] >= 32 && tfile[p] <= 126;
			}
			if (Ok) {
				auto size{ FileSize(tfile) };
				if (size) {
					//int asize; if (size > 1000000) asize = 1000000; else asize = (int)size; 
					//auto content = LoadCharBuf(tfile); for(size_t i=0;i<size;i++) if (content[i]==0) content[i]=1)
					auto content = FLoadString(tfile, true);
					//auto hash = md5((uint8_t*)content, asize); //qc_md5(content.c_str());
					auto hash = md5(content.substr(0, 12000));
					auto tag{ hash + ":" + to_string(size) };
					Result[tag].push_back(tfile);
				} else {
					cout << "Empty file: " << file << "\n";
				}
			} else {
				cout << "Error: " << file << " (Non-ASCII characters in filename)\n\7";
			}
		}
		cout << "           " << dName("") << "\r";
		return 0;
	}
}


int main(int argc, char** args) {
	using namespace Idem;
	cout << "Idem - Coded by: Tricky\n";
	cout << "(c) Copyright " << CYear(2023, Right(__DATE__, 4)) << " Jeroen P.Broks\n";
	cout << "Build: " << __DATE__ << "; " << __TIME__ << "\n\n";
	FlagConfig FC{};
	Args = ParseArg(argc, args, FC);
	if (!Args.arguments.size()) {
		cout << "Usage: " << StripAll(Args.myexe) << " <Directories>\n";
		return 1;
	}
	int ret{ 0 };
	for (size_t i = 0; i < Args.arguments.size() && (!ret); i++) {
		ret = Process(Args.arguments[i]);
	}
	for (auto iR : Result) {
		if (iR.second.size() > 1) {
			printf("Possibly %d matches found on %s\n", (int)iR.second.size(), iR.first.c_str());
			for (size_t i = 0; i < iR.second.size(); i++) printf("%9d: %s\n", (int)(i+1), iR.second[i].c_str());
			printf("\n");
		}
	}
	return ret;
}