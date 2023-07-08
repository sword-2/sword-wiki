# Learning/Problem - xml2gbs

An OSIS genbook example from the [developers wiki for SWORD](https://wiki.crosswire.org/Main_Page) was not successfully displayed in the bishop front-end. The problem was investigated to determine if it was a personal mistake. The most recent effort was in the [mod](https://github.com/sword-2/sword-wiki/tree/master/docs/CDE/CP-Computational_Problems/utilities/xml2gbs/mod) folder. This folder primarily investigates changing the source code for xml2gbs.cpp to allow the source file to be built standalone to learn it alone without the tens of thousands of other code files. Three make commands are used to run the code in the folder - 1) make clean, 2) make, 3) make run.


Some older efforts include Table 1 regarding code analysis. Further, Table 2 outlines some previous attempts to fix errors using a systematic process (core practice 6) in the same way young students might have reordered the sequence of commands. All of this didn't succeed however.


<table border=1><caption>Table 1 - code analysis</caption>
<tr><th>File</th><th>Comments</th><th>Link</th></tr>
<tr>
	<td>utilities/xml2gbs.cpp</td>
	<td>Software lines of code were read and analyzed to learn. Comments were put into doxygen format.</td>
	<td><a href="https://sword-2.github.io/sword-wiki/CDE/CP-Computational_Problems/utilities/xml2gbs/html_/index.html">doxygen</a></td>
</tr>
</table>

<table border=1><caption>Table 2 fix errors with systematic process, young students trial and error</caption>
<tr><th>File/folder</th><th>Comments</th><th>Link</th></tr>
<tr>
	<td>subprob1</td>
	<td>subproblem 1 - a model xml file from w3schools
	<br>A model xml file was acquired from w3schools for a <a href="https://www.w3schools.com/xml/xml_examples.asp">plant catalog</a>. The plant catalog was just used for a data file to learn what the bishop front-end could process. It was not known if the plant\_catalog had the plants that would be mentioned in a High School class. 
	<br><br>According to the script 0, this model file was converted to a SWORD module / zip file. The zip was moved to an Android device running bishop, and installed. Result: Same outcome as the SWORD tutorial file - it loaded to the bishop front-end, was listed in the library, but nothing was observed for display.
	</td>
	<td>-</td>
</tr>

<tr>
	<td>subprob2</td>
	<td>investigate root node
		<br>The script 0 copied links from the root node of the OSIS Genbook tutorial to test them in a browser. Results for several links produced the following message ... 
		<br>Not Found
		<br>The requested URL was not found on this server.
		<br><br>Another sword wiki predicted the domain [www.bibletechnologies.net](https://wiki.crosswire.org/OSIS_Bibles#Bible_Technologies_Group) will no longer work. Another [link](https://wiki.crosswire.org/OSIS_211_CR) says the schema now needs to be a local copy on your computer. Therefore, piece 3 tries this approach.
	</td>
	<td>-</td>
</tr>


<tr>
	<td>subprob3</td>
	<td>subproblem 3 - add a schema (\*xsd file)
		<br>Supporting content was loaded to the sub-folder subprob3. Here, two files were developed - an xml with doctrine (new library categories added) and a corresponding xsd, so that there was a schema. xmllint reported the file(s) were valid. The same steps were repeated to load it to the bishop app. Also, the same result occurred - it loaded but nothing observed for display. 
		<br><br>No intent was made to modify doctrine, but library categories had been added. The categories were to eventually see if the doctrine could be restructured to fit the library categories - same text different ordering.
	</td>
	<td>-</td>
</tr>

<tr>
	<td>-</td>
	<td>subproblem 4 - see if the xml files open in a common browser - Chrome, Samsung internet, Firefox, etc
	<br>This was just another type of validity check that the xml files were reasonable. Both files plant\_catalog.xml and doctrineFcc.xml were copied on a USB to move from the laptop to Android tablet since the w3m terminal browser didn't open xml. Samsung internet opened and displayed both xml files without any errors and allowed clicking arrows to expand and hide content.
	</td>
	<td>-</td>
</tr>

<tr>
	<td>subprob5</td>
	<td>subproblem 5 - example from a users manual
	<br>Supporting content was loaded to the sub-folder subprob5. Another example was copied from the OSIS Users Manual (OSIS Schema 2.1.1) By Patrick Durusau, section 4 - Your first OSIS Document. In addition to the example, the mods.d folder was added with a conf file. No parser errors were reported by xmllint. This example was not a genbook like previous ones so the menu in the bishop app was different. The conf file was also a little different and adapted from a KJV conf file. A similar result occurred - a message saying "Nothing to display" after going to Esther chapter 1 where the content was in the xml file.
	</td>
	<td>-</td>
</tr>

<tr>
	<td>-</td>
	<td>subproblem 6 - subscribe to users group for human help
	<br>The OSIS Users Manual (OSIS Schema 2.1.1) had been acquired from a Google search. That manual was several years old (2006), but noted there was a OSIS users group. The procedure to subscribe to the group was done, sending mail to osis-user@whi.wts.edu and setting the Subject line to "subscribe". The replies initially and after retry were essentially copied below:
	<br><br>Delivery incomplete
	<br>There was temporary problem delivering your message to osis-user@whi.wt.edu. Gmail will retry for 46 more hours. You'll be notified if the delivery fails permanently.
	<br><br>The response was:
	<br>The recipient server did not accept our requests to connect. Learn more at https://support.google.com/mail/answer/7720 [whi.wts.edu 107.0.236.14: FAILED PRECONDITION: connect error (113): No route to host]
	</td>
	<td>-</td>
</tr>
</table>

