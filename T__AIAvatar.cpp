/************************************************************
************************************************************/
#include "T__AIAvatar.h"

/************************************************************
************************************************************/

/******************************
******************************/
T__AI_AVATAR::T__AI_AVATAR()
: image_ofs_y(0)
, shift_y_ratio_per_sec(0.2)
, t_LastINT(0)
, c_SkipProcessControl(0)
{
}

/******************************
******************************/
T__AI_AVATAR::~T__AI_AVATAR()
{
}

/******************************
******************************/
void T__AI_AVATAR::exit()
{
}

/******************************
******************************/
void T__AI_AVATAR::setup()
{
	image.loadImage("texture/AI_Avatart.png");
	
	fbo.allocate(image.getWidth(), image.getHeight(), GL_RGBA, 0);
	fbo_TextureSyphonServer.setName("oF AI Avatar");
	
	
	shift_y_per_sec = shift_y_ratio_per_sec * image.getWidth();
}

/******************************
******************************/
void T__AI_AVATAR::update()
{
	image_ofs_y -= (now - t_LastINT) * shift_y_per_sec;
	if(image_ofs_y < -fbo.getHeight()){
		image_ofs_y = 0;
	}
	
	t_LastINT = now;
}

/******************************
******************************/
void T__AI_AVATAR::drawToFbo_SendSyphon()
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
	ofSetColor(255, 255, 255, 255);
		ofPushMatrix();
			ofRotate(1);
			ofTranslate(0, image_ofs_y);
			image.draw(0, 0, fbo.getWidth(), fbo.getHeight());
			image.draw(0, fbo.getHeight(), fbo.getWidth(), fbo.getHeight());
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
void T__AI_AVATAR::draw_FboToMonitor()
{
	ofSetColor(255, 255, 255, 255);
	fbo.draw(0, 0, fbo.getWidth(), fbo.getHeight());
}



