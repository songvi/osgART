/* -*-c++-*- 
 * 
 * osgART - Augmented Reality ToolKit for OpenSceneGraph
 * 
 * Copyright (C) 2005-2009 Human Interface Technology Laboratory New Zealand
 * Copyright (C) 2010-2013 Raphael Grasset, Julian Looser, Hartmut Seichter
 *
 * This library is open source and may be redistributed and/or modified under
 * the terms of the OpenSceneGraph Public License (OSGPL) version 0.0 or
 * (at your option) any later version.  The full license is in LICENSE file
 * included with this distribution, and on the osgart.org website.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * OpenSceneGraph Public License for more details.
*/

#include <osg/PositionAttitudeTransform>

#include <osgViewer/Viewer>
#include <osgViewer/ViewerEventHandlers>

#include <osgDB/ReadFile>
#include <osgDB/FileUtils>

#include <osgART/Foundation>
#include <osgART/VideoLayer>
#include <osgART/PluginManager>
#include <osgART/VideoGeode>

#include <osgART/Utils>
#include <osgART/GeometryUtils>
#include <osgART/TrackerUtils>
#include <osgART/VideoUtils>

#include <osgART/TrackerCallback>
#include <osgART/TargetCallback>
#include <osgART/TransformFilterCallback>
#include <osgART/ImageStreamCallback>

#include <iostream>
#include <sstream>

