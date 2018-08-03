/************************************************************
************************************************************/
#include "ofApp.h"

/************************************************************
************************************************************/

/******************************
******************************/
ofApp::ofApp()
: Osc_Golem("127.0.0.1", 12350, 12345)
// : Osc_Golem("10.0.0.2", 12350, 12345)
, Osc_Unity("127.0.0.1", 12347, 12346)
, Osc_oF_AmbientSound("127.0.0.1", 12349, 12348)
, Osc_oF_LiveCam("127.0.0.1", 12354, 12353)
, draw_TextureImage_id(TEXTURE__FLOOR)
, T_Floor(T__FLOOR::getInstance())
, T_AiAvatar(T__AI_AVATAR::getInstance())
, T_AiBone(T__AI_BONE::getInstance() )
, T_DataText(T__DATA_TEXT::getInstance() )
, T_Cursor(T__CURSOR::getInstance() )
, T_DataGraph(T__DATA_GRAPH::getInstance() )
{
	/********************
	********************/
	// font.load("font/RictyDiminished-Regular.ttf", 15, true, true, true);
	font.load("font/RictyDiminishedDiscord-Bold.ttf", 15, true, true, true);
}

/******************************
******************************/
ofApp::~ofApp()
{
}

/******************************
******************************/
void ofApp::exit()
{
}

/******************************
******************************/
void ofApp::setup(){
	/********************
	********************/
	srand((unsigned) time(NULL));
	
	/********************
	********************/
	ofSetBackgroundAuto(true);
	
	ofSetWindowTitle("Kinobi");
	// ofSetVerticalSync(true);
	ofSetVerticalSync(false);
	ofSetFrameRate(60);
	ofSetWindowShape(WINDOW_WIDTH, WINDOW_HEIGHT);
	ofSetEscapeQuitsApp(false);
	
	ofEnableAlphaBlending();
	ofEnableBlendMode(OF_BLENDMODE_ALPHA);
	// ofEnableBlendMode(OF_BLENDMODE_ADD);
	// ofEnableSmoothing();
	
	ofSetCircleResolution(60);
	
	/********************
	********************/
	udp_ReceiveFromGolem.Create();
	udp_ReceiveFromGolem.Bind(12351);
	udp_ReceiveFromGolem.SetNonBlocking(true);
	
    udp_SendToUnity.Create();
	udp_SendToUnity.Connect("127.0.0.1",12352);
	udp_SendToUnity.SetNonBlocking(true);
	
	/********************
	********************/
	all_TextureObj__setup();
}

/******************************
******************************/
void ofApp::all_TextureObj__setup()
{
	T_Floor->setup();
	T_AiAvatar->setup();
	T_AiBone->setup();
	T_DataText->setup();
	T_Cursor->setup();
	T_DataGraph->setup();
}

/******************************
******************************/
void ofApp::ResTo_OscFromGolem(){

	while(Osc_Golem.OscReceive.hasWaitingMessages()){
		ofxOscMessage m_receive;
		Osc_Golem.OscReceive.getNextMessage(&m_receive);
		
		if(m_receive.getAddress() == "/Golem/Status"){
			const int NUM_SENSORS = 6;
			
			bool b_ValidMessage = true;
			
			if(m_receive.getNumArgs() != NUM_SENSORS){
				b_ValidMessage = false;
			}else{
				for(int i = 0; i < NUM_SENSORS; i++){
					if(m_receive.getArgType(i) != OFXOSC_TYPE_INT32){
						b_ValidMessage = false;
					}
				}
			}
			
			if(b_ValidMessage){
				ofxOscMessage m;
				m.setAddress("/Golem/Status");
				for(int i = 0; i < NUM_SENSORS; i++) { m.addIntArg(m_receive.getArgAsInt(i)); }
				Osc_Unity.OscSend.sendMessage(m);
			}else{
				printf("/Golem/Status : Envalid format\n");
			}
			
		}else if(m_receive.getAddress() == "/Golem/Message"){
			bool b_ValidMessage = true;
			
			if(m_receive.getNumArgs() != 1){
				b_ValidMessage = false;
			}else{
				if(m_receive.getArgType(0) != OFXOSC_TYPE_INT32){
					b_ValidMessage = false;
				}
			}
			
			if(b_ValidMessage){
				ofxOscMessage m;
				m.setAddress("/Golem/Message");
				m.addIntArg(m_receive.getArgAsInt(0));
				Osc_Unity.OscSend.sendMessage(m);
			}else{
				printf("/Golem/Message : Envalid format\n");
			}
		}
	}
	
}

