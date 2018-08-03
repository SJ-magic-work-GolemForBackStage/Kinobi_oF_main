/************************************************************
size of
	int		4
	float	4
	double	8
************************************************************/
#pragma once

/************************************************************
************************************************************/
#include "ofMain.h"

#include "ofxOsc_BiDirection.h"
#include "ofxNetwork.h"

#include "sj_common.h"

#include "T__AISensor.h"
#include "T__AIAvatar.h"
#include "T__AIBone.h"
#include "T__Cursor.h"
#include "T__DataGraph.h"
#include "T__DataText.h"
#include "T__Floor.h"


/************************************************************
************************************************************/

class ofApp : public ofBaseApp{
private:
	/****************************************
	****************************************/
	enum TEXTURE_IMAGES{
		TEXTURE__FLOOR,
		TEXTURE__CURSOR,
		TEXTURE__DATA_GRAPH,
		TEXTURE__DATA_TEXT_0,
		TEXTURE__DATA_TEXT_1,
		TEXTURE__DATA_TEXT_2,
		
		TEXTURE__AI_AVATAR,
		TEXTURE__AI_SENSOR,
		TEXTURE__AI_BONE,
		
		
		NUM__TEXTURE_IMAGES,
	};
	
	/****************************************
	****************************************/
	/********************
	********************/
	OSC_TARGET Osc_Golem;
	OSC_TARGET Osc_Unity;
	OSC_TARGET Osc_oF_AmbientSound;
	OSC_TARGET Osc_oF_LiveCam;
	
	ofxUDPManager udp_ReceiveFromGolem;
	ofxUDPManager udp_SendToUnity;
	
	TEXTURE_IMAGES draw_TextureImage_id;
	
	ofTrueTypeFont font;
	
	/********************
	********************/
	T__FLOOR* T_Floor;
	T__AI_AVATAR* T_AiAvatar;
	T__AI_BONE* T_AiBone;
	T__DATA_TEXT* T_DataText;
	T__CURSOR* T_Cursor;
	T__DATA_GRAPH* T_DataGraph;
	
	/****************************************
	****************************************/
	void ResTo_OscFromGolem();
	void ResTo_OscFromUnity();
	
	void ResTo_UdpFromGolem();
	void SaveUdpData_SkeltonData_all(vector<string> &block);
	void update_internal_oFclass__Effected_by_Golem();
	void update_internal_oFclass__independent_of_Golem();
	
	void all_TextureObj__setup();
	void all_TextureObj__drawToFbo_SendSyphon();
	
public:
	/****************************************
	****************************************/
	ofApp();
	~ofApp();
	void exit();
	
	void setup();
	void update();
	void draw();

	void keyPressed(int key);
	void keyReleased(int key);
	void mouseMoved(int x, int y );
	void mouseDragged(int x, int y, int button);
	void mousePressed(int x, int y, int button);
	void mouseReleased(int x, int y, int button);
	void mouseEntered(int x, int y);
	void mouseExited(int x, int y);
	void windowResized(int w, int h);
	void dragEvent(ofDragInfo dragInfo);
	void gotMessage(ofMessage msg);
	
};
