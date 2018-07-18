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
class T__AI_BONE{
private:
	/****************************************
	****************************************/
	enum{
		FBO_WIDTH	= 300,
		FBO_HEIGHT	= 300,
	};
	
	enum{
		LINE_FEED_PER		= 10,
		NUM_DIGITS_CONTENTS	= 40,
	};
	
	/****************************************
	****************************************/
	/********************
	singleton
	********************/
	T__AI_BONE();
	~T__AI_BONE();
	T__AI_BONE(const T__AI_BONE&); // Copy constructor. no define.
	T__AI_BONE& operator=(const T__AI_BONE&); // コピー代入演算子. no define.
	
	void Refresh_Contents();
	void init_Contents();
	void CopyContentsToString();
	
	
	/****************************************
	****************************************/
	/********************
	********************/
	int c_SkipProcessControl;
	
	/********************
	********************/
	ofFbo fbo;
	ofxSyphonServer fbo_TextureSyphonServer;
	
	const float t_Change;
	float t_LastRefresh;
	
	int DigitsContents[NUM_DIGITS_CONTENTS];
	string str_DigitsContents;
	
	ofTrueTypeFont font;
	
public:
	/****************************************
	****************************************/
	/********************
	********************/
	static T__AI_BONE* getInstance(){
		static T__AI_BONE inst;
		return &inst;
	}
	
	void exit();
	void setup();
	void update();
	void drawToFbo_SendSyphon();
	void draw_FboToMonitor();
};
