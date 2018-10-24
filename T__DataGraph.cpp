/************************************************************
************************************************************/
#include "T__DataGraph.h"

/************************************************************
************************************************************/

/******************************
******************************/
T__DATA_GRAPH::T__DATA_GRAPH()
: State(STATE_SLEEP)
, RunType(RUN_ALL)
, col_ZUPT_on(255, 255, 255, 100)
, col_Graph_on(255, 255, 255, 120)
, col_Text_on(255, 255, 255, 240)
, col_off(0, 0, 0, 0)
, col_Text_Rader(255, 255, 255, 200)
, col_Graph_Rader_Coord(255, 255, 255, 200)
, t_toNextReset(8.0)
, t_LastReset(0)
, t_LastSkeltonData(0)
, t_LastDraw(0)
, ScanBar_speed(20)
, ScanBar_y_L__StartFrom(200)
, col_GraphArc_L(255, 0, 0, 120)
, col_GraphArc_R(0, 180, 255, 120)
// , col_GraphArc_R(0, 100, 0, 120)
, RaderArc(20, ofColor(255, 255, 255, 100)) // temporary color : will be updated before every draw.
, c_SkipProcessControl(0)
, Sound_Morse("sound/morse.wav", 0.12, 1.0, 1.0)
{
	/********************
	********************/
	init_GraphParam();
	
	ScanBar_y_L = ScanBar_y_L__StartFrom;
	ScanBar_y_R = 0;
}

/******************************
******************************/
void T__DATA_GRAPH::init_GraphParam()
{
	/********************
	********************/
	// font.load("font/RictyDiminished-Regular.ttf", 15, true, true, true);
	//*
	font_center.load("font/RictyDiminishedDiscord-Bold.ttf", 10, true, true, true);
	font_side.load("font/RictyDiminishedDiscord-Bold.ttf", 10, true, true, true);
	/*/
	font_center.load("font/RictyDiminished-Regular.ttf", 9, true, true, true);
	font_side.load("font/RictyDiminished-Regular.ttf", 9, true, true, true);
	//*/
	
	font_Rader_L.load("font/RictyDiminishedDiscord-Bold.ttf", 24, true, true, true);
	font_Rader_S.load("font/RictyDiminishedDiscord-Bold.ttf", 15, true, true, true);
	
	/********************
	********************/
	RaderArc.update(55, 68, -30, 60);
	
	/********************
	********************/
	for(int lr = 0; lr < NUM_LR; lr++){
		/********************
		********************/
		for(int i = 0; i < NUM_HISTORY__CENTER; i++){
			History_ZUPT[lr][i] = true; // stay.
			History_Center[lr][i] = 0.0;
		}
		
		for(int i = 0; i < NUM_HISTORY__SIDE; i++){
			History_HandAngle[lr][i] = 0.0;
		}
		
		/********************
		********************/
		Vboset_ZUPT[lr].setup(NUM_HISTORY__CENTER * 4); // square.
		Vboset_HandAngle_Bar[lr].setup(NUM_HISTORY__SIDE * 4); // square.
		Vboset_HandAngle_Line[lr].setup(NUM_HISTORY__SIDE * 1); // Line.
		
		/********************
		1-2
		| |
		0-3
		********************/
		for(int i = 0; i < NUM_HISTORY__CENTER; i++){
			Vboset_ZUPT[lr].VboVerts[i * 4 + 0].set(0						, GRAPH__ZUPT__BAR_SPACE * i + GRAPH__ZUPT__BAR_HEIGHT);
			Vboset_ZUPT[lr].VboVerts[i * 4 + 1].set(0						, GRAPH__ZUPT__BAR_SPACE * i);
			Vboset_ZUPT[lr].VboVerts[i * 4 + 2].set(GRAPH__ZUPT__BAR_WIDTH	, GRAPH__ZUPT__BAR_SPACE * i);
			Vboset_ZUPT[lr].VboVerts[i * 4 + 3].set(GRAPH__ZUPT__BAR_WIDTH	, GRAPH__ZUPT__BAR_SPACE * i + GRAPH__ZUPT__BAR_HEIGHT);
		}
		
		Vboset_ZUPT[lr].set_singleColor(col_ZUPT_on);
		
		
		/********************
		********************/
		for(int i = 0; i < NUM_HISTORY__SIDE; i++){
			/* */
			Vboset_HandAngle_Bar[lr].VboVerts[i * 4 + 0].set(0, GRAPH__HAND_ANGLE__BAR_SPACE * i + GRAPH__HAND_ANGLE__BAR_HEIGHT);
			Vboset_HandAngle_Bar[lr].VboVerts[i * 4 + 1].set(0, GRAPH__HAND_ANGLE__BAR_SPACE * i);
			Vboset_HandAngle_Bar[lr].VboVerts[i * 4 + 2].set(0, GRAPH__HAND_ANGLE__BAR_SPACE * i);
			Vboset_HandAngle_Bar[lr].VboVerts[i * 4 + 3].set(0, GRAPH__HAND_ANGLE__BAR_SPACE * i + GRAPH__HAND_ANGLE__BAR_HEIGHT);
			
			/* */
			Vboset_HandAngle_Line[lr].VboVerts[i].set(0, GRAPH__HAND_ANGLE__BAR_SPACE * i);
		}
		
		Vboset_HandAngle_Bar[lr].set_singleColor(col_Graph_on);
		Vboset_HandAngle_Line[lr].set_singleColor(col_Graph_on);
	}
}

