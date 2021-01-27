#pragma once

#include <px4_platform_common/app.h>

class SaxionExample
{
public:
	SaxionExample() {}

	~SaxionExample() {}

	int main();

	static px4::AppState appState; /* track requests to terminate app */
}
