/************************************************************
************************************************************/
#include "T__DataText.h"

/************************************************************
************************************************************/

/******************************
******************************/
T__DATA_TEXT::T__DATA_TEXT()
: t_Change(0.06)
, t_LastRefresh(0)
, State(STATE_SLEEP)
, b_RefreshFbo(false)
, c_SkipProcessControl(0)
// , Sound("sound/WhiteNoise.wav", 0.3, 0.5, 1.0)
, Sound("sound/rain.wav", 0.3, 1.0, 2.0)
, t_LastSkeltonData(0)
{
}

/******************************
******************************/
T__DATA_TEXT::~T__DATA_TEXT()
{
}

/******************************
******************************/
void T__DATA_TEXT::exit()
{
}

/******************************
******************************/
void T__DATA_TEXT::setup()
{
	/********************
	********************/
	// font.load("font/RictyDiminished-Regular.ttf", 15, true, true, true);
	font.load("font/RictyDiminishedDiscord-Bold.ttf", 20, true, true, true);
	
	/********************
	********************/
	for(int i = 0; i < NUM_FBOS; i++){
		fbo[i].allocate(FBO_WIDTH, FBO_HEIGHT, GL_RGBA, 0);
		fbo_masked[i].allocate(FBO_WIDTH, FBO_HEIGHT, GL_RGBA, 0);
		
		char buf[BUF_SIZE_S];
		sprintf(buf, "oF Data text_%d", i);
		fbo_TextureSyphonServer[i].setName(buf);
	}
	
	/********************
	********************/
	img_mask.loadImage( "Data_text/mask.png" );	
	shader.load( "Data_text/mask.vert", "Data_text/mask.frag" );
	
	/********************
	********************/
	Reset();
}

/******************************
******************************/
void T__DATA_TEXT::Reset()
{
	/********************
	********************/
	for(int i = 0; i < NUM_FBOS; i++){
		fbo[i].begin();
		ofClear(0, 0, 0, 0);
		fbo[i].end();
	}

	Dice_Contents();
}

/******************************
******************************/
void T__DATA_TEXT::StateChart()
{
	switch(State){
		case STATE_SLEEP:
			break;
			
		case STATE_WAIT_SKELTONDATA:
			if(FromGolem_FrameDataAll.b_set){
				State = STATE_RUN;
				Sound.play();
				t_LastSkeltonData = now;
			}
			break;
			
		case STATE_RUN:
			if(FromGolem_FrameDataAll.b_set){
				t_LastSkeltonData = now;
			}
			
			if(0.2 < now - t_LastSkeltonData){
				State = STATE_WAIT_SKELTONDATA;
				Sound.pause();
			}
			
			break;
	}
}

/******************************
******************************/
void T__DATA_TEXT::OscMessage(int MessageId)
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
					Reset();
					Sound.pause();
					break;
				case MESSAGE_RUN:
					break;
			}
			break;
	}
}

/******************************
******************************/
void T__DATA_TEXT::Dice_Contents()
{
	SurfaceId = (int)( ((double)rand() / ((double)RAND_MAX + 1)) * NUM_FBOS );
	
	DrawPos.x = ofRandom(0, FBO_WIDTH);
	DrawPos.y = ofRandom(0, FBO_HEIGHT);
}

/******************************
******************************/
void T__DATA_TEXT::update()
{
	/********************
	********************/
	StateChart();
	
	/********************
	********************/
	Sound.update();
	
	/********************
	********************/
	if( (State == STATE_RUN) && (FromGolem_FrameDataAll.b_set) ){
		if( t_Change < (now - t_LastRefresh) ){
			b_RefreshFbo = true;
			t_LastRefresh = now;
			
			Dice_Contents();
		}
	}
}

/******************************
******************************/
void T__DATA_TEXT::CopyContentsToString()
{
	str_Contents = "";
	
	string PartsName[6] = {
		"Pelvis",
		"Head",
		"Hand_R",
		"Hand_L",
		"Foot_R",
		"Foot_L",
	};
	char buf_add[BUF_SIZE_S];
	for(int i = 0; i < 6; i++){
		// sprintf(buf_add, "%7s:%X %X %X\n", PartsName[i].c_str(), float2hex(FromGolem_FrameDataAll.AI1_pos[i].x), float2hex(FromGolem_FrameDataAll.AI1_pos[i].y), float2hex(FromGolem_FrameDataAll.AI1_pos[i].z));
		sprintf(buf_add, "%X %X %X\n", float2hex(FromGolem_FrameDataAll.AI1_pos[i].x), float2hex(FromGolem_FrameDataAll.AI1_pos[i].y), float2hex(FromGolem_FrameDataAll.AI1_pos[i].z));
		str_Contents += buf_add;
	}
}

/******************************
******************************/
void T__DATA_TEXT::drawToFbo_SendSyphon()
{
	/********************
	********************/
	c_SkipProcessControl++;
	if( b_SkipProcessControl && (c_SkipProcessControl % 3 != 0) ){
		return;
	}
	c_SkipProcessControl = 0;

		
	/********************
	********************/
	ofEnableSmoothing();
	
	/********************
	fade out.
	********************/
	ofEnableAlphaBlending();
	ofEnableBlendMode(OF_BLENDMODE_ALPHA); // so that fade in fbo works, need to be alpha(not add).
	// ofEnableBlendMode(OF_BLENDMODE_ADD);
	
	for(int i = 0; i < NUM_FBOS; i++){
		fbo[i].begin();
			ofSetColor(0, 0, 0, 3); // fade out.
			ofFill();
			ofDrawRectangle(0, 0, fbo[i].getWidth(), fbo[i].getHeight());
		fbo[i].end();
	}
	
	/********************
	********************/
	ofDisableAlphaBlending();
	if(b_RefreshFbo){
		fbo[SurfaceId].begin();
			// ofClear(0, 0, 0, 0); // no clear.
			
			CopyContentsToString();
			
			// ofSetColor(255, 255, 255, 180);
			ofSetColor(255, 255, 255, 210);
			font.drawStringAsShapes(str_Contents.c_str(), DrawPos.x, DrawPos.y);
		fbo[SurfaceId].end();
	
		b_RefreshFbo = false;
	}
	
	/********************
	********************/
	ofDisableAlphaBlending();
	
	for(int i = 0; i < NUM_FBOS; i++){
		fbo_masked[i].begin();
		ofClear(0, 0, 0, 0);
		shader.begin();		
	
			shader.setUniformTexture( "texture1", img_mask.getTextureReference(), 1 ); 
			
			ofSetColor( 255, 255, 255 );
			fbo[i].draw( 0, 0 );
	
		shader.end();		
		fbo_masked[i].end();
	}
	
	/********************
	publish
	********************/
	for(int i = 0; i < NUM_FBOS; i++){
		// ofTexture tex = fbo[i].getTextureReference();
		ofTexture tex = fbo_masked[i].getTextureReference();
		fbo_TextureSyphonServer[i].publishTexture(&tex);
	}
}

/******************************
******************************/
void T__DATA_TEXT::draw_FboToMonitor(int id)
{
	ofSetColor(255, 255, 255, 255);
	// fbo[id].draw(0, 0, fbo[id].getWidth(), fbo[id].getHeight());
	fbo_masked[id].draw(0, 0, fbo_masked[id].getWidth(), fbo_masked[id].getHeight());
}

/******************************
******************************/
void T__DATA_TEXT::CheckStatusOfSound()
{
	Sound.CheckStatus();
}