/******************************
******************************/
T__DATA_GRAPH::~T__DATA_GRAPH()
{
}

/******************************
******************************/
void T__DATA_GRAPH::exit()
{
}

/******************************
******************************/
void T__DATA_GRAPH::vboset_Square_SetCol(VBO_SET& Vboset, int id, const ofColor& color)
{
	Vboset.VboColor[id * 4 + 0].set( double(color.r)/255, double(color.g)/255, double(color.b)/255, double(color.a)/255 );
	Vboset.VboColor[id * 4 + 1].set( double(color.r)/255, double(color.g)/255, double(color.b)/255, double(color.a)/255 );
	Vboset.VboColor[id * 4 + 2].set( double(color.r)/255, double(color.g)/255, double(color.b)/255, double(color.a)/255 );
	Vboset.VboColor[id * 4 + 3].set( double(color.r)/255, double(color.g)/255, double(color.b)/255, double(color.a)/255 );
}

/******************************
******************************/
void T__DATA_GRAPH::setup()
{
	/********************
	********************/
	fbo.allocate(FBO_WIDTH, FBO_HEIGHT, GL_RGBA, 4);
	fbo_TextureSyphonServer.setName("oF Data Graph");
	
	/********************
	********************/
	load_and_set_sound();
}

/******************************
******************************/
void T__DATA_GRAPH::Reset()
{
	int NextRunType = (int)( ((double)rand() / ((double)RAND_MAX + 1)) * NUM_RUN_TYPES );
	if(NextRunType == RunType){
		NextRunType++;
		if(NUM_RUN_TYPES <= NextRunType) NextRunType = 0;
	}
	RunType = RUN_TYPE(NextRunType);
	// RunType = RUN_ALL;
	
	Sound_pi.play();
	Sound_Morse.play();
	
	t_toNextReset = ofRandom(15.0, 25.0);
	t_LastReset = now;
}

/******************************
******************************/
void T__DATA_GRAPH::load_and_set_sound()
{
	Sound_pi.loadSound("sound/pi_1.mp3");
	Sound_pi.setLoop(false);
	Sound_pi.setMultiPlay(true);
	Sound_pi.setVolume(0.2);
}

/******************************
******************************/
void T__DATA_GRAPH::update()
{
	/********************
	********************/
	ofSoundUpdate();
	Sound_Morse.update();
	
	/********************
	********************/
	StateChart();
	
	/********************
	********************/
	if(FromGolem_FrameDataAll.b_set){
		Save_histroy();
		
		Refresh_vboVerts();
		Refresh_vboColor();
		
		for(int lr = 0; lr < NUM_LR; lr++){
			Vboset_ZUPT[lr].update();
			Vboset_HandAngle_Bar[lr].update();
			Vboset_HandAngle_Line[lr].update();
		}
	}
}

