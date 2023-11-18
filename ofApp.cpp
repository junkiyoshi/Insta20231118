#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {

	ofSetFrameRate(25);
	ofSetWindowTitle("openframeworks");

	ofBackground(0);
	ofNoFill();
	ofEnableBlendMode(ofBlendMode::OF_BLENDMODE_ADD);
}

//--------------------------------------------------------------
void ofApp::update() {

	int base_radius = (ofGetFrameNum() * 2) % 420 + 60;

	for (int i = this->log_list.size() - 1; i >= 0; i--) {

		if (glm::length(this->log_list[i].back()) > 720) {

			this->log_list.erase(this->log_list.begin() + i);
			this->seed_list.erase(this->seed_list.begin() + i);
			this->velocity_list.erase(this->velocity_list.begin() + i);
			this->color_list.erase(this->color_list.begin() + i);

			continue;
		}

		auto feature = this->velocity_list[i] * 80;
		auto deg = ofMap(ofNoise(this->seed_list[i], ofGetFrameNum() * 0.015), 0, 1, 0, 360);
		feature += glm::vec2(30 * cos(deg * DEG_TO_RAD), 30 * sin(deg * DEG_TO_RAD));
		auto next = this->log_list[i].back() + glm::normalize(feature) * 15;

		this->log_list[i].push_back(next);

		while (this->log_list[i].size() > 15) {

			this->log_list[i].erase(this->log_list[i].begin());
		}
	}

	ofColor color;
	for (int i = 0; i < 4; i++) {

		int param = ofGetFrameNum();
		int next_param = param + 1;
		int len = ofRandom(345, 355);

		auto location = this->make_point(len, param);
		auto next = this->make_point(len, next_param);

		vector<glm::vec2> log;
		log.push_back(location);
		this->log_list.push_back(log);

		this->seed_list.push_back(ofMap(ofNoise(location * 0.001), 0, 1, 0, 1000));

		this->velocity_list.push_back(glm::normalize(location  - next));
		
		color.setHsb(ofRandom(0, 30), 200, 255);
		this->color_list.push_back(color);
	}
}

//--------------------------------------------------------------
void ofApp::draw() {

	ofTranslate(ofGetWindowSize() * 0.5);
	
	ofSetLineWidth(1.5);

	for (int i = 0; i < this->log_list.size(); i++) {

		ofSetColor(this->color_list[i]);

		ofBeginShape();
		ofVertices(this->log_list[i]);
		ofEndShape();
	}

	ofSetColor(128);
	ofSetLineWidth(0.5);
	ofDrawRectangle(-175, -175, 350, 350);

	/*
	int start = 150;
	if (ofGetFrameNum() > start) {

		ostringstream os;
		os << setw(4) << setfill('0') << ofGetFrameNum() - start;
		ofImage image;
		image.grabScreen(0, 0, ofGetWidth(), ofGetHeight());
		image.saveImage("image/cap/img_" + os.str() + ".jpg");
		if (ofGetFrameNum() - start >= 25 * 20) {

			std::exit(1);
		}
	}
	*/
}

//--------------------------------------------------------------
glm::vec2 ofApp::make_point(int len, int param) {

	param = param % 100;
	if (param < 25) {

		return glm::vec2(ofMap(param, 0, 25, -len * 0.5, len * 0.5), -len * 0.5);
	}
	else if (param < 50) {

		return glm::vec2(len * 0.5, ofMap(param, 25, 50, -len * 0.5, len * 0.5));
	}
	else if (param < 75) {

		return glm::vec2(ofMap(param, 50, 75, len * 0.5, -len * 0.5), len * 0.5);
	}
	else {

		return glm::vec2(-len * 0.5, ofMap(param, 75, 100, len * 0.5, -len * 0.5));
	}
}


//--------------------------------------------------------------
int main() {

	ofSetupOpenGL(720, 720, OF_WINDOW);
	ofRunApp(new ofApp());
}