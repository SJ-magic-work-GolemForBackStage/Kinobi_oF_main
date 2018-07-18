/************************************************************
************************************************************/
#include "T__AIBone.h"

/************************************************************
************************************************************/

/******************************
******************************/
T__AI_BONE::T__AI_BONE()
// : t_Change(0.1)
: t_Change(0.25)
, t_LastRefresh(0)
, c_SkipProcessControl(0)
{
}

/******************************
******************************/
T__AI_BONE::~T__AI_BONE()
{
}

/******************************
******************************/
void T__AI_BONE::exit()
{
}

/******************************
******************************/
void T__AI_BONE::setup()
{
	/********************
	********************/
	// font.load("font/RictyDiminishedDiscord-Bold.ttf", 60, true, true, true);
	font.load("font/RictyDiminishedDiscord-Bold.ttf", 75, true, true, true);
	
	/********************
	********************/
	fbo.allocate(FBO_WIDTH, FBO_HEIGHT, GL_RGBA, 4);
	fbo_TextureSyphonServer.setName("oF AI Bone");
	
	/********************
	********************/
	init_Contents();
}

/******************************
******************************/
void T__AI_BONE::update()
{
	if(t_Change < (now - t_LastRefresh)){
		Refresh_Contents();
	}
}

/******************************
******************************/
void T__AI_BONE::init_Contents()
{
	for(int i = 0; i < NUM_DIGITS_CONTENTS; i++){
		DigitsContents[i] = (int)( ((double)rand() / ((double)RAND_MAX + 1)) * 2 );
	}
	
	CopyContentsToString();
}

/******************************
******************************/
void T__AI_BONE::Refresh_Contents()
{
	for(int i = 0; i < NUM_DIGITS_CONTENTS - 1; i++){
		DigitsContents[i] = DigitsContents[i + 1];
	}
	
	DigitsContents[NUM_DIGITS_CONTENTS - 1] = (int)( ((double)rand() / ((double)RAND_MAX + 1)) * 2 );
	
	CopyContentsToString();
}

/******************************
******************************/
void T__AI_BONE::CopyContentsToString()
{
	str_DigitsContents = "";
	
	char buf_add[BUF_SIZE_S];
	for(int i = 0; i < NUM_DIGITS_CONTENTS; i++){
		if( (i != 0) && (i % LINE_FEED_PER == 0) ){
			str_DigitsContents += "\n";
		}
		sprintf(buf_add, "%d", DigitsContents[i]);
		str_DigitsContents += buf_add;
	}
	
	t_LastRefresh = now;
}

/******************************
******************************/
void T__AI_BONE::drawToFbo_SendSyphon()
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
	ofDisableAlphaBlending();
	ofEnableSmoothing();
	
	fbo.begin();
	ofClear(0, 0, 0, 0);
		ofPushMatrix();
		ofRotate(-90);
		ofTranslate(-fbo.getWidth(), 0);
			/* */
			ofSetColor(0, 120, 255, 150);
			const int LineWidth = 60;
			const int step = LineWidth + 10;
			ofSetLineWidth(60);
			for(int y = 0; y < fbo.getHeight(); y+=step){
				ofDrawLine(0, y, fbo.getWidth(), y);
			}
			
			/* */
			// ofSetColor(255, 255, 255, 240);
			ofSetColor(0, 120, 255, 230);
			int ofs_x = 0;
			int ofs_y = 40;
			font.drawStringAsShapes(str_DigitsContents.c_str(), ofs_x, font.stringHeight("0"));
		ofPopMatrix();
	fbo.end();
	
	/********************
	publish
	********************/
	ofTexture tex = fbo.getTextureReference();
	fbo_TextureSyphonServer.publishTexture(&tex);
}

/******************************
******************************/
void T__AI_BONE::draw_FboToMonitor()
{
	ofSetColor(255, 255, 255, 255);
	fbo.draw(0, 0, fbo.getWidth(), fbo.getHeight());
}





