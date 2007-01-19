// ***************************************************************************
//
//   Generated automatically by genwrapper.
//   Please DO NOT EDIT this file!
//
// ***************************************************************************

#include <osgIntrospection/ReflectionMacros>
#include <osgIntrospection/TypedMethodInfo>
#include <osgIntrospection/StaticMethodInfo>
#include <osgIntrospection/Attributes>

#include <osgART/GenericVideo>
#include <osgART/VideoManager>

// Must undefine IN and OUT macros defined in Windows headers
#ifdef IN
#undef IN
#endif
#ifdef OUT
#undef OUT
#endif

// TYPE_NAME_ALIAS(osgART::GenericVideo *(*, osgART::VideoManager::p_VideoCreateFunc);

BEGIN_VALUE_REFLECTOR(osgART::VideoManager)
	I_Method1(int, addVideoStream, IN, osgART::GenericVideo *, video);
	I_Method1(void, removeVideoStream, IN, osgART::GenericVideo *, video);
	I_Method1(osgART::GenericVideo *, getVideo, IN, int, idVideo);
	I_StaticMethod0(osgART::VideoManager *, getInstance);
	I_StaticMethod1(osgART::GenericVideo *, createVideoFromPlugin, IN, const std::string &, plugin);
	I_StaticMethod0(void, destroy);
END_REFLECTOR

