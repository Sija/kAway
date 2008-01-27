========================================================================
    KONNEKT PLUGIN : kAway2 Project Overview
========================================================================

This file contains a summary of what you will find in each of the files that
make up your kAway2 plugin.

[Format, NetList, IMController, Status, FormattedStatus, MRU, Message]
    Shared classes, they require stdafx.h with basic files inclusion from Stamina and 
    boost packages. another requirement is "using namespace <boost, std, konnekt, Stamina>"
    statement. Some of them may also require Helpers.

EditFix
	Little fix for standard, windows text controls with some extra functionality 
	which they seems to be lacking.

kAway2.vcproj
    This is the main project file for VC++ projects generated using an Application Wizard. 
    It contains information about the version of Visual C++ that generated the file, and 
    information about the platforms, configurations, and project features selected with the
    Application Wizard.

kAway2.cpp
    This is the main DLL source file.

	When created, this DLL does not export any symbols. As a result, it  
	will not produce a .lib file when it is built. If you wish this project 
	to be a project dependency of some other project, you will either need to 
	add code to export some symbols from the DLL so that an export library 
	will be produced, or you can set the Ignore Input Library property to Yes 
	on the General propert page of the Linker folder in the project's Property 
	Pages dialog box.

kAway2.h
    This is the main header file.
    Copy of this file should be on svn://konnekt.info/konnekt/ as well.

/////////////////////////////////////////////////////////////////////////////
Other standard files:

StdAfx.h, StdAfx.cpp
    These files are used to build a precompiled header (PCH) file
    named kAway2.pch and a precompiled types file named StdAfx.obj.

/////////////////////////////////////////////////////////////////////////////
Other notes:

AppWizard uses "TODO:" comments to indicate parts of the source code you
should add to or customize.

/////////////////////////////////////////////////////////////////////////////
