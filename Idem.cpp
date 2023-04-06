#include <Slyvina.hpp>
#include <SlyvString.hpp>
#include <SlyvDir.hpp>
#include <SlyvArgParse.hpp>
#include <SlyvStream.hpp>
#include <SlyvVolumes.hpp>
#include <SlyvMD5.hpp>

using namespace std;
using namespace Slyvina;
using namespace Units;


namespace Idem {
	
	ParsedArg Args{};
	map<string, vector<string>> Result{};

	string dName(string f,uint32 siz=60) {
		string ret{};
		if (f.size() >= siz) {
			ret = f.substr(0, 10) + "....." + f.substr(f.size() - 16);
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
			auto size{ FileSize(tfile) };
			auto content = FLoadString(tfile);
			auto hash = md5(content);
			auto tag{ TrSPrintF("%s::%09d",hash.c_str(),size) };
			Result[tag].push_back(tfile);
		}
		cout << "           " << dName("") << "\r";
		return 0;
	}
}


int main(int argc, char** args) {
	using namespace Idem;
	cout << "Idem - Coded by: Tricky\n";
	cout << "(c) Copyright 2023 Jeroen P. Broks\n";
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
			printf("Possibly %d matches found on %s\n", iR.second.size(), iR.first.c_str());
			for (size_t i = 0; i < iR.second.size(); i++) printf("%9d: %s\n", i, iR.second[i]);
			printf("\n");
		}
	}
	return ret;
}
