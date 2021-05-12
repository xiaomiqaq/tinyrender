#include <chrono>

using namespace std::chrono;
double fps()
{
	static double fps = 0.0;
	static int frameCount = 0;
	static auto lastTime = system_clock::now();
	static auto curTime = system_clock::now();

	curTime = system_clock::now();

	auto duration = duration_cast<microseconds>(curTime - lastTime);
	double duration_s = double(duration.count()) * microseconds::period::num / microseconds::period::den;

	if (duration_s > 2)//2秒之后开始统计FPS
	{
		fps = frameCount / duration_s;
		frameCount = 0;
		lastTime = curTime;
	}

	++frameCount;

	return fps;
}