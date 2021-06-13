#ifndef UTILS_HPP
#define UTILS_HPP

#include <string>

using namespace std;

class Utils {
public:

	static string shift(string& cmd){
		string res = cmd;
		while(res.length() && res[0]==' ') res = res.substr(1);
		size_t pos = res.find(' ');
		if (pos == string::npos) {
			cmd = "";
		}else{
			res = res.substr(0, pos);
			cmd = cmd.substr(pos+1);
		}
		return res;
	}
};

#endif
