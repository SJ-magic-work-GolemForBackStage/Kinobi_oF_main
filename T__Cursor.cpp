/************************************************************
************************************************************/
#include "T__Cursor.h"

/************************************************************
************************************************************/

/******************************
******************************/
T__CURSOR::T__CURSOR()
: State(STATE_SLEEP)
, LastId_ofTarget(0)
, Speed(10) // temporary.
, t_LastSkeltonData(0)
, c_SkipProcessControl(0)
, t_Last_Sound_pi(0)
{
}

/******************************
******************************/
T__CURSOR::~T__CURSOR()
{
}

/******************************
******************************/
void T__CURSOR::exit()
{
}

/******************************
******************************/
void T__CURSOR::setup()
{
	/********************
	********************/
	fbo.allocate(FBO_WIDTH, FBO_HEIGHT, GL_RGBA, 0);
	fbo_TextureSyphonServer.setName("oF Cursor");
	
	load_and_set_sound();
}

/******************************
******************************/
void T__CURSOR::Reset(bool b_1st)
{
	/********************
	********************/
	int NextId = (int)( ((double)rand() / ((double)RAND_MAX + 1)) * NUM_SENSORS );
	if(!b_1st && (NextId == LastId_ofTarget)){
		NextId++;
		if(NUM_SENSORS <= NextId) NextId = 0;
	}
	LastId_ofTarget = NextId;
	
	/********************
	********************/
	if(b_1st){
		history[0].set(0, 0);
		history[1].set(fbo.getWidth() - 1, fbo.getHeight() - 1);
	}else{
		for(int i = 0; i < NUM_HISTORY - 1; i++){
			history[i] = history[i + 1];
		}
	}
	
	/********************
	********************/
	/*
	history[2].x = ofMap(-FromGolem_FrameDataAll.INS_pos[NextId].x, UNITY_CURSOR_SURFACE_SIZE_W/2, -UNITY_CURSOR_SURFACE_SIZE_W/2, 0, fbo.getWidth(), true);
	history[2].y = ofMap( FromGolem_FrameDataAll.INS_pos[NextId].z, -UNITY_CURSOR_SURFACE_SIZE_H/2, UNITY_CURSOR_SURFACE_SIZE_H/2, 0, fbo.getHeight(), true);
	*/
	
	float Range = 1.0;
	history[2].x = ofMap(-FromGolem_FrameDataAll.INS_pos[NextId].x, Range, -Range, 0, fbo.getWidth(), true); // x:反転
	history[2].y = ofMap( FromGolem_FrameDataAll.INS_pos[NextId].z, -Range, Range, 0, fbo.getHeight(), true);
	/*
	history[2].x = ofRandom(0, fbo.getWidth());
	history[2].y = ofRandom(0, fbo.getHeight());
	*/
	
	/********************
	********************/
	CursorPos = history[1];
	
	/********************
	********************/
	MovDirection = history[2] - CursorPos;
	float length = MovDirection.length();
	
	MovDirection.normalize();
	
	
	const float Diag = FBO_WIDTH * 1.4; // assume:正方形
	// const float time = 3.0; // 所望の長さを移動する時間.
	const float time = 4.0; // 所望の長さを移動する時間.
	if( length < (Diag*1/3) )		Speed = Diag*1/3/time;
	else if( length < (Diag*2/3) )	Speed = Diag*2/3/time;
	else							Speed = Diag*3/3/time;
	
	/********************
	********************/
	t_LastRefresh = now;
}

/******************************
******************************/
void T__CURSOR::OscMessage(int MessageId)
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
					break;
				case MESSAGE_RUN:
					break;
			}
			break;
	}
}

/******************************
******************************/
void T__CURSOR::StateChart()
{
	switch(State){
		case STATE_SLEEP:
			break;
			
		case STATE_WAIT_SKELTONDATA:
			if(FromGolem_FrameDataAll.b_set){
				State = STATE_RUN;
				Reset(true);
				t_LastSkeltonData = now;
			}
			break;
			
		case STATE_RUN:
			if(FromGolem_FrameDataAll.b_set){
				t_LastSkeltonData = now;
			}
			if(0.2 < now - t_LastSkeltonData){
				State = STATE_WAIT_SKELTONDATA;
			}
			break;
	}
}

/******************************
******************************/
void T__CURSOR::load_and_set_sound()
{
	Sound_pi.loadSound("sound/pi_1.mp3");
	Sound_pi.setLoop(false);
	Sound_pi.setMultiPlay(true);
	Sound_pi.setVolume(0.2);
}

/******************************
******************************/
void T__CURSOR::update()
{
	/********************
	********************/
	ofSoundUpdate();
	
	/********************
	********************/
	StateChart();
	
	/********************
	********************/
	if(State == STATE_RUN){
		ofVec2f VecLeft = history[2] - CursorPos;
		float distance = VecLeft.length();
		if(distance < Speed * (now - t_LastRefresh)){
			Reset(false);
			
			if(1.0 < now - t_Last_Sound_pi){ // 近すぎてnoisyになるのを避ける.
				Sound_pi.play();
				t_Last_Sound_pi = now;
			}
			
		}else{
			CursorPos += Speed * (now - t_LastRefresh) * MovDirection;
		}
		
		t_LastRefresh = now;
	}
}

/******************************
******************************/
void T__CURSOR::drawCursor(const ofVec2f& pos, const ofColor& col)
{
	ofSetColor(col);
	ofSetLineWidth(2);
	ofDrawLine(0, pos.y, fbo.getWidth(), pos.y);
	ofDrawLine(pos.x, 0, pos.x, fbo.getHeight());
}

/******************************
******************************/
void T__CURSOR::drawToFbo_SendSyphon()
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
	ofSetCircleResolution(60);
	
	fbo.begin();
		ofClear(0, 0, 0, 0);
		
		if(State == STATE_RUN){
			/*
			drawCursor(history[0], ofColor(255, 130));
			drawCursor(history[1], ofColor(255, 190));
			drawCursor(CursorPos, ofColor(255, 230));
			*/
			drawCursor(history[0], ofColor(255, 100));
			drawCursor(history[1], ofColor(255, 130));
			drawCursor(CursorPos, ofColor(255, 170));
			
			/*
			ofSetColor(255, 130);
			ofDrawCircle(history[2].x, history[2].y, 3);
			*/
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
void T__CURSOR::draw_FboToMonitor()
{
	ofSetColor(255, 255, 255, 255);
	fbo.draw(0, 0, fbo.getWidth(), fbo.getHeight());
}





