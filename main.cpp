#include "ofMain.h"
#include "ofApp.h"

//========================================================================
int main( int argc, char** argv ){
	ofSetupOpenGL(1024,768,OF_WINDOW);			// <-------- setup the GL context

	if(argc < 3){
		printf("exe [BootMode] [Golem_id]\n");
		printf("\tBootMode\n");
		printf("\t0:Alone\n");
		printf("\t1:Mirror\n");
		
		return -1;
		
	}else{
		int BootMode = atoi(argv[1]);
		if( (BootMode < 0) || (1 < BootMode) ){
			printf("invalid BootMode\n");
			return -1;
		}
		
		int Golem_id = atoi(argv[2]);
		if( (Golem_id < 0) || (1 < Golem_id) ){
			printf("invalid Golem id\n");
			return -1;
		}
		
		ofRunApp(new ofApp(BootMode, Golem_id));
	}
}
