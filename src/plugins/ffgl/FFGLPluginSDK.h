//
// Copyright (c) 2004 - InfoMus Lab - DIST - University of Genova
//
// InfoMus Lab (Laboratorio di Informatica Musicale)
// DIST - University of Genova 
//
// http://www.infomus.dist.unige.it
// news://infomus.dist.unige.it
// mailto:staff@infomus.dist.unige.it
//
// Developer: Gualtiero Volpe
// mailto:volpe@infomus.dist.unige.it
//
// Developer: Trey Harrison
// www.harrisondigitalmedia.com
//
// Last modified: October 26 2006
//

#ifndef FFGLPLUGINSDK_STANDARD
#define FFGLPLUGINSDK_STANDARD

#include "FFGLPluginManager.h"
#include "FFGLPluginInfo.h"

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// \class		CFreeFrameGLPlugin
///	\brief		CFreeFrameGLPlugin is the base class for all FreeFrameGL plugins developed with the FreeFrameGL SDK. 
/// \author		Gualtiero Volpe
/// \version	1.0.0.2
///
/// The CFreeFrameGLPlugin class is the base class for every FreeFrameGL plugins developed with the FreeFrameGL SDK. 
/// It is derived from CFFGLPluginManager, so that most of the plugin management and communication with the host 
/// can be transparently handled through the default implementations of the methods of CFFGLPluginManager. 
/// While CFFGLPluginManager is used by the global FreeFrame methods, CFreeFrameGLPlugin provides a default implementation 
/// of the instance specific FreeFrame functions. Note that CFreeFrameGLPlugin methods are virtual methods: any given 
/// FreeFrameGL plugin developed with the FreeFrameGL SDK will be a derived class of CFreeFrameGLPlugin and will have to 
/// provide a custom implementation of most of such methods. Except for CFreeFrameGLPlugin::GetParameterDisplay and 
/// CFreeFrameGLPlugin::GetInputStatus, all the default methods of CFreeFrameGLPlugin just return FF_FAIL: every derived 
/// plugin is responsible of providing its specific implementation of such default methods.
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class CFreeFrameGLPlugin :
public CFFGLPluginManager
{
public:

	/// The standard destructor of CFreeFrameGLPlugin.
	virtual ~CFreeFrameGLPlugin();

  /// Default implementation of the FFGL InitGL instance specific function. This function allocates
  /// the OpenGL resources the plugin needs during its lifetime
	///
	/// \param		vp Pointer to a FFGLViewportStruct structure (see the definition in FFGL.h and 
	///						the description in the FFGL specification).
	/// \return		The default implementation always returns FF_SUCCESS. 
	///						A custom implementation must be provided by every specific plugin that allocates
  ///           any OpenGL resources
  virtual DWORD InitGL(const FFGLViewportStruct *vp) { return FF_SUCCESS; }

  /// Default implementation of the FFGL DeInitGL instance specific function. This function frees
  /// any OpenGL resources the plugin has allocated
	///
	/// \return		The default implementation always returns FF_SUCCESS. 
	///						A custom implementation must be provided by every specific plugin that allocates
  ///           any OpenGL resources
  virtual DWORD DeInitGL() { return FF_SUCCESS; }

	/// Default implementation of the FreeFrame getParameterDisplay instance specific function. It provides a string 
	/// to display as the value of the plugin parameter whose index is passed as parameter to the method. This default 
	/// implementation just returns the string representation of the float value of the plugin parameter. A custom 
	/// implementation may be provided by every specific plugin.
	///
	/// \param		dwIndex		The index of the parameter whose display value is queried. 
	///							It should be in the range [0, Number of plugin parameters).
	/// \return					The display value of the plugin parameter or NULL in case of error
	virtual char* GetParameterDisplay(DWORD dwIndex);

	/// Default implementation of the FreeFrame setParameter instance specific function. It allows setting the current 
	/// value of the plugin parameter whose index is passed as parameter to the method. This default implementation 
	/// always returns FF_FAIL. A custom implementation must be provided by every specific plugin.
	///
	/// \param		pParam		A pointer to a SetParameterStruct (see FreeFrame.h and FreeFrame specification for 
	///							further information) containing the index and the new value of the plugin parameter 
	///							whose value is going to be set. The parameter index should be in the range 
	///							[0, Number of plugin parameters). 
	/// \return					The default implementation always returns FF_FAIL. 
	///							A custom implementation must be provided.
	virtual DWORD SetParameter(const SetParameterStruct* pParam);
	
	/// Default implementation of the FreeFrame getParameter instance specific function. It allows getting the current 
	/// value of the plugin parameter whose index is passed as parameter to the method. This default implementation 
	/// always returns FF_FAIL. A custom implementation must be provided by every specific plugin.
	///
	/// \param		dwIndex		The index of the parameter whose current value is queried.
	///							It should be in the range [0, Number of plugin parameters).
	/// \return					The default implementation always returns FF_FAIL. 
	///							A custom implementation must be provided by every specific plugin
	virtual DWORD GetParameter(DWORD dwIndex);
	
	/// Default implementation of the FFGL ProcessOpenGL instance specific function. This function processes 
	/// the input texture(s) by 
	///
	/// \param		pOpenGLData to a ProcessOpenGLStruct structure (see the definition in FFGL.h and 
	///						the description in the FFGL specification).
	/// \return		The default implementation always returns FF_FAIL. 
	///						A custom implementation must be provided by every specific plugin.
  virtual DWORD ProcessOpenGL(ProcessOpenGLStruct* pOpenGLData) { return FF_FAIL; }

 	/// Default implementation of the FFGL SetTime instance specific function
	///
	/// \param		pOpenGLData to a ProcessOpenGLStruct structure (see the definition in FFGL.h and 
	///						the description in the FFGL specification).
	/// \return		The default implementation always returns FF_FAIL. 
	///						A custom implementation must be provided by every specific plugin.
  virtual DWORD SetTime(double time) { return FF_FAIL; }

	/// Default implementation of the FreeFrame getInputStatus instance specific function. This function is called 
	/// to know whether a given input is currently in use. For the default implementation every input is always in use. 
	/// A custom implementation may be provided by every specific plugin.
	///
	/// \param		dwIndex		The index of the input whose status is queried.
	///							It should be in the range [Minimum number of inputs, Maximum number of inputs).
	/// \return					The default implementation always returns FF_FF_INPUT_INUSE or FF_FAIL if the index 
	///							is out of range. A custom implementation may be provided by every specific plugin.
	virtual DWORD GetInputStatus(DWORD dwIndex);

	/// The only public data field CFreeFrameGLPlugin contains is m_pPlugin, a pointer to the plugin instance. 
	/// Subclasses may use this pointer for self-referencing (e.g., a plugin may pass this pointer to external modules, 
	/// so that they can use it for calling the plugin methods).
	CFreeFrameGLPlugin *m_pPlugin;

protected:

	/// The only protected function of CFreeFrameGLPlugin is its constructor. In fact, nor CFFGLPluginManager objects nor 
	/// CFreeFrameGLPlugin objects should be created directly, but only objects of the subclasses implementing specific 
	/// plugins should be instantiated. Moreover, subclasses should define and provide a factory method to be used by 
	/// the FreeFrame SDK for instantiating plugin objects.
	CFreeFrameGLPlugin();
};


#endif
