#ifndef __Load_Obj_File_h__
#define __Load_Obj_File_h__

#include "Camera_YawPitch.h"
#include "OpenGLShaderUtilities.h"
#include "ObjModel.h"

#include "GlfwApp.h"

// not successful currelty 
// maybe the problem of textures (in frag shader?)
class LoadObjFile : public GlfwApp
{
protected:
	Camera_YawPitch camera;
	OpenGLShaderProgram shader;
	ObjModel model;
	float last_frame_dtime;
	float last_xpos, last_ypos;
	bool is_first_mouse;

public:
	LoadObjFile();
	~LoadObjFile() {}

	int init() override;
	int paint() override;
	void destory() override;
	int resize(int wd, int ht) override;
	void mouse_move(double xpos, double ypos) override;
	void process_keyboard_input() override;
};

#endif