/************************************************************
************************************************************/
#include "T__Floor.h"

/************************************************************
************************************************************/

/******************************
******************************/
T__FLOOR::T__FLOOR()
: c_SkipProcessControl(0)
{
}

/******************************
******************************/
T__FLOOR::~T__FLOOR()
{
}

/******************************
******************************/
void T__FLOOR::exit()
{
}

/******************************
******************************/
void T__FLOOR::setup()
{
	fbo.allocate(FBO_WIDTH, FBO_HEIGHT, GL_RGBA, 0);
	fbo_TextureSyphonServer.setName("oF Floor");
}

/******************************
******************************/
void T__FLOOR::update()
{
}

/******************************
******************************/
void T__FLOOR::drawToFbo_SendSyphon()
{
	/********************
	********************/
	c_SkipProcessControl++;
	if( b_SkipProcessControl && (c_SkipProcessControl % 10 != 0) ){
		return;
	}
	c_SkipProcessControl = 0;

		
	/********************
	********************/
	ofDisableAlphaBlending();
	ofEnableSmoothing();
	
	fbo.begin();
	ofClear(0, 0, 0, 0);
		/*
		// 2m
		ofSetColor(255, 255, 255, 200);
		ofSetLineWidth(4);
		ofDrawLine(0, fbo.getHeight()/2, fbo.getWidth(), fbo.getHeight()/2);
		ofDrawLine(fbo.getWidth()/2, 0, fbo.getWidth()/2, fbo.getHeight());
		/*/
		
		// 1m
		ofSetColor(255, 255, 255, 170);
		ofSetLineWidth(2);
		/*
		ofSetColor(255, 255, 255, 120);
		ofSetLineWidth(600);
		*/
		ofDrawLine(0, fbo.getHeight()*1/4, fbo.getWidth(), fbo.getHeight()*1/4);
		ofDrawLine(0, fbo.getHeight()*3/4, fbo.getWidth(), fbo.getHeight()*3/4);
		ofDrawLine(fbo.getWidth()*1/4, 0, fbo.getWidth()*1/4, fbo.getHeight());
		ofDrawLine(fbo.getWidth()*3/4, 0, fbo.getWidth()*3/4, fbo.getHeight());
		//*/
	fbo.end();
	
	/********************
	publish
	********************/
	ofTexture tex = fbo.getTextureReference();
	fbo_TextureSyphonServer.publishTexture(&tex);
}

/******************************
******************************/
void T__FLOOR::draw_FboToMonitor()
{
	ofSetColor(255, 255, 255, 255);
	fbo.draw(0, 0, fbo.getWidth(), fbo.getHeight());
	// fbo.draw(0, 0, ofGetWidth(), ofGetHeight());
}



