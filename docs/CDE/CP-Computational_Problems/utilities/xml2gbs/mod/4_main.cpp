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
