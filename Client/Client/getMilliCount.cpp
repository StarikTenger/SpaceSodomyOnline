#include "getMilliCount.h"

using namespace std;
int getMilliCount() {
	timeb tb;
	ftime(&tb);
	int nCount = tb.millitm + (tb.time & 0xfffff) * 1000;
	return nCount;
}

std::string getTime(int time) {
	std::string milliSeconds = std::to_string(time % 1000);
	time /= 1000;
	std::string seconds = std::to_string(time % 60);
	time /= 60;
	std::string minutes = std::to_string(time);

	milliSeconds = milliSeconds.substr(0, 1);
	while (seconds.size() < 2)
		seconds = "0" + seconds;
	while (minutes.size() < 2)
		minutes = "0" + minutes;

	return minutes + ":" + seconds ;
}