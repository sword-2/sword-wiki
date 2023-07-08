#Makefile for planning changes to xml2gbs

.DELETE_ON_ERROR:

#The include path was just from one person's past use on Ubuntu server system
CXX=g++ -std=c++17 -I/home2/usr/local/include

##############################################################################################################################################

#The following 3 blocks can make some lists to be used later

#xml2gbs_mod := $(wildcard *.cpp) #wildcard not perfect since there are files to ignore
xml2gbs_mod := $(shell find * -maxdepth 0 -type f ! -name xml2gbs_mod.cpp ! -name Makefile ! -name firstDoc.xml)
#debugging printout option follows:
$(info $$xml2gbs_mod content: [${xml2gbs_mod}])

#sorting the input better keeps the original order of functions
sorted := $(sort $(xml2gbs_mod))
#debugging printout option follows:
$(info $$sorted content: [${sorted}])

#unused for now - prepare names for object files as a function of source files
#xml2gbs_mod_ := $(patsubst %.cpp, %.o, $(sorted))
#debugging printout option follows:
#$(info $$xml2gbs_mod_ content: [${xml2gbs_mod_}])

##############################################################################################################################################

#The following preprocessor option STANDALONE is to temporarily remove / swap out code that tries to link to the rest of the larger program. 
#The option allows the file to build as a simple file without linking to all the other SWORD stuff. Remove to build with linking out to other SWORD stuff
standalone=-D STANDALONE


##############################################################################################################################################

all: xml2gbs_mod.o

#concatenate the pieces of the file into a total file prior to running compiler. It just allows files to be separated by functions, for example.
xml2gbs_mod.cpp: $(sorted)
	@cat $(sorted) > xml2gbs_mod.cpp; \
	printf '\n/** \\file' >> xml2gbs_mod.cpp; \
	printf "\n* docs for this file" >> xml2gbs_mod.cpp; \
	printf "\n*/" >> xml2gbs_mod.cpp;

xml2gbs_mod.o: xml2gbs_mod.cpp
	$(CXX) $(standalone) $^ -o $@ 2>&1

#This runs the program (standalone) and reads an existing OSIS file as input.
run:
	./xml2gbs_mod.o firstDoc.xml

.PHONY : clean
clean:
	@if [ -f xml2gbs_mod.cpp ]; then rm xml2gbs_mod.cpp; fi; \
	if [ -f xml2gbs_mod.o ]; then rm xml2gbs_mod.o; fi

