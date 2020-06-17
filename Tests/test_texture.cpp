#include "TestsMain.h"

#include "TestTexture.h"

int test_texture(int argc, char** argv)
{
	TestTexture app;
	//app.set_texture_dim(TestTexture::one_d);
	return app.run();
}
