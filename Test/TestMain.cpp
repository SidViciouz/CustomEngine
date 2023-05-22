#include "Renderer.h"
#include <iostream>

using namespace Renderer;

int main()
{
	try {

		CRenderer r;
		r.Initialize();

	}
	catch (std::string errorMessage)
	{
		std::cout << errorMessage;
	}

	while (1);

	return 0;
}