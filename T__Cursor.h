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
class T__CURSOR{
private:
	/****************************************
	****************************************/
	enum{
		FBO_WIDTH	= 300,
		FBO_HEIGHT	= 300,
	};
	
	enum{
		NUM_HISTORY = 3,
		NUM_SPEED_STEPS = 3,
		NUM_SENSORS = 6,
	};
	
	enum{
		UNITY_CURSOR_SURFACE_SIZE_W = 40,
		UNITY_CURSOR_SURFACE_SIZE_H = 40,
	};
	
	enum STATE{
		STATE_SLEEP,
		STATE_WAIT_SKELTONDATA,
		STATE_RUN,
	};
	
	enum MESSAGE{
		MESSAGE_SLEEP,
		MESSAGE_RUN,
	};
	
	/****************************************
	****************************************/
	/********************
	********************/
	int c_SkipProcessControl;
	
	/********************
	********************/
	float t_LastRefresh;
	
	ofFbo fbo;
	ofxSyphonServer fbo_TextureSyphonServer;
	
	ofSoundPlayer Sound_pi;
	
	ofVec2f history[NUM_HISTORY];
	ofVec2f CursorPos;
	int LastId_ofTarget;
	
	float Speed;
	ofVec2f MovDirection;
	
	STATE State;
	float t_LastSkeltonData;
	
	/****************************************
	****************************************/
	/********************
	singleton
	********************/
	T__CURSOR();
	~T__CURSOR();
	T__CURSOR(const T__CURSOR&); // Copy constructor. no define.
	T__CURSOR& operator=(const T__CURSOR&); // コピー代入演算子. no define.
	
	/********************
	********************/
	void load_and_set_sound();
	void Reset(bool b_1st);
	void StateChart();
	void drawCursor(const ofVec2f& pos, const ofColor& col);
	
public:
	/****************************************
	****************************************/
	/********************
	********************/
	static T__CURSOR* getInstance(){
		static T__CURSOR inst;
		return &inst;
	}
	
	void exit();
	void setup();
	void update();
	void drawToFbo_SendSyphon();
	void draw_FboToMonitor();
	void OscMessage(int MessageId);
};
