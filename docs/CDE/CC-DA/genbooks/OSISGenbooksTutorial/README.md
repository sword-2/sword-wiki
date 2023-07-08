# OSIS Genbooks

The [OSIS Genbooks example](https://wiki.crosswire.org/OSIS\_Genbook) was attempted.


## Systems relevant to the problem
Two computer devices were used as well as data files. The devices had some built-in or extra installations as outlined below.

- Ubuntu 20.04 server (console).
	-- curl to download the Wikipedia web page
	-- mount/umount to connect a USB to transfer files
	-- vim text editor
	-- xmllint
	-- libxml2-utils
	-- libsword-utils #xml2gbs, etc
	-- zip

- Android tablet
	-- bishop front-end app installed from Google play. 

- USB stick to transfer a file from the Ubuntu laptop to the Android tablet.

## Model of proposed solution
This repo includes the files output by xml2gbs program, reformats the output into a zip file which the bishop front-end accepted, adds a conf file, and discussed what happened when it was tried.


## Interactions within and between systems
The following steps mostly correspond to the shell script 0.


### Wikipedia-curl
The OSIS file on the Wikipedia article was currently split in pieces, intermingled with Wikipedia text. The Linux curl command was used to download the HTML content. 

### vim editor
The vim editor was used to extract just the xml content from the downloaded HTML file. The modification was saved to tutorial.xml.

### xmllint
The subject tool was run on tutorial.xml to detect errors in the code.

### File system adjustments
The SWORD project used a certain directory structure - a configuration file in the mods.d directory and the content in the module directory. A configuration (.conf) file was copied from another genbook and its content adjusted to what had been known for the tutorial. And the same folder hierarchy used by other genbooks was put in the modules directory.

### xml2gbs, zip
xml2gbs was run to convert the xml content to a sword module. Additionally zip was run to make a final zip file. The zip file was copied onto a USB drive.

### Bishop front-end
The zip file was copied to the Android tablet. The bishop app's library menu allowed installing the zip. This was done by selecting the pick list titled "modules" and then picking "Advanced Installation Options". That option allowed finding and installing a module with the file browser. 

## Status
The bishop app accepted the module install without reporting any error, however a table of contents was not observed like other genbooks. The table of contents would have provided a way to navigate to the content for reading based on other genbooks loaded. Some other General books in the internet repos were successfully installed to the bishop app's (Android app) so it could work, but the data processing might have an error in this instance.
