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

