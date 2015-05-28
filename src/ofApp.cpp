#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    ofSetVerticalSync(true);
    ofSetWindowTitle("No Video - FramePlayer");
    ofBackgroundHex(0xFFFFFF);
    
    player.setPixelFormat(OF_PIXELS_RGB);
    player.setSynchronousSeeking(false);
    font.loadFont("Helvetica.dfont", 26);
    
    clipStart = clipEnd = markerTimeStart = markerTimeEnd = 0;
    
    background.loadImage("background.jpg");
}

//--------------------------------------------------------------
void ofApp::update(){
    if (player.isLoaded()) {
        player.update();
        
        if (player.isPlaying())
        {
            posSlider.setPercent((float)player.getCurrentFrame()/(float)(player.getTotalNumFrames()-1));
        }
    }

    imageX = 140*cos(ofGetElapsedTimef()/80*M_PI);
}

//--------------------------------------------------------------
void ofApp::draw()
{
    float xt = -0.005*ofGetMouseX();
    float yt = -0.005*ofGetMouseY();
    
    ofPushMatrix();
    ofTranslate(xt, yt);
    
    ofSetHexColor(0xFFFFFF);
    
    background.draw(imageX - ofGetWidth()/4, 0, ofGetScreenWidth()*1.8, ofGetScreenHeight()*1.8);
    
    ofTranslate(xt, yt);
    
    if (player.isLoaded())
    {
        float leftOffset = ofGetWidth()/2 - player.getWidth()/2;
        
        ofSetHexColor(0x000000);
        ofRect(leftOffset - 5, 15, player.getWidth()+10, player.getHeight()+10);
        
        ofSetHexColor(0xFFFFFF);
        player.draw(leftOffset, 20);
        
        ofSetHexColor(0x000000);
        
        if (player.isPlaying())
        {
            font.drawString("Playing...", leftOffset, 100 + player.getHeight());
        }
        else
        {
            font.drawString("Frame: " + ofToString(player.getCurrentFrame()) + "/" + ofToString(player.getTotalNumFrames()), leftOffset, 100 + player.getHeight());
            font.drawString("Time (s): " + ofToString(player.getPosition()*player.getDuration()) + "/" + ofToString(player.getDuration()), leftOffset, 140 + player.getHeight());
            font.drawString("Marker: " + ofToString(clipStart) + " - " + ofToString(clipEnd) + "\nDelta Time (s): " + ofToString(abs(markerTimeEnd - markerTimeStart)), leftOffset, 180 + player.getHeight());
        }
    }
    else
    {
        ofSetHexColor(0x000000);
        font.drawString("No video loaded\n\n\nDrag a video file", 100, 100);
    }
    
    ofPopMatrix();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key)
{
    if (!player.isLoaded())
        return;
    
    if  (key == ' ')
    {
        if (player.isPlaying()) {
            player.stop();
        }
        else{
            player.play();
        }
    }
    
    if (player.isPlaying()) {
        return;
    }
    
    switch (key)
    {
        case OF_KEY_LEFT:
            player.previousFrame();
            break;
        case OF_KEY_RIGHT:
            player.nextFrame();
            break;
        case '1':
            player.firstFrame();
            break;
        case '0':
            player.setFrame(player.getTotalNumFrames()-1);
            break;
        case OF_KEY_RIGHT_SHIFT:
            clipEnd = player.getCurrentFrame();
            markerTimeEnd = player.getPosition()*player.getDuration();
            break;
        case OF_KEY_LEFT_SHIFT:
            clipStart = player.getCurrentFrame();
            markerTimeStart = player.getPosition()*player.getDuration();
            break;
        case '[':
            player.setFrame(clipStart);
            break;
        case ']':
            player.setFrame(clipEnd);
            break;
        default:
            break;
    }
    
    posSlider.setPercent((float)player.getCurrentFrame()/(float)(player.getTotalNumFrames()-1));
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){
    
}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button)
{
    if (player.isLoaded())
    {
        player.setFrame((int)posSlider.getValue());
    }
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
    
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){
    
}

void ofApp::setupSlider()
{
    posSlider.setup(ofGetWidth()/2 - player.getWidth()/2, 240 + player.getHeight(), player.getWidth(), 20, 0, player.getTotalNumFrames()-1, 0, false, false);
    posSlider.setNumberDisplayPrecision(0);
    posSlider.setPercent((float)player.getCurrentFrame()/(float)(player.getTotalNumFrames()-1));
}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){
    if (player.isLoaded())
    {
        setupSlider();
    }
}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){
    
}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo)
{

    player.loadMovie(dragInfo.files[0], decodeMode);
    string filePath = ofFilePath::getBaseName(dragInfo.files[0]);
    ofSetWindowTitle(filePath + " - FramePlayer");
    
    setupSlider();
    
    clipEnd = player.getTotalNumFrames()-1;
    markerTimeEnd = player.getDuration();
}
