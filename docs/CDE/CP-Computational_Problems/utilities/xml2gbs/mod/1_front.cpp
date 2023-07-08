/**
 *
 *  \brief xml2gbs.cpp - Importer for GenBooks formatted as OSIS, ThML, or TEI
 *  \author CrossWire Bible Society, etc, chrislit
 *  \version 1.2
 *  \date 2023.05.25
 *  \copyright 2003-2012 CrossWire Bible Society (http://www.crosswire.org)
 *  \copyright GNU GPL
 *  \details $Id: xml2gbs.cpp 3063 2014-03-04 13:04:11Z chrislit $  
 */


/**
 * \note If the following preprocessor block on _MSC_VER is only for a Microsoft compiler, those using other compilers can skip this code.
 */
#ifdef _MSC_VER
	#pragma warning( disable: 4251 )
#endif


/**
 * \todo the includes were divided and shuffled to separte those which are typical in C/C++ programs versus those that are specific to the SWORD project.
 */
//includes for most or all C or C++ programs
	#include <ctype.h>
	#include <stdio.h>
	#include <fcntl.h>
	#include <errno.h>
	#include <stdlib.h>
	#include <iostream>
	#include <string>
	#include <fstream>
	#include <cstring> //added for compiling strstr() when SWORD project includes are removed.


/**
 * \note A preprocessor condition on STANDALONE was inserted several places to allow this code file to build alone, without linking to any other SWORD project files.
 */
#ifndef STANDALONE
//includes specific to the SWORD project
	#include <entriesblk.h>
	#include <treekeyidx.h>
	#include <rawgenbook.h>

/**
* \todo NO_SWORD_NAMESPACE was mentioned in ChangeLog and defs.h. defs.h had the following comment about it ...
* TODO: What is this? jansorg, why does NO_SWORD_NAMESPACE still define
* a C++ namespace, and then force using it?  This makes no sense to me.
* see commit 1195
*/
	#ifndef NO_SWORD_NAMESPACE
	using sword::TreeKeyIdx;
	using sword::RawGenBook;
	using sword::SWKey;
	#endif
#endif

//#define DEBUG


enum XML_FORMATS { F_AUTODETECT, F_OSIS, F_THML, F_TEI };

/**
 * \bug 1a - different helps circulating in the trunk vs package manager.
 * The help in the source code mentions the TEI format and detectFormat() checks for it. However, 'man xml2gbs' or 'info xml2gbs' help from libsword-utils (from package manager) omits TEI. The function processXML() omits logic for TEI. If TEI is a broader term for OSIS and THML, that isn't obvious from the wording.
 * <br>
*/

/**
 * \bug 1b - different helps circulating in the trunk vs package manager.
 * The help for the manpage that came with the package libsword-utils was observed to have some typos and differ from this source. In the DESCRIPTION section of the manpage, there was the word 'sourse' intead of 'source'. Also, in the OPTIONS section, there was the word 'formal' instead of 'format'. The beginning of the help file from libsword-utils was copied below ...
 * <br><br>
* XML2GBS.1(1)                                                            Sword Module Utilities                                                           XML2GBS.1(1)
* <br> 
* <br> NAME
* <br>       xml2gbs - OSIS/ThML General Book module creation tool
* <br> 
* <br> SYNOPSIS
* <br>       xml2gbs [OPTIONS] SOURCE [MODULENAME]
* <br> 
* <br> DESCRIPTION
* <br>       This is a tool to create SWORD general book modules from OSIS/ThML sourse. A module name can be optionally specified.
* <br> 
* <br> OPTIONS
* <br>      -l  Use long div names in the ThML files.
* <br> 
* <br>       -i  exports to IMP formal instead of creating a module.
* <br> 
* <br>      -fT|-fO
* <br>            Set the importer to ThML or OSIS format respectively. By default it attempts to autodetect.
* <br>
* <br> 
* <br> SEE ALSO
* <br> SWORD 1.8.1+dfsg                                                              2020-02-13                                                                 XML2GBS.1(1)
*/


#define HELPTEXT "xml2gbs 1.0 OSIS/ThML/TEI General Book module creation tool for the SWORD Project\n  usage:\n   xml2gbs [-l] [-i] [-fT|-fO|-fE] <filename> [modname]\n  -l uses long div names in ThML files\n  -i exports to IMP format instead of creating a module\n  -fO, -fT, and -fE will set the importer to expect OSIS, ThML, or TEI format respectively\n    (otherwise it attempts to autodetect)\n"

