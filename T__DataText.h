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
class T__DATA_TEXT{
private:
	/****************************************
	****************************************/
	enum{
		FBO_WIDTH	= 500,
		FBO_HEIGHT	= 500,
	};
	enum{
		NUM_FBOS = 3,
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
	ofFbo fbo[NUM_FBOS];
	ofFbo fbo_masked[NUM_FBOS];
	ofxSyphonServer fbo_TextureSyphonServer[NUM_FBOS];
	
	const float t_Change;
	float t_LastRefresh;
	
	ofTrueTypeFont font;
	
	STATE State;
	
	bool b_RefreshFbo;
	int SurfaceId;
	ofVec2f DrawPos;
	string str_Contents;
	
	ofImage img_mask;
	ofShader shader;
	
	SOUND_FADE Sound;
	float t_LastSkeltonData;
	
	/****************************************
	****************************************/
	/********************
	singleton
	********************/
	T__DATA_TEXT();
	~T__DATA_TEXT();
	T__DATA_TEXT(const T__DATA_TEXT&); // Copy constructor. no define.
	T__DATA_TEXT& operator=(const T__DATA_TEXT&); // コピー代入演算子. no define.
	
	/********************
	********************/
	void Reset();
	void Dice_Contents();
	void CopyContentsToString();
	void StateChart();
	
public:
	/****************************************
	****************************************/
	/********************
	********************/
	static T__DATA_TEXT* getInstance(){
		static T__DATA_TEXT inst;
		return &inst;
	}
	
	void exit();
	void setup();
	void update();
	void drawToFbo_SendSyphon();
	void draw_FboToMonitor(int id);
	void OscMessage(int MessageId);
};
