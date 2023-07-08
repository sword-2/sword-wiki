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
