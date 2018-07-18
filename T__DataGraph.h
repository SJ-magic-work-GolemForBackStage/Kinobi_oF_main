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

/**************************************************
**************************************************/
struct VBO_SET{
	ofVbo Vbo;
	vector<ofVec3f> VboVerts;
	vector<ofFloatColor> VboColor;
	
	void setup(int size){
		VboVerts.resize(size);
		VboColor.resize(size);
		
		Vbo.setVertexData(&VboVerts[0], VboVerts.size(), GL_DYNAMIC_DRAW);
		Vbo.setColorData(&VboColor[0], VboColor.size(), GL_DYNAMIC_DRAW);
	}
	
	void set_singleColor(const ofColor& color){
		for(int i = 0; i < VboColor.size(); i++) { VboColor[i].set( double(color.r)/255, double(color.g)/255, double(color.b)/255, double(color.a)/255); }
	}
	
	void update(){
		Vbo.updateVertexData(&VboVerts[0], VboVerts.size());
		Vbo.updateColorData(&VboColor[0], VboColor.size());
	}
	
	void draw(int drawMode){
		Vbo.draw(drawMode, 0, VboVerts.size());
	}
	
	void draw(int drawMode, int total){
		if(VboVerts.size() < total) total = VboVerts.size();
		Vbo.draw(drawMode, 0, total);
	}
};

/**************************************************
**************************************************/
class ARC{
private:
	int Resolution;
	int size;
	
	ofVbo Vbo;
	vector<ofVec3f> VboVerts;
	vector<ofFloatColor> VboColor;
	
	float DegToRad(float deg){
		return deg * 3.14159265358979 / 180;
	}
	
public:
	ARC(int _Resolution, const ofColor& color)
	: Resolution(_Resolution)
	, size(2 * Resolution + 2)
	{
		VboVerts.resize(size);
		VboColor.resize(size);
		
		Vbo.setVertexData(&VboVerts[0], VboVerts.size(), GL_DYNAMIC_DRAW);
		Vbo.setColorData(&VboColor[0], VboColor.size(), GL_DYNAMIC_DRAW);
		
		set_singleColor(color);
	}
	
	void set_singleColor(const ofColor& color){
		for(int i = 0; i < VboColor.size(); i++) { VboColor[i].set( double(color.r)/255, double(color.g)/255, double(color.b)/255, double(color.a)/255); }
		Vbo.updateColorData(&VboColor[0], VboColor.size());
	}
	
	void update(float r0, float r1, float theta_from, float theta_w){
		/********************
		********************/
		if(r1 <= r0) return;
		
		/********************
		********************/
		float theta_step = theta_w / (Resolution - 1);
		float theta = theta_from;
		for(int i = 0; i < Resolution; i++, theta += theta_step){
			VboVerts[2 * i    ].set( r0 * cos(DegToRad(theta)), r0 * sin(DegToRad(theta)) );
			VboVerts[2 * i + 1].set( r1 * cos(DegToRad(theta)), r1 * sin(DegToRad(theta)) );
		}
		
		/********************
		********************/
		Vbo.updateVertexData(&VboVerts[0], VboVerts.size());
		Vbo.updateColorData(&VboColor[0], VboColor.size());
	}
	
	void draw(int x, int y){
		ofPushMatrix();
		ofTranslate(x, y);
			Vbo.draw(GL_TRIANGLE_STRIP, 0, VboVerts.size()); // GL_POLYGON は、Fillの仕方が狙いと違った.
		ofPopMatrix();
	}
};

/**************************************************
**************************************************/
class T__DATA_GRAPH{
private:
	/****************************************
	****************************************/
	enum{
		FBO_WIDTH	= 600,
		FBO_HEIGHT	= 600,
	};
	
	enum STATE{
		STATE_SLEEP,
		STATE_WAIT_SKELTONDATA,
		STATE_RUN,
	};
	
	enum RUN_TYPE{
		RUN_CENTER,
		RUN_SIDE,
		RUN_ALL,
		
		NUM_RUN_TYPES,
	};

	
	enum MESSAGE{
		MESSAGE_SLEEP,
		MESSAGE_RUN,
	};
	
