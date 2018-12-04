#include "ofMain.h"
#include "ofApp.h"

//========================================================================
int main( int argc, char** argv ){
	ofSetupOpenGL(1024,768,OF_WINDOW);			// <-------- setup the GL context

	if(argc < 4){
		printf("exe [GolemIP] [MirrorIP] [Golem_id]\n");
		printf("\tGolemIP:127.0.0.1 or 10.0.0.2 or 10.0.0.3 or 10.0.0.4\n");
		printf("\tMirrorIP:127.0.0.1 or 10.0.0.10\n");
		return -1;
		
	}else{
		if( (strcmp(argv[1], "127.0.0.1") != 0) && (strcmp(argv[1], "10.0.0.2") != 0) && (strcmp(argv[1], "10.0.0.3") != 0) && (strcmp(argv[1], "10.0.0.4") != 0) ){
			printf("invalid Golem IP\n");
			return -1;
		}
		
		if( (strcmp(argv[2], "127.0.0.1") != 0) && (strcmp(argv[2], "10.0.0.10") != 0) ){
			printf("invalid Mirror IP\n");
			return -1;
		}
		
		int Golem_id = atoi(argv[3]);
		if( (Golem_id < 0) || (1 < Golem_id) ){
			printf("invalid Golem id\n");
			return -1;
		}
		
		ofRunApp(new ofApp(argv[1], argv[2], Golem_id));
	}
}