/******************************
******************************/
void ofApp::ResTo_OscFromUnity(){

	while(Osc_Unity.OscReceive.hasWaitingMessages()){
		ofxOscMessage m_receive;
		Osc_Unity.OscReceive.getNextMessage(&m_receive);
		
		if(m_receive.getAddress() == "/Message/ToGolem"){
			ofxOscMessage m;
			m.setAddress("/Message");
			
			m.addIntArg(int(m_receive.getArgAsFloat(0)));								// message id
			for(int i = 1; i < 11; i++) { m.addFloatArg(m_receive.getArgAsFloat(i)); }	// params
			
			Osc_Golem.OscSend.sendMessage(m);
			
		}else if(m_receive.getAddress() == "/Message/to_oF/Floor"){
			// nothing.
			
		}else if(m_receive.getAddress() == "/Message/to_oF/AI_Sensor"){
			// nothing.
			
		}else if(m_receive.getAddress() == "/Message/to_oF/AI_Avatar"){
			// nothing.
			
		}else if(m_receive.getAddress() == "/Message/to_oF/AI_Bone"){
			// nothing.
			
		}else if(m_receive.getAddress() == "/Message/to_oF/Data_Graph"){
			T_DataGraph->OscMessage(m_receive.getArgAsInt(0));
			
		}else if(m_receive.getAddress() == "/Message/to_oF/Data_Text"){
			T_DataText->OscMessage(m_receive.getArgAsInt(0));
		
		}else if(m_receive.getAddress() == "/Message/to_oF/Cursor"){
			T_Cursor->OscMessage(m_receive.getArgAsInt(0));
		
		}else if(m_receive.getAddress() == "/Message/to_oF_AmbSound/Play"){
			ofxOscMessage m;
			m.setAddress("/Play");
			m.addIntArg(0); // dummy.
			Osc_oF_AmbientSound.OscSend.sendMessage(m);
		
		}else if(m_receive.getAddress() == "/Message/to_oF_AmbSound/Stop"){
			ofxOscMessage m;
			m.setAddress("/Stop");
			m.addIntArg(0); // dummy.
			Osc_oF_AmbientSound.OscSend.sendMessage(m);
		
		}else if(m_receive.getAddress() == "/Message/Quit"){
			ofxOscMessage m;
			m.setAddress("/Quit");
			m.addIntArg(0); // dummy.
			Osc_oF_AmbientSound.OscSend.sendMessage(m);
			
		}else if(m_receive.getAddress() == "/Message/to_oF_LiveCam/PlayStops"){
			ofxOscMessage m;
			m.setAddress("/PlayStop");
			m.addIntArg(m_receive.getArgAsInt(0));
			Osc_oF_LiveCam.OscSend.sendMessage(m);
		}
	}
}

/******************************
******************************/
void ofApp::ResTo_UdpFromGolem(){

	bool b_MessageReceived;
	
	do{
		char udpMessage[BUF_SIZE_UDP];
		udp_ReceiveFromGolem.Receive(udpMessage, BUF_SIZE_UDP);
		
		string message = udpMessage;
		
		if(message!=""){
			b_MessageReceived = true;
			
			vector<string> block = ofSplitString(message,"<p>");
			
			if(block[0] == "/Golem/SkeletonDefinition"){
				udp_SendToUnity.Send(message.c_str(), message.length()); // bypass
				
			}else if(block[0] == "/Golem/SkeletonData"){
				udp_SendToUnity.Send(message.c_str(), message.length()); // bypass
				FromGolem_FrameDataAll.set(block);
			}
			
		}else{
			b_MessageReceived = false;
		}
		
	}while(b_MessageReceived);
}

/******************************
******************************/
void ofApp::update_internal_oFclass__Effected_by_Golem(){
	/********************
	if(FromGolem_FrameDataAll.b_set){
	}else{
	}
	
	で各class、分岐があるので、全てupdateする。
	********************/
}

