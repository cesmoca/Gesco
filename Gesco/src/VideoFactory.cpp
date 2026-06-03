/*
 * VideoFactory.cpp
 *
 *  Created on: 20/02/2011
 *      Author: cesar
 */

#include "VideoFactory.h"

VideoFactory::VideoFactory() {
	_mainCamera = NULL;
}

VideoSource& VideoFactory::getMainCamera(){
	if(_mainCamera==NULL)
		throw "Not main camera found exception!";

	return *_mainCamera;
}

VideoSource& VideoFactory::getCamera(const std::string& name){
	for(unsigned int i=0;i<_videoSources.size();i++){
		if(_videoSources.at(i)->getName()==name)
			return *_videoSources.at(i);
	}

	throw "No camera found with name "+name;
}

void VideoFactory::setMainCamera(const std::string& name){
	Logger::getInstance()->out("Probar VideoFactory::setMainCamera!");

	for(unsigned int i=0;i<_videoSources.size();i++)
		if(_videoSources.at(i)->getName()==name)
			_mainCamera = _videoSources.at(i);

}

void VideoFactory::addVideoSource(const std::string& name, const int& nDevice){
	VideoSource* vs;
	try{
		vs = new VideoSource(name,nDevice);

	}catch(char const* e){
		throw e;
	}
	_videoSources.push_back(vs);

	if(_mainCamera==NULL) /* Setting up the main camera */
		_mainCamera = vs;
}

VideoFactory::~VideoFactory() {
	std::cout << "Destroying VideoFactory..." << std::endl;

	for(unsigned int i=0;i<_videoSources.size();i++)
		delete _videoSources.at(i);

}