/******************************
******************************/
void T__DATA_GRAPH::Refresh_vboVerts()
{
	for(int lr = 0; lr < NUM_LR; lr++){
		for(int i = 0; i < NUM_HISTORY__SIDE; i++){
			float val_to_x = ofMap(History_HandAngle[lr][i], 0, 360, 0, GRAPH__HAND_ANGLE__BAR_MAX_WIDTH, true);
			
			/* */
			Vboset_HandAngle_Bar[lr].VboVerts[i * 4 + 0].set(0			, GRAPH__HAND_ANGLE__BAR_SPACE * i + GRAPH__HAND_ANGLE__BAR_HEIGHT);
			Vboset_HandAngle_Bar[lr].VboVerts[i * 4 + 1].set(0			, GRAPH__HAND_ANGLE__BAR_SPACE * i);
			Vboset_HandAngle_Bar[lr].VboVerts[i * 4 + 2].set(val_to_x	, GRAPH__HAND_ANGLE__BAR_SPACE * i);
			Vboset_HandAngle_Bar[lr].VboVerts[i * 4 + 3].set(val_to_x	, GRAPH__HAND_ANGLE__BAR_SPACE * i + GRAPH__HAND_ANGLE__BAR_HEIGHT);
			
			/* */
			Vboset_HandAngle_Line[lr].VboVerts[i].set(val_to_x, GRAPH__HAND_ANGLE__BAR_SPACE * i);
		}
	}
}

/******************************
******************************/
void T__DATA_GRAPH::Refresh_vboColor()
{
	for(int lr = 0; lr < NUM_LR; lr++){
		for(int i = 0; i < NUM_HISTORY__CENTER; i++){
			if(History_ZUPT[lr][i])	vboset_Square_SetCol(Vboset_ZUPT[lr], i, col_ZUPT_on);
			else					vboset_Square_SetCol(Vboset_ZUPT[lr], i, col_off);
		}
	}
}

/******************************
******************************/
void T__DATA_GRAPH::Save_histroy()
{
	slide_array(History_ZUPT[GRAPH_L], NUM_HISTORY__CENTER, FromGolem_FrameDataAll.ZUPT_L ? true : false);
	slide_array(History_ZUPT[GRAPH_R], NUM_HISTORY__CENTER, FromGolem_FrameDataAll.ZUPT_R ? true : false);
	
	slide_array(History_Center[GRAPH_L], NUM_HISTORY__CENTER, FromGolem_FrameDataAll.AI2_pos[0].x);
	slide_array(History_Center[GRAPH_R], NUM_HISTORY__CENTER, FromGolem_FrameDataAll.AI2_pos[0].z);
	
	slide_array(History_HandAngle[GRAPH_L], NUM_HISTORY__SIDE, FromGolem_FrameDataAll.Hand_Theta_L);
	slide_array(History_HandAngle[GRAPH_R], NUM_HISTORY__SIDE, FromGolem_FrameDataAll.Hand_Theta_R);
}

/******************************
******************************/
void T__DATA_GRAPH::StateChart()
{
	switch(State){
		case STATE_SLEEP:
			break;
			
		case STATE_WAIT_SKELTONDATA:
			if(FromGolem_FrameDataAll.b_set){
				State = STATE_RUN;
				Reset();
				t_LastSkeltonData = now;
			}
			break;
			
		case STATE_RUN:
			if(FromGolem_FrameDataAll.b_set){
				t_LastSkeltonData = now;
			}
			
			if(0.2 < now - t_LastSkeltonData){
				State = STATE_WAIT_SKELTONDATA;
				Sound_Morse.pause();
			}else if(t_toNextReset < now - t_LastReset){
				Reset();
			}
			
			
			break;
	}
}

/******************************
******************************/
void T__DATA_GRAPH::OscMessage(int MessageId)
{
	switch(State){
		case STATE_SLEEP:
			switch(MESSAGE(MessageId)){
				case MESSAGE_SLEEP:
					break;
				case MESSAGE_RUN:
					State = STATE_WAIT_SKELTONDATA;
					break;
			}
			break;
			
		case STATE_WAIT_SKELTONDATA:
		case STATE_RUN:
			switch(MESSAGE(MessageId)){
				case MESSAGE_SLEEP:
					State = STATE_SLEEP;
					Sound_Morse.pause();
					break;
				case MESSAGE_RUN:
					break;
			}
			break;
	}
}

