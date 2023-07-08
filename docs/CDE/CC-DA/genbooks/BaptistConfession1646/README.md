# README

This readme describes an attempt to repeat/investigate an issue on a wiki talk [Internal structure of a Genbook module made using xml2gbs](https://wiki.crosswire.org/Talk:OSIS_Genbooks) for the SWORD module "BaptistConfession1646".

A wiki paragraph already noted it was best if the OSIS format for the module originated from the baptist denomination. An OSIS original was not yet discovered by internet search, thus some alternate processing methods were attempted as follows.

The SWORD install manager had installed the module of interest which may have helped some logic succeed. 

## Alternate processing - downloads
The shell script 0dl just downloads some initial confessions found on the internet to the downloads folder. These were not used however.

## Alternate processing - SWORD utilities
The shell script 0util contained some simple logic using sword utilities. The mod2imp output and a bdt file in the module install location (/usr/share/sword/) were processed. Unexpected internal structure was initially observed like the wiki mentioned, but a different unexpected structure. The table of contents was split between two <list> tags, one for each level 1 section (Confession; Appendix). The unexpected thing was that no div tag was observed. Reference 1 below predicted such a tag for the table of contents.

Some output went to the mod2imp\_output directory

reference
1: [OSIS Users Manual (OSIS Schema 2.1.1) By Patrick Durusau](http://www.crosswire.org/osis/OSIS%202.1.1%20User%20Manual%2006March2006.pdf)
table of contents was mentioned in section 7.5. "The div Element" and
Appendix F "USFM to OSIS Mapping"

## Alternate processing - swift parser
The shell script 0swift did a rudimentary swift parser which just echoed content, but required installing swift on Ubuntu. Selected module files were also copied to the folders mods.d and modules to enable running the swift parser independent of SWORD code. Other files and directories used by swift included Package.swift, Sources, and Tests. The .gitignore intended to stop uploading the .build directory since one might regenerate that.

The parser quit early at the initial tag(s). The early quit appeared to be caused by not having a root tag in those files. A root tag was temporarily added to the local copy of the bdt file and then the parsing progressed as expected - just echoing content. Afterwards, the new root tag was removed to return to the file to its original.