	enum{
		GRAPH_L,
		GRAPH_R,
		
		NUM_LR,
	};
	
	enum{
		NUM_HISTORY__CENTER	= 40,
		NUM_HISTORY__SIDE	= 34,
	};
	
	enum{
		GRAPH__ZUPT__BAR_SPACE = 15,
		GRAPH__ZUPT__BAR_WIDTH = 15,
		GRAPH__ZUPT__BAR_HEIGHT = 15,
		
		GRAPH__HAND_ANGLE__BAR_SPACE = 12,
		GRAPH__HAND_ANGLE__BAR_MAX_WIDTH = 55,
		GRAPH__HAND_ANGLE__BAR_HEIGHT = 6,
	};
	
	
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
	
	STATE State;
	RUN_TYPE RunType;
	
	float t_toNextReset;
	float t_LastReset;
	float t_LastSkeltonData;
	
	/********************
	********************/
	VBO_SET Vboset_ZUPT[NUM_LR];
	VBO_SET Vboset_HandAngle_Bar[NUM_LR];
	VBO_SET Vboset_HandAngle_Line[NUM_LR];
	
	bool History_ZUPT[NUM_LR][NUM_HISTORY__CENTER];
	float History_Center[NUM_LR][NUM_HISTORY__CENTER];
	float History_HandAngle[NUM_LR][NUM_HISTORY__SIDE];
	
	ARC RaderArc; // include color.
	
	
	ofTrueTypeFont font_center;
	ofTrueTypeFont font_side;
	ofTrueTypeFont font_Rader_L;
	ofTrueTypeFont font_Rader_S;
	
	const ofColor col_ZUPT_on;
	const ofColor col_Graph_on;
	const ofColor col_Text_on;
	const ofColor col_off;
	const ofColor col_Graph_Rader_Coord;
	const ofColor col_Text_Rader;
	
	const ofColor col_GraphArc_L;
	const ofColor col_GraphArc_R;
	
	/********************
	********************/
	float t_LastDraw;
	const float ScanBar_speed;
	float ScanBar_y_R;
	float ScanBar_y_L;
	const float ScanBar_y_L__StartFrom;
	
	/********************
	********************/
	ofSoundPlayer Sound_pi;
	// ofSoundPlayer Sound_Morse;
	SOUND_FADE Sound_Morse;
	
	/****************************************
	****************************************/
	/********************
	singleton
	********************/
	T__DATA_GRAPH();
	~T__DATA_GRAPH();
	T__DATA_GRAPH(const T__DATA_GRAPH&); // Copy constructor. no define.
	T__DATA_GRAPH& operator=(const T__DATA_GRAPH&); // コピー代入演算子. no define.
	
	/********************
	********************/
	void load_and_set_sound();
	void init_GraphParam();
	void vboset_Square_SetCol(VBO_SET& Vboset, int id, const ofColor& color);
	void Reset();
	void StateChart();
	void Save_histroy();
	void Refresh_vboVerts();
	void Refresh_vboColor();
	void DrawVboset(const ofVec3f& Matrix_ofs, const ofVec3f& Matrix_scale, VBO_SET& vbo_set, int drawMode);
	void Draw_Rader(const ofVec3f& Matrix_ofs, const float angle, const string& str_Label, const ofColor& ArcCol);
	void Draw__DataToString(bool *SkipDisp, float DataArray[], int num, const ofVec3f& Matrix_ofs, const ofVec3f& Matrix_1st_ofs, const ofVec3f& Matrix_ofs_step, ofTrueTypeFont& font, bool b_IsHex);
	
	/****************************************
	****************************************/
	/******************************
	******************************/
	template <class T>
	void slide_array(T array[], int size, T new_val)
	{
		for(int i = 0; i < size - 1; i++){
			array[i] = array[i + 1];
		}
		
		array[size - 1] = new_val;
	}


	
public:
	/****************************************
	****************************************/
	/********************
	********************/
	static T__DATA_GRAPH* getInstance(){
		static T__DATA_GRAPH inst;
		return &inst;
	}
	
	void exit();
	void setup();
	void update();
	void OscMessage(int MessageId);
	void drawToFbo_SendSyphon();
	void draw_FboToMonitor();
};