/******************************
******************************/
void T__DATA_GRAPH::DrawVboset(const ofVec3f& Matrix_ofs, const ofVec3f& Matrix_scale, VBO_SET& vbo_set, int drawMode)
{
	ofSetColor(255, 255, 255, 255);
	
	ofFill();
	glPointSize(1.0);
	glLineWidth(2);
	
	ofPushMatrix();
		ofTranslate(Matrix_ofs);
		ofScale(Matrix_scale.x, Matrix_scale.y, Matrix_scale.z);
		vbo_set.draw(drawMode);
	ofPopMatrix();
}

/******************************
******************************/
void T__DATA_GRAPH::Draw__DataToString(bool *SkipDisp, float DataArray[], int num, const ofVec3f& Matrix_ofs, const ofVec3f& Matrix_1st_ofs, const ofVec3f& Matrix_ofs_step, ofTrueTypeFont& font, bool b_IsHex)
{
	ofSetColor(col_Text_on);
	
	ofPushMatrix();
		ofTranslate(Matrix_ofs);
		
		ofTranslate(Matrix_1st_ofs);
		
		for(int i = 0; i < num; i++){
			if( (SkipDisp != NULL) && (SkipDisp[i] == false)){ // move
				// no disp.
			}else{
				char buf[BUF_SIZE_S];
				if(b_IsHex)	sprintf(buf, "%X", float2hex(DataArray[i]));
				else		sprintf(buf, "%+8.3f", DataArray[i]);
				font.drawString(buf, 0, 0);
			}
			
			ofTranslate(Matrix_ofs_step);
		}
	ofPopMatrix();
}

/******************************
******************************/
void T__DATA_GRAPH::Draw_Rader(const ofVec3f& Matrix_ofs, const float angle, const string& str_Label, const ofColor& ArcCol)
{
	ofPushMatrix();
	ofTranslate(Matrix_ofs);
		/********************
		********************/
		ofSetCircleResolution(100);
		// ofSetCircleResolution(10);
		ofSetColor(col_Graph_Rader_Coord);
		ofNoFill();
		
		ofSetLineWidth(3);
		ofDrawCircle(0, 0, 85);
		// ofDrawCircle(0, 0, 90);
		
		ofSetLineWidth(10);
		ofDrawCircle(0, 0, 76);
		// ofDrawCircle(0, 0, 80);
		
		/********************
		********************/
		ofPushMatrix();
		ofRotate(angle);
			ofSetColor(255, 255, 255, 255);
			glPointSize(1.0);
			glLineWidth(1);
			ofFill();
			
			RaderArc.set_singleColor(ArcCol);
			RaderArc.draw(0, 0);
			
		ofPopMatrix();
		
		/********************
		********************/
		ofSetColor(col_Text_Rader);
		char buf[BUF_SIZE_S];
		sprintf(buf, "%03d", int(angle));
		font_Rader_L.drawString(buf, -23, 9);
		
		font_Rader_S.drawString(str_Label.c_str(), -10, 45);
		
	ofPopMatrix();
}

