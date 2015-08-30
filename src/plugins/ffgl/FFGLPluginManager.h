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

#ifndef FFGLPLUGINMANAGER_STANDARD
#define FFGLPLUGINMANAGER_STANDARD


#include "FFGL.h"


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// \class		CFFGLPluginManager
///	\brief		CFFGLPluginManager manages information concerning a plugin inputs, parameters, and capabilities.
/// \author		Gualtiero Volpe
/// \version	1.0.0.3
///
/// The CFFGLPluginManager class is the base class for FreeFrameGL plugins developed with the FreeFrameGL SDK since it provides 
/// them with methods for automatically manage information concerning plugin inputs, paramaters, and capabilities. 
/// Examples of information managed by this class are the number of inputs and parameters of a plugin; the name, type and 
/// default value of each parameter; the image formats a plugin supports; the supported optimizations. 
/// Plugins developed with the FreeFrameGL SDK (and thus having this class as base class) should call the protected methods 
/// of this class in order to specify the information related to their inputs, parameters and capabilities. These calls 
/// are usually done while constructing the plugin subclass. Plugins subclasses should also call methods of this class in 
/// order to get information about the images they are going to process (i.e., their width, height, depth, orientation). 
/// The defualt implementations of the FreeFrame gloabal functions call the public methods of this class in order to 
/// return to the host information about a plugin inputs, parameters, and capabilities.
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class CFFGLPluginManager
{
public:
	
	/// The standard destructor of CFFGLPluginManager.
	virtual ~CFFGLPluginManager();

	/// This method returns the minimum number of inputs the host must provide. 
	/// It is usually called by the default implementations of the FreeFrame global functions.
	///
	/// \return		The minimum number of inputs the host must provide.	
	int GetMinInputs() const;
	
	/// This method returns the maximum number of inputs the plugin can receive. 
	/// It is usually called by the default implementations of the FreeFrame global functions.
	///
	/// \return		The maximum number of inputs the plugin can receive.
	int GetMaxInputs() const;
	
	/// This method returns how may parameters the plugin has. 
	/// It is usually called by the default implementations of the FreeFrame global functions.	
	///
	/// \return		The number of parameters of the plugin.
	int GetNumParams() const;
	
	/// This method returns the name of the plugin parameter whose index is passed as parameter 
	/// to the method. It is usually called by the default implementations of the FreeFrame global functions.
	///
	/// \param	dwIndex		The index of the plugin parameter whose name is queried. 
	///						It should be in the range [0, Number of plugin parameters).
	/// \return				The name of the plugin parameter whose index is passed to the method. 
	///						The return value is a pointer to an array of 16 1-byte ASCII characters, 
	///						not null terminated (see FreeFrame specification). NULL is returned on error.
	char* GetParamName(DWORD dwIndex) const;
	
	/// This method is called to know the type of the plugin parameter whose index is passed as parameter 
	/// to the method. It is usually called by the default implementations of the FreeFrame global functions.
	///
	/// \param	dwIndex		The index of the plugin parameter whose name is queried. 
	///						It should be in the range [0, Number of plugin parameters).
	/// \return				The type of the plugin parameter whose index is passed as parameter to the method. 
	///						Codes for allowed parameter types are defined in FreeFrame.h. 
	///						In case of error, FF_FAIL is returned. 
	DWORD GetParamType(DWORD dwIndex) const;

	/// This method is called to get the default value of the plugin parameter whose index is passed as parameter 
	/// to the method. It is usually called by the default implementations of the FreeFrame global functions.
	///
	/// \param	dwIndex		The index of the plugin parameter whose name is queried. 
	///						It should be in the range [0, Number of plugin parameters).
	/// \return				The default value of the plugin parameter whose index is passed as parameter to the method. 
	///						The return value should be cast either to a char* in case of text parameters or to a float* 
	///						in any other case. In case of error, NULL is returned.
	void* GetParamDefault(DWORD dwIndex) const;

	/// This method is called by a the host to determine whether the plugin supports the SetTime function
	bool GetTimeSupported() const;

protected:

	///	The standard constructor of CFFGLPluginManager. 
	/// \remark	Notice that the CFFGLPluginManager constructor is a protected member function, i.e., nor CFFGLPluginManager 
	///			objects nor CFreeFramePlugin objects should be created directly, but only objects of the subclasses 
	///			implementing specific plugins should be instantiated.
	CFFGLPluginManager();
	
	/// This method is called by a plugin subclass, derived from this class, to indicate the minimum number 
	/// of inputs the host must provide. This method is usually called when a plugin object is instantiated 
	/// (i.e., in the plugin subclass constructor).
	///
	/// \param	iMinInputs	The plugin subclass should set it to the minimum number of inputs 
	///						the host must provide.
	void SetMinInputs(int iMinInputs);
	
	/// This method is called by a plugin subclass, derived from this class, to indicate the maximum number 
	/// of inputs the plugin can receive. This method is usually called when a plugin object is instantiated 
	/// (i.e., in the plugin subclass constructor).
	///
	/// \param	iMaxInputs	The plugin subclass should set it to the maximum number of inputs the plugin 
	///						can receive.
	void SetMaxInputs(int iMaxInputs);

	/// This method is called by a plugin subclass, derived from this class, to specify name, type, and default 
	/// value of the plugin parameter whose index is passed as parameter to the method. This method is usually 
	/// called when a plugin object is instantiated (i.e., in the plugin subclass contructor). This version of 
	/// the SetParamInfo function (DefaultValue of type float) should be called for all types of plugin parameters 
	/// except for text, boolean, and event parameters.
	///
	/// \param	dwIndex			Index of the plugin parameter whose data are specified.
	///							It should be in the range [0, Number of plugin parameters).
	/// \param	pchName			A string containing the name of the plugin parameter.
	///							According to the FreeFrame specification it should be at most 16 1-byte ASCII 
	///							characters long. Longer strings will be truncated at the 16th character.
	/// \param	dwType			The type of the plugin parameter. Codes for allowed types are defined in FreeFrame.h.
	/// \param	fDefaultValue	The default value of the plugin parameter. According to the FreeFrame
	///							specification it must be a float in the range [0, 1].
	void SetParamInfo(DWORD dwIndex, const char* pchName, DWORD dwType, float fDefaultValue);
	
	/// This method is called by a plugin subclass, derived from this class, to specify name, type, and default 
	/// value of the plugin parameter whose index is passed as parameter to the method. This method is usually 
	/// called when a plugin object is instantiated (i.e., in the plugin subclass contructor). This version of 
	/// the SetParamInfo function (DefaultValue of type bool) should be called for plugin parameters of type 
	/// boolean or event.
	///
	/// \param	dwIndex			Index of the plugin parameter whose data are specified.
	///							It should be in the range [0, Number of plugin parameters).
	/// \param	pchName			A string containing the name of the plugin parameter.
	///							According to the FreeFrame specification it should be at most 16 1-byte ASCII 
	///							characters long. Longer strings will be truncated at the 16th character.
	/// \param	dwType			The type of the plugin parameter. Codes for allowed types are defined in FreeFrame.h.
	/// \param	bDefaultValue	The boolean default value of the plugin parameter.
	void SetParamInfo(DWORD dwIndex, const char* pchName, DWORD dwType, bool bDefaultValue);

	/// This method is called by a plugin subclass, derived from this class, to specify name, type, and default 
	/// value of the plugin parameter whose index is passed as parameter to the method. This method is usually 
	/// called when a plugin object is instantiated (i.e., in the plugin subclass contructor). This version of 
	/// the SetParamInfo function (DefaultValue of type char*) should be called for plugin parameters of type text.
	///
	/// \param	dwIndex			Index of the plugin parameter whose data are specified.
	///							It should be in the range [0, Number of plugin parameters).
	/// \param	pchName			A string containing the name of the plugin parameter.
	///							According to the FreeFrame specification it should be at most 16 1-byte ASCII 
	///							characters long. Longer strings will be truncated at the 16th character.
	/// \param	dwType			The type of the plugin parameter. Codes for allowed types are defined in FreeFrame.h.
	/// \param	pchDefaultValue	A string to be used as the default value of the plugin parameter.
	void SetParamInfo(DWORD dwIndex, const char* pchName, DWORD dwType, const char* pchDefaultValue);

	/// This method is called by a plugin subclass, derived from this class, to indicate whether the
  /// SetTime function is supported
	///
	/// \param	supported	The plugin indicates whether it supports the SetTime function by passing true or false (1 or 0)
	void SetTimeSupported(bool supported);

private:
		
	// Structure for keeping information about each plugin parameter
	typedef struct ParamInfoStruct {
		DWORD ID;							
		char Name[16];					
		DWORD dwType;					
		float DefaultValue;				
		char* StrDefaultValue;			
		ParamInfoStruct* pNext;	
	} ParamInfo;

	// Information on paramters and pointers to ParamInfo list
	int m_NParams;
	ParamInfo* m_pFirst;
	ParamInfo* m_pLast;
	
	// Inputs
	int m_iMinInputs;
	int m_iMaxInputs;

  // Time capability
  bool m_timeSupported;
};


#include "FFGLPluginManager_inl.h"

#endif

