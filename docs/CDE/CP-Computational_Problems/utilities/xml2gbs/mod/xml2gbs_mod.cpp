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

unsigned char detectFormat(char* filename) {

  unsigned char format = F_AUTODETECT;

  std::ifstream infile(filename);
  std::string entbuffer;
  
  if (!infile.is_open()) {
        std::cerr << HELPTEXT;
        std::cerr << std::endl << std::endl << "Could not open file \"" << filename << "\"" << std::endl;
  }
  else {
	while (std::getline(infile, entbuffer) && format == F_AUTODETECT) {
                if (strstr(entbuffer.c_str(), "<osis")) {
                        format = F_OSIS;
                }
                else if (strstr(entbuffer.c_str(), "<ThML")) {
                        format = F_THML;
                }
		else if (strstr(entbuffer.c_str(), "<TEI")) {
                        format = F_TEI;
		}
        }
        infile.close();
  }

  return format;
}

/**
 * \param[in] filename	input filename to process. Command line argument
 * \param[in] modname
 * \param[in] longnames - a command line option to use long div names for THML format 
 * \param[in] exportfile - a command line option to identify if the program should output IMP format instead of a module.
 * \param[in] format - a command line option to identify the file format
* \return -1 if unable to open input file;
* 	<br>0 if function successfully completed. Function success may not guarantee a module output will successfully load to a SWORD front-end.
*/
int processXML(const char* filename, char* modname, bool longnames, bool exportfile, unsigned char format) {
  signed long i = 0;
  char* strtmp;
  std::string entbuffer;

#ifdef DEBUG
  printf ("%s :%s :%d :%d :%d\n\n", filename, modname, longnames, exportfile, format);
#endif

/**
 * \note Would variable names be clearer if related variables used similar names? An example was char * filename and std::ifstream infile(filename).
 */
  std::ifstream infile(filename);
  if (!infile.is_open()) {
        std::cerr << HELPTEXT;
        std::cerr << std::endl << std::endl << "Could not open file \"" << filename << "\"" << std::endl;
        return -1;
  }
  std::ofstream outfile;
  if (exportfile) {
    strcat (modname, ".imp");
    outfile.open(modname);
  }

#ifndef STANDALONE
  TreeKeyIdx * treeKey;
  RawGenBook * book = NULL;
#endif
 
  std::string divs[32];

  int level = 0;
/**
 \note Would it make sense to change the variable keybuffer to some variation of StartTag or EndTag? The names StartTag and EndTag were observed in the XML recommendation, reference W3C #1.
*/
  std::string keybuffer = "";
  std::string keybuffer2;
  std::string n;
  std::string type;
  std::string title;
  unsigned long entrysize = 0;
  unsigned long keysize = 0;
  bool closer = false;

  if (!exportfile) {
    // Do some initialization stuff
/**
 * \bug 2 - possible software memory safety hazard part 1/1.
 * Is the treeKey variable unused? There is only one line of code between the new and delete which doesn't use the variable name. Maybe an opportunity to simplify the includes or linking. 
*/

#ifndef STANDALONE
    TreeKeyIdx::create(modname);
    treeKey = new TreeKeyIdx(modname);
    RawGenBook::createModule(modname);
    delete treeKey;
/**
 * \bug 3a - possible software memory safety hazard part 1/2.
 * The memory for book is allocated conditionally and there is no a variable to remember if had been allocated. The condition depends on whether the user supplied an exportfile input.
*/
    book = new RawGenBook(modname);
#endif
  }



/**
 * \note Unusued lines of code below. Can it be removed?
 */
#ifdef DEBUG
//  TreeKeyIdx root = *((TreeKeyIdx *)((SWKey *)(*book)));
#endif

  int c;
  while ((c = infile.get()) != EOF) {
    if (c == '<') {
	    {
/**
 \note Why not keep the beginning bracket of the tag? Partial tags can work, but they are not the common definiton of a tag someone new to the program knows about.
*/
		    keybuffer = "";
		    while ((c = infile.get()) != '>')
			    keybuffer += c;
		    keybuffer += c;
	    }

      if (keybuffer.length()) {
 if (((format == F_OSIS) && ((!strncmp(keybuffer.c_str(), "/div>", 5)) || (!strncmp(keybuffer.c_str(), "/verse>", 7)) || (!strncmp(keybuffer.c_str(), "/chapter>", 9)))) ||
           ((format == F_THML) && ((!strncmp(keybuffer.c_str(), "/div", 4)) && (keybuffer[4] > '0' && keybuffer[4] < '7')))) {
	  if (!closer) {
       	    keysize = 0;
            keybuffer2 = "";
       	    for (i = 0; i < level; i++) {
              keybuffer2 += '/';
       	      keysize++;
       	      keybuffer2 += divs[i];
              keysize += divs[i].length();
	      std::cout << keybuffer2 << std::endl;
       	    }

	    if (level) {
	      std::cout << keybuffer2 << std::endl;
	      if (exportfile) {
		outfile << "$$$" << keybuffer2 << std::endl << entbuffer << std::endl;
	      }
	      else {
			#ifndef STANDALONE
				book->setKey(keybuffer2.c_str());
				book->setEntry(entbuffer.c_str(), entrysize); // save text to module at current position
			#else
				std::cout << "\nkeybuffer2.c_str() =" << keybuffer2.c_str();
				std::cout << "\nentbuffer.c_str() =" << entbuffer.c_str();
			#endif
	      }
	    }
	  }
	  level--;
	  entbuffer = "";
	  entrysize = 0;

	  closer = true;
	}
	else if (((format == F_OSIS) && !((!strncmp(keybuffer.c_str(), "div>", 4) || !strncmp(keybuffer.c_str(), "div ", 4)) || (!strncmp(keybuffer.c_str(), "verse>", 6) || !strncmp(keybuffer.c_str(), "verse ", 6)) || (!strncmp(keybuffer.c_str(), "chapter>", 8) || !strncmp(keybuffer.c_str(), "chapter ", 8)))) ||
                ((format == F_THML) && !((!strncmp(keybuffer.c_str(), "div", 3)) && (keybuffer[3] > '0' && keybuffer[3] < '7')))) {
	  entbuffer += '<';
	  entrysize++;
	  entrysize += keybuffer.length();
	  entbuffer += keybuffer;
	}
	else {
	  //we have a divN...
       	  if (!closer) {
            keysize = 0;
       	    keybuffer2= "";
       	    for (i = 0; i < level; i++) {
              keybuffer2 += '/';
       	      keysize++;
       	      keybuffer2 += divs[i];
              keysize += divs[i].length();
	      std::cout << keybuffer2 << std::endl;
       	    }

	    if (level) {
	      std::cout << keybuffer2 << std::endl;
	      if (exportfile) {
		outfile << "$$$" << keybuffer2 << std::endl << entbuffer << std::endl;
	      }
	      else {
	#ifndef STANDALONE
		book->setKey(keybuffer2.c_str());
		book->setEntry(entbuffer.c_str(), entrysize); // save text to module at current position
	#else
		std::cout << "\nkeybuffer2.c_str() =" << keybuffer2.c_str();
		std::cout << "\nentbuffer.c_str() =" << entbuffer.c_str();
	#endif
	      }
	    }
	  }

	  entbuffer= "";
	  entrysize = 0;

	  level++;
          keysize = keybuffer.length()-1;

          type = "";
      	  n = "";
       	  title = "";

          if (format == F_OSIS && longnames == false) {
               	  strtmp = (char*)strstr(keybuffer.c_str(), "osisID=\"");
               	  if (strtmp) {
               	    strtmp += 8;
               	    for (;*strtmp != '\"'; strtmp++) {
               	      if (*strtmp == 10) {
               		title += ' ';
               	      }
               	      else if (*strtmp == '.') {
                        title = "";
               	      }
               	      else if (*strtmp != 13) {
               		title += *strtmp;
               	      }
               	    }
               	  }
                  keybuffer = title;
          }
          else {
               	  strtmp = (char*)strstr(keybuffer.c_str(), "type=\"");
               	  if (strtmp) {
               	    strtmp += 6;
               	    for (;*strtmp != '\"'; strtmp++) {
               	      if (*strtmp == 10) {
               		type+= ' ';
               	      }
               	      else if (*strtmp != 13) {
               		type+= *strtmp;
               	      }
               	    }
               	  }

               	  strtmp = (char*)strstr(keybuffer.c_str(), "n=\"");
               	  if (strtmp) {
               	    strtmp += 3;
               	    for (;*strtmp != '\"'; strtmp++) {
               	      if (*strtmp == 10) {
               		n += ' ';
               	      }
               	      else if (*strtmp != 13) {
               		n += *strtmp;
               	      }
               	    }
               	  }

                  if (format == F_OSIS) {
                       	  strtmp = (char*)strstr(keybuffer.c_str(), "title=\"");
                	  if (strtmp) {
                	    strtmp += 7;
                	    for (;*strtmp != '\"'; strtmp++) {
                	      if (*strtmp == 10) {
                		title += ' ';
                	      }
                	      else if (*strtmp != 13) {
                		title += *strtmp;
                	      }
                	    }
                	  }
                  }
                  else if (format == F_THML) {
                	  strtmp = (char*)strstr(keybuffer.c_str(), "title=\"");
                	  if (strtmp) {
                	    strtmp += 7;
                	    for (;*strtmp != '\"'; strtmp++) {
                	      if (*strtmp == 10) {
                		title += ' ';
                	      }
                	      else if (*strtmp != 13) {
                		title += *strtmp;
                	      }
                	    }
                	  }
                  }

        	  keybuffer = type;
        	  if (keybuffer.length() && n.length())
        	    keybuffer += " ";
        	  keybuffer += n;

        	  if (longnames && keybuffer.length())
        	    keybuffer += ": ";
        	  if (longnames || !keybuffer.length())
        	    keybuffer += title;
          }
          divs[level-1] = keybuffer;

	  closer = false;
	}
      }
    }
/**
 * \todo Is the check for 13 for carriage return. If so a comment for this would be easy.
 */

    else if (c != 13) {
      entbuffer += c;
      entrysize++;
    }
  }

/**
 * \note Unused lines of code below. Can it be removed?
 */
#ifdef DEBUG
//  printTree(root, treeKey);
#endif

/**
 * \bug 3b - possible software memory safety hazard part 2/2.
 * The delete was commented out and had a reason that it causes nasty-bad errors upon execution. This may have been from trying to delete memory that had not been allocated.
*/
//  delete book;  //causes nasty-bad errors upon execution
	return 0;
}
int main(int argc, char **argv) {
  int i = 0;

  char modname[256];
  *modname = 0;
  char filename[256];
  *filename = 0;

  bool longnames = false;
  bool exportfile = false;
  unsigned char format = F_AUTODETECT;

  if (argc > 2) {
        for (i = 1; i < argc; i++) {
                if (argv[i][0] == '-') {
                        switch (argv[i][1]) {
                                case 'l':
                                        longnames = true;
                                        continue;
/**
 * \todo For case i, can the variable exportfile be renamed to some version of imp/IMP? This is because IMP is the term used in the help text for the case option and also IMP was used in the <a href="https://www.crosswire.org/sword/develop/swordmodule/">Module Making</a> web site in the crosswire domain.
 */
                                case 'i':
                                        exportfile = true;
                                        continue;
                                case 'f':
                                        if (argv[i][2] == 'O') {
                                                format = F_OSIS;
                                        }
                                        else if (argv[i][2] == 'T') {
                                                format = F_OSIS;
                                        }
                                        else {
                                                format = F_AUTODETECT;
                                        }
                                        continue;
                        }
                }
                else if (*filename == 0) {
                        strncpy (filename, argv[i], 200);
                }
/**
 * \note if modname is an optional input, would it fit with the other options like l-longnames, i-exportfile, and f-format?
 */
                else if (*modname == 0) {
                        strncpy (modname, argv[i], 200);
                }
        }
  }
  else if (argc > 1) {
    strncpy (filename, argv[1], 200);
  }

  if (!*filename) {
    std::cerr << HELPTEXT << std::endl;
    return -1;
  }
  else {
        if (!*modname) {
                for (i = 0; (i < 256) && (filename[i]) && (filename[i] != '.'); i++) {
                        modname[i] = filename[i];
                }
                modname[i] = 0;
        }

        format = (format == F_AUTODETECT) ? detectFormat(filename) : format;
        if (format == F_AUTODETECT) {
                fprintf(stderr, HELPTEXT);
                fprintf(stderr, "\n\nCould not detect file format for file \"%s\", please specify.\n", filename);
                return -1;
        }

/**
 * \note retCode received from processXML() is not used to print a message for error or success. 
 */
        int retCode =  processXML (filename, modname, longnames, exportfile, format);

/**
 * \bug 4 - possible software memory safety hazard part 1/1.
 * The return code from processXML is directly sent back to the shell or calling program. The value can be -1, which may not be a valid code for the Linux terminal. The Linux terminal expects 0 - 255.
*/
        return retCode;
  }
}

/** \file
* docs for this file
*/