/******************************
******************************/
void T__DATA_GRAPH::drawToFbo_SendSyphon()
{
	/********************
	********************/
	c_SkipProcessControl++;
	if( b_SkipProcessControl && (c_SkipProcessControl % 2 != 0) ){
		return;
	}
	c_SkipProcessControl = 0;
		
	/********************
	********************/
	ofDisableAlphaBlending();
	ofEnableSmoothing();

	fbo.begin();
	ofClear(0, 0, 0, 0);
	if(State == STATE_RUN){
		ofSetColor(255, 255, 255, 255);
		/********************
		draw center.
		********************/
		if( (RunType == RUN_CENTER) || (RunType == RUN_ALL)){
			DrawVboset(ofVec3f(305, 0, 0), ofVec3f(1, 1, 1), Vboset_ZUPT[GRAPH_R], GL_QUADS);
			DrawVboset(ofVec3f(280, 0, 0), ofVec3f(1, 1, 1), Vboset_ZUPT[GRAPH_L], GL_QUADS);
			
			Draw__DataToString(History_ZUPT[GRAPH_R], History_Center[GRAPH_R], NUM_HISTORY__CENTER, ofVec3f(330, 0, 0), ofVec3f(0, GRAPH__ZUPT__BAR_SPACE, 0), ofVec3f(0, GRAPH__ZUPT__BAR_SPACE, 0), font_center, false);
			Draw__DataToString(History_ZUPT[GRAPH_L], History_Center[GRAPH_L], NUM_HISTORY__CENTER, ofVec3f(215, 0, 0), ofVec3f(0, GRAPH__ZUPT__BAR_SPACE, 0), ofVec3f(0, GRAPH__ZUPT__BAR_SPACE, 0), font_center, false);
		}
		
		ofSetColor(255, 255, 255, 255);
		/********************
		draw side.
		********************/
		if( (RunType == RUN_SIDE) || (RunType == RUN_ALL)){
			DrawVboset(ofVec3f(400, 0, 0), ofVec3f(1, 1, 1), Vboset_HandAngle_Bar[GRAPH_R], GL_QUADS);
			DrawVboset(ofVec3f(200, 600, 0), ofVec3f(-1, -1, 1), Vboset_HandAngle_Bar[GRAPH_L], GL_QUADS);
			
			DrawVboset(ofVec3f(455, 0, 0), ofVec3f(1, 1, 1), Vboset_HandAngle_Line[GRAPH_R], GL_LINE_STRIP);
			DrawVboset(ofVec3f(145, 600, 0), ofVec3f(-1, -1, 1), Vboset_HandAngle_Line[GRAPH_L], GL_LINE_STRIP);
			
			Draw__DataToString(NULL, History_HandAngle[GRAPH_R], NUM_HISTORY__SIDE, ofVec3f(510, 0, 0), ofVec3f(0, GRAPH__HAND_ANGLE__BAR_SPACE, 0), ofVec3f(0, GRAPH__HAND_ANGLE__BAR_SPACE, 0), font_side, true);
			Draw__DataToString(NULL, History_HandAngle[GRAPH_L], NUM_HISTORY__SIDE, ofVec3f(5, 600, 0), ofVec3f(0, 0, 0), ofVec3f(0, -GRAPH__HAND_ANGLE__BAR_SPACE, 0), font_side, true);
			
			Draw_Rader(ofVec3f(496, 505, 0), History_HandAngle[GRAPH_R][NUM_HISTORY__SIDE - 1], "RIGHT HAND", col_GraphArc_R);
			Draw_Rader(ofVec3f(105, 95, 0), History_HandAngle[GRAPH_L][NUM_HISTORY__SIDE - 1], "LEFT HAND", col_GraphArc_L);
		}
		
		/********************
		scan bar
			面白ろ追加機能.
			なので、全て現物合わせ.
			
			fbo上での ofEnableAlphaBlending は、動作controlが実質不可能であるが、
			visual上面白くなればいいだろう.
		********************/
		{
			const float max_y = 410;
			ScanBar_y_R += ScanBar_speed * (now - t_LastDraw);
			if(max_y < ScanBar_y_R) ScanBar_y_R = -50;
			
			ofEnableAlphaBlending();
			// ofSetColor(0, 180, 255, 160);
			ofSetColor(col_GraphArc_R.r, col_GraphArc_R.g, col_GraphArc_R.b, 160);
			// ofDrawRectangle(215, 100, 165, 7);
			ofDrawRectangle(210, ScanBar_y_R, 355, 8);
			ofDisableAlphaBlending();
		}
		{
			ScanBar_y_L += ScanBar_speed * (now - t_LastDraw);
			// L/R で、ストローク量を変えることで、位置関係を少しずつずらす.
			if(fbo.getHeight() + 100 < ScanBar_y_L) ScanBar_y_L = ScanBar_y_L__StartFrom;
			
			ofEnableAlphaBlending();
			ofSetColor(col_GraphArc_L.r, col_GraphArc_L.g, col_GraphArc_L.b, 160);
			// ofDrawRectangle(215, 100, 165, 7);
			ofDrawRectangle(0, ScanBar_y_L, 385, 8);
			ofDisableAlphaBlending();
		}
		
		t_LastDraw = now;
	}
	fbo.end();
	
	/********************
	publish
	********************/
	ofTexture tex = fbo.getTextureReference();
	fbo_TextureSyphonServer.publishTexture(&tex);
}

/******************************
******************************/
void T__DATA_GRAPH::draw_FboToMonitor()
{
	ofSetColor(255, 255, 255, 255);
	fbo.draw(0, 0, fbo.getWidth(), fbo.getHeight());
}

/******************************
******************************/
void T__DATA_GRAPH::CheckStatusOfSound()
{
	Sound_Morse.CheckStatus();
}