class KeyboardEventHandler : public osgGA::GUIEventHandler {
 
protected:
	osg::MatrixTransform* _driveCar;

public:
    KeyboardEventHandler(osg::MatrixTransform* drivecar) : osgGA::GUIEventHandler() {_driveCar=drivecar; }      
 
 
    /**
        OVERRIDE THE HANDLE METHOD:
        The handle() method should return true if the event has been dealt with
        and we do not wish it to be handled by any other handler we may also have
        defined. Whether you return true or false depends on the behaviour you 
        want - here we have no other handlers defined so return true.
    **/
    virtual bool handle(const osgGA::GUIEventAdapter& ea,osgGA::GUIActionAdapter& aa, 
                        osg::Object* obj, osg::NodeVisitor* nv) { 
 
        switch (ea.getEventType()) {
 
 
            /** KEY EVENTS:
                Key events have an associated key and event names.
                In this example, we are interested in keys up/down/right/left arrow
                and space bar.
                If we detect a press then we modify the transformation matrix 
                of the local transform node. **/
            case osgGA::GUIEventAdapter::KEYDOWN: {
 
                switch (ea.getKey()) {
 
                    case osgGA::GUIEventAdapter::KEY_Up: // Move forward 5mm
                        _driveCar->preMult(osg::Matrix::translate(0, -5, 0));
                        return true;
 
                    case osgGA::GUIEventAdapter::KEY_Down: // Move back 5mm
                        _driveCar->preMult(osg::Matrix::translate(0, 5, 0));
                        return true; 
 
                    case osgGA::GUIEventAdapter::KEY_Left: // Rotate 10 degrees left
                        _driveCar->preMult(osg::Matrix::rotate(osg::DegreesToRadians(10.0f),  
                                                       osg::Z_AXIS));
                        return true;
 
                    case osgGA::GUIEventAdapter::KEY_Right: // Rotate 10 degrees right
                        _driveCar->preMult(osg::Matrix::rotate(osg::DegreesToRadians(-10.0f),  
                                                       osg::Z_AXIS));
                        return true;
 
                    case ' ': // Reset the transformation
                       _driveCar->setMatrix(osg::Matrix::identity());
                        return true;
                }
 
                   default: return false;
            }
 
 
        }
    }
};
int main(int argc, char* argv[])  {

	//ARGUMENTS INIT

	//VIEWER INIT

	//create a default viewer
	osgViewer::Viewer viewer;

	//setup default threading mode
	viewer.setThreadingModel(osgViewer::Viewer::SingleThreaded);

	// add relevant handlers to the viewer
	viewer.addEventHandler(new osgViewer::StatsHandler);//stats, press 's'
	viewer.addEventHandler(new osgViewer::WindowSizeHandler);//resize, fullscreen 'f'
	viewer.addEventHandler(new osgViewer::ThreadingHandler);//threading mode, press 't'
	viewer.addEventHandler(new osgViewer::HelpHandler);//help menu, press 'h'

	//AR INIT

	//preload plugins
	//video plugin
	osgART::PluginManager::instance()->load("osgart_video_artoolkit");
	//tracker plugin
	osgART::PluginManager::instance()->load("osgart_tracker_artoolkit");

	// Load a video plugin.
	osg::ref_ptr<osgART::Video> video = dynamic_cast<osgART::Video*>(osgART::PluginManager::instance()->get("osgart_video_artoolkit"));

	// check if an instance of the video stream could be started
	if (!video.valid())
	{
		// Without video an AR application can not work. Quit if none found.
		osg::notify(osg::FATAL) << "Could not initialize video plug-in!" << std::endl;
	}

	// found video - configure now
	osgART::VideoConfiguration* _configvideo = video->getConfiguration();

	// if the configuration is existing
	if (_configvideo)
	{
		// it is possible to configure the plugin before opening it

		//artoolkit2 plugin will generate a default configuration for you
		//if you omit this line
		//here we use the default config file in the artoolkit2 data directory
		_configvideo->config="Data/artoolkit2/WDM_camera.xml";

		//you can also specify configuration file here:
		//_config->deviceconfig = "<?xml version=\"1.0\" encoding=\"UTF-8\"?>"
		//	"<dsvl_input><avi_file use_reference_clock=\"true\" file_name=\"Data\\MyVideo.avi\" loop_avi=\"true\" render_secondary=\"true\">"
		//	"<pixel_format><RGB32/></pixel_format></avi_file></dsvl_input>";

	}

	// Open the video. This will not yet start the video stream but will
	// get information about the format of the video which is essential
	// for connecting a tracker
	// Note: configuration should be defined before opening the video
	video->init();

	osg::ref_ptr<osgART::Tracker> tracker 
		= dynamic_cast<osgART::Tracker*>(osgART::PluginManager::instance()->get("osgart_tracker_artoolkit"));

	if (!tracker.valid())
	{
		// Without tracker an AR application can not work. Quit if none found.
		osg::notify(osg::FATAL) << "Could not initialize tracker plug-in!" << std::endl;

		return -1;

	}

	// found tracker - configure now
	osgART::TrackerConfiguration* _configtracker = tracker->getConfiguration();

	// if the configuration is existing
	if (_configtracker)
	{
		// it is possible to configure the plugin before opening it
		//artoolkit2: no configuration
		_configtracker->config="";
	}

	// get the tracker camera configuration object
	osg::ref_ptr<osgART::CameraConfiguration> cameraconfig = tracker->getOrCreateCameraConfiguration();

	// load a camera configuration file
	if (!cameraconfig->load("data/camera_para.dat")) 
	{

		// the camera configuration file was non-existing or couldnt be loaded
		osg::notify(osg::FATAL) << "Non existing or incompatible camera configuration file" << std::endl;
		exit(-1);
	}

	// setup one target
	osg::ref_ptr<osgART::Target> target = tracker->addTarget("single;data/artoolkit2/patt.hiro;80;0;0");

	target->setActive(true);

	tracker->setImage(video->getStream());

	tracker->init();


	//AR SCENEGRAPH INIT

	//create root 
	osg::ref_ptr<osg::Group> root = new osg::Group;

	//add video update callback (update video stream)
	if (osg::ImageStream* imagestream = dynamic_cast<osg::ImageStream*>(video->getStream())) {
		osgART::addEventCallback(root.get(), new osgART::ImageStreamCallback(imagestream));
	}

	//add tracker update callback (update tracker from video stream)
	osgART::TrackerCallback::addOrSet(root.get(),tracker.get());

	//add a video background
	osg::ref_ptr<osg::Group> videoBackground = osgART::createBasicVideoBackground(video->getStream());
	videoBackground->getOrCreateStateSet()->setRenderBinDetails(0, "RenderBin");

	root->addChild(videoBackground.get());

	//add a virtual camera
	osg::ref_ptr<osg::Camera> cam = osgART::createBasicCamera(cameraconfig);
	root->addChild(cam.get());

	//add a target transform callback (update transform from target information)
	osg::ref_ptr<osg::MatrixTransform> arTransform = new osg::MatrixTransform();
	arTransform->getOrCreateStateSet()->setRenderBinDetails(100, "RenderBin");

	osgART::attachDefaultEventCallbacks(arTransform.get(), target.get());

	cam->addChild(arTransform.get());

	osg::ref_ptr<osg::MatrixTransform> driveCar = new osg::MatrixTransform();
	driveCar->addChild(osgDB::readNodeFile("media/models/car.ive"));
	arTransform->addChild(driveCar.get());

	viewer.addEventHandler(new KeyboardEventHandler(driveCar)); // Our handler

	//APPLICATION INIT

	//BOOTSTRAP INIT
	viewer.setSceneData(root.get());

	viewer.realize();

	//video start
	video->start();

	//tracker start
	tracker->start();


	//MAIN LOOP
	while (!viewer.done()) {
		viewer.frame();
	}

	//EXIT CLEANUP

	//tracker stop
	tracker->stop();

	//video stop
	video->stop();

	//tracker open
	tracker->close();

	//video open
	video->close();

	return 0;
}