/******************************
******************************/
void ofApp::update_internal_oFclass__independent_of_Golem(){
	T_Floor->update();
	T_AiAvatar->update();
	T_AiBone->update();
	
	T_DataText->update();
	T_Cursor->update();
	T_DataGraph->update();
}

/******************************
******************************/
void ofApp::update(){
	/********************
	********************/
	ofSoundUpdate(); // 全体まとめて.
	
	/********************
	********************/
	now = ofGetElapsedTimef();
	
	/********************
	********************/
	ResTo_OscFromGolem();
	ResTo_OscFromUnity();
	
	/********************
	********************/
	ResTo_UdpFromGolem();
	
	/********************
	********************/
	update_internal_oFclass__Effected_by_Golem();
	update_internal_oFclass__independent_of_Golem();
	
	/********************
	********************/
	FromGolem_FrameDataAll.b_set = false;
}

/******************************
******************************/
void ofApp::all_TextureObj__drawToFbo_SendSyphon()
{
	T_Floor->drawToFbo_SendSyphon();
	T_AiAvatar->drawToFbo_SendSyphon();
	T_AiBone->drawToFbo_SendSyphon();
	T_DataText->drawToFbo_SendSyphon();
	T_Cursor->drawToFbo_SendSyphon();
	T_DataGraph->drawToFbo_SendSyphon();
}

/******************************
******************************/
void ofApp::draw(){
	/********************
	********************/
	// Clear with alpha, so we can capture via syphon and composite elsewhere should we want.
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	/********************
	each class : draw to fbo, and send via syohon.
	********************/
	all_TextureObj__drawToFbo_SendSyphon();
	
	/********************
	monitor.
	********************/
	ofEnableAlphaBlending();
	ofEnableBlendMode(OF_BLENDMODE_ALPHA);
	// ofEnableBlendMode(OF_BLENDMODE_ADD);
	ofEnableSmoothing();
	
	ofSetColor(255, 255, 255, 255);
	switch(draw_TextureImage_id){
		case TEXTURE__FLOOR:
			T_Floor->draw_FboToMonitor();
			break;
			
		case TEXTURE__CURSOR:
			T_Cursor->draw_FboToMonitor();
			break;
			
		case TEXTURE__DATA_GRAPH:
			T_DataGraph->draw_FboToMonitor();
			break;
			
		case TEXTURE__DATA_TEXT_0:
			T_DataText->draw_FboToMonitor(0);
			break;
			
		case TEXTURE__DATA_TEXT_1:
			T_DataText->draw_FboToMonitor(1);
			break;
			
		case TEXTURE__DATA_TEXT_2:
			T_DataText->draw_FboToMonitor(2);
			break;
			
		case TEXTURE__AI_AVATAR:
			T_AiAvatar->draw_FboToMonitor();
			break;
			
		case TEXTURE__AI_SENSOR:
			break;
			
		case TEXTURE__AI_BONE:
			T_AiBone->draw_FboToMonitor();
			break;
			
		default:
			break;
	}
	
	/********************
	********************/
	char buf[BUF_SIZE_S];
	sprintf(buf, "%4d", int(ofGetFrameRate() + 0.5));
	ofSetColor(255, 255, 255, 210);
	font.drawStringAsShapes(buf, 800, 20);
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
	switch(key){
		case '0':
		case '1':
		case '2':
		case '3':
		case '4':
		case '5':
		case '6':
		case '7':
		case '8':
		case '9':
		{
			int id = key - '0';
			if(id < NUM__TEXTURE_IMAGES){
				draw_TextureImage_id = (TEXTURE_IMAGES)id;
			}
		}
			break;
		
		case 'b':
			/*
			b_SkipProcessControl = !b_SkipProcessControl;
			printf("b_SkipProcessControl = %d\n", b_SkipProcessControl);
			*/
			break;
			
		case 'r':
			T_DataText->OscMessage(1);
			T_Cursor->OscMessage(1);
			T_DataGraph->OscMessage(1);
			break;
			
		case 's':
			T_DataText->OscMessage(0);
			T_Cursor->OscMessage(0);
			T_DataGraph->OscMessage(0);
			break;
			
		case ' ':
			ofSaveScreen("image.png");
			printf("image saved\n");
			break;
			
		case 'c':
			T_DataText->CheckStatusOfSound();
			T_DataGraph->CheckStatusOfSound();
			break;
	}
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
