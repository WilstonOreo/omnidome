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
// Last modified: Oct. 26 2006
//

#ifndef FFGLPLUGININFO_STANDARD
#define FFGLPLUGININFO_STANDARD

#include "FFGL.h"

#ifdef TARGET_OS_MAC
	//there is no need for __stdcall on mac, so this will eliminate any
	//usage of it
	#define __stdcall
#endif

#ifdef __linux__
	#define __stdcall
#endif

//FPCREATEINSTANCEGL is a pointer to a function that creates FFGL plugins
//in this SDK, all FFGL plugins must derive from CFreeFrameGLPlugin
typedef DWORD __stdcall FPCREATEINSTANCEGL(class CFreeFrameGLPlugin **ppOutInstance);

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// \class		CFFGLPluginInfo
///	\brief		CFFGLPluginInfo manages static information concerning a plugin name, version, and description. 
/// \author		Gualtiero Volpe
/// \date		20041110
/// \version	1.0.0.2
///
/// The CFFGLPluginInfo class manages static information related to a FreeFrameGL plugin. Examples are the name of 
/// the plugin, its unique identifier, its type (either source or effect), the current version, the version of 
/// the FreeFrame API the plugin refers to, a short description of the plugin, information about the developer(s) 
/// and possible copyright. In other words, this class stores the information required by the FreeFrame getInfo 
/// and getExtendedInfo global functions. 
/// The CFFGLPluginInfo class is also involved in the process of creating an instance of the subclass implementing 
/// a plugin: it stores a pointer to the factory method of the plugin subclass, which is called when the plugin 
/// object needs to be instantiated. The FreeFrame SDK keeps a prototype instance of the plugin in order to be able 
/// to access information on the plugin at any time. The effectively working instance is created at the time the 
/// plugin is instantiated by the host. 
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class CFFGLPluginInfo {

public:

	/// The constructor of CFFGLPluginInfo receives through its parameters the information that needs
	/// to be stored.
	/// 
	/// \param	pCreateInstance					A pointer to the factory method of the subclass implementing 
	///											the plugin.
	/// \param	pchUniqueID						A string representing the unique identificator of the plugin. 
	///											According to the FreeFrame specification, it must be a not null 
	///											terminated string of 4 1-byte ASCII characters. Longer strings will 
	///											be truncated at the 4th character. 
	/// \param	pchPluginName					A string containing the name of the plugin. According to the 
	///											FreeFrame specification, it must be a not null terminated string 
	///											of 16 1-byte ASCII characters. Longer strings will be truncated at 
	///											the 16th character.
	/// \param	dwAPIMajorVersion				The major version number of the FreeFrame API employed by the plugin.
	///											It is the number before the decimal point in the API version number.
	/// \param	dwAPIMinorVersion				The minor version number of the FreeFrame API employed by the plugin.
	///											It is the number after the decimal point in the API version number.
	/// \param	dwPluginMajorVersion			The major version number of the plugin. It is the number before 
	///											the decimal point in the plugin version number.
	/// \param	dwPluginMinorVersion			The minor version number of the plugin. It is the number after 
	///											the decimal point in the plugin version number.
	/// \param	dwPluginType					The type of the plugin. According to the FreeFrame specification, 
	///											it should be 0 in case of effect plugins and 1 in case of source 
	///											plugins.
	/// \param	pchDescription					A string providing a short description of what the plugin does.
	/// \param	pchAbout						A string providing information on the developer(s) of the plugin, 
	///											their possible company, and possible copyright information.
	/// \param	dwFreeFrameExtendedDataSize		Size in bytes of the FreeFrame ExtendedDataBlock, or 0 if not 
	///											provided by plugin. Extended Data Bloks are not yet exploited 
	///											in the current version of the FreeFrame specification (1.0).
	///											Therefore, at the moment the default value (0) should be used 
	///											for this parameter.
	/// \param	pFreeFrameExtendedDataBlock		32-bit pointer to a FreeFrame ExtendedDataBlock, Extended
	///											Data Bloks are not yet expolited by the FreeFrame specification 
	///											version 1.0. Therefore, at the moment the default value (NULL) 
	///											should be used for this parameter
	CFFGLPluginInfo(
		FPCREATEINSTANCEGL* pCreateInstance,
		const char* pchUniqueID,
		const char* pchPluginName,
		DWORD dwAPIMajorVersion,
		DWORD dwAPIMinorVersion,
		DWORD dwPluginMajorVersion,
		DWORD dwPluginMinorVersion,
		DWORD dwPluginType,
		const char* pchDescription,
		const char* pchAbout,
		DWORD dwFreeFrameExtendedDataSize = 0,
		const void* pFreeFrameExtendedDataBlock = NULL
	);

	/// The standard destructor of CFFGLPluginInfo.
	~CFFGLPluginInfo();

	/// This method returns a pointer to a PluginInfoStruct as defined in FreeFrame.h. Such structure 
	/// contains information on the plugin name and type, its unique identifier, and the version of the 
	/// FreeFrame API it uses.
	///
	/// \return		A pointer to a PluginInfoStruct containing information on the plugin. For further 
	///				information on the definition of PluginInfoStruct see the header file FreeFrame.h and 
	///				the FreeFrame specification version 1.0. 
	const PluginInfoStruct* GetPluginInfo() const;

	/// This method returns a pointer to a PluginExtendedInfoStruct (for further information see
	/// FreeFrame.h and the FreeFrame specification). A PluginExtendedInfoStruct contains information on 
	/// the plugin version, a short description of the plugin, and information about the developer(s) and 
	/// possible copyright issues.
	///
	/// \return		A pointer to a PluginExtendedInfoStruct containing information on the plugin.
	///				For further information on the definition PluginExtendedInfoStruct see the header file 
	///				FreeFrame.h and the FreeFrame specification version 1.0. 
	const PluginExtendedInfoStruct* GetPluginExtendedInfo() const;

	/// This method returns a pointer to the factory method of the subclass implementing the plugin. It is 
	///	called by the FreeFrame SDK when creating a new instance of the plugin.
	///
	/// \return		A pointer to the factory method of the plugin subclass.
	FPCREATEINSTANCEGL* GetFactoryMethod() const;

private:
	
	// Structures containing information about the plugin
	PluginInfoStruct m_PluginInfo;
	PluginExtendedInfoStruct m_PluginExtendedInfo;

	// Pointer to the factory method of the plugin subclass
	FPCREATEINSTANCEGL* m_pCreateInstance;
};


#endif
