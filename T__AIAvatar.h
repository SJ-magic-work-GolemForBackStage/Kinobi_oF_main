/************************************************************
************************************************************/
#pragma once

/************************************************************
************************************************************/
#include "ofMain.h"
#include "ofxSyphon.h"

#include "sj_common.h"

/************************************************************
************************************************************/
class T__AI_AVATAR{
private:
	/****************************************
	****************************************/
	/*
	enum{
		FBO_WIDTH	= 300,
		FBO_HEIGHT	= 300,
	};
	*/
	
	/****************************************
	****************************************/
	/********************
	singleton
	********************/
	T__AI_AVATAR();
	~T__AI_AVATAR();
	T__AI_AVATAR(const T__AI_AVATAR&); // Copy constructor. no define.
	T__AI_AVATAR& operator=(const T__AI_AVATAR&); // コピー代入演算子. no define.
	
	/****************************************
	****************************************/
	/********************
	********************/
	int c_SkipProcessControl;
	
	/********************
	********************/
	ofFbo fbo;
	ofxSyphonServer fbo_TextureSyphonServer;
	ofImage image;
	
	float image_ofs_y;
	const float shift_y_ratio_per_sec;
	float shift_y_per_sec;
	
	float t_LastINT;
	
	
public:
	/****************************************
	****************************************/
	/********************
	********************/
	static T__AI_AVATAR* getInstance(){
		static T__AI_AVATAR inst;
		return &inst;
	}
	
	void exit();
	void setup();
	void update();
	void drawToFbo_SendSyphon();
	void draw_FboToMonitor();
};
