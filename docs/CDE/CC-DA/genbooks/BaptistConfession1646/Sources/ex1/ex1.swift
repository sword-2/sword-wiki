
import Foundation
import FoundationXML

//class to store global variables that remember which tags are currently being processed
class activeTags {
		//var articleNth = 0 //a counter was put into a function
	//some tags observed in baptist confession 1689
		var titlePage = 0
		var title = 0
		var p = 0
		var list = 0
		var item = 0
		var reference = 0
		var div = 0
		var table = 0
		var row = 0
		var cell = 0
		var hi = 0
		var foreign = 0
		var work = 0
		var creator = 0
		var refSystem = 0
		var figure = 0
		var note = 0
		var header = 0
		var osisText = 0
		var osis = 0
		var other = 0
		var otherContent = ""

		var lineNumber = 0
}
let aT = activeTags()

//debugTags - This function may be called to enhance printouts since it checks which tags are in use (non-zero) depth or level.
func debugTags() -> String {
			var tagStatus = "debugTags:"
			tagStatus += " lN=" + String(aT.lineNumber) + ", tags:"
			if (aT.titlePage != 0) {
				tagStatus += " titlePage(" + String(aT.titlePage) + ")"
			}
			if (aT.title != 0) {
				tagStatus += " title(" + String(aT.title) + ")"
			}
			if (aT.p != 0) {
				tagStatus += " p(" + String(aT.p) + ")"
			}
			if (aT.list != 0) {
				tagStatus += " list(" + String(aT.list) + ")"
			}
			if (aT.item != 0) {
				tagStatus += " item(" + String(aT.item) + ")"
			}
			if (aT.reference != 0) {
				tagStatus += " reference(" + String(aT.reference) + ")"
			}
			if (aT.div != 0) {
				tagStatus += " div(" + String(aT.div) + ")"
			}
			if (aT.table != 0) {
				tagStatus += " table(" + String(aT.table) + ")"
			}
			if (aT.row != 0) {
				tagStatus += " row(" + String(aT.row) + ")"
			}
			if (aT.cell != 0) {
				tagStatus += " cell(" + String(aT.cell) + ")"
			}
			if (aT.hi != 0) {
				tagStatus += " hi(" + String(aT.hi) + ")"
			}
			if (aT.foreign != 0) {
				tagStatus += " foreign(" + String(aT.foreign) + ")"
			}
			if (aT.work != 0) {
				tagStatus += " work(" + String(aT.work) + ")"
			}
			if (aT.creator != 0) {
				tagStatus += " creator(" + String(aT.creator) + ")"
			}
			if (aT.refSystem != 0) {
				tagStatus += " refSystem(" + String(aT.refSystem) + ")"
			}
			if (aT.figure != 0) {
				tagStatus += " figure(" + String(aT.figure) + ")"
			}
			if (aT.note != 0) {
				tagStatus += " note(" + String(aT.note) + ")"
			}
			if (aT.osisText != 0) {
				tagStatus += " osisText(" + String(aT.osisText) + ")"
			}
			if (aT.osis != 0) {
				tagStatus += " osis(" + String(aT.osis) + ")"
			}
			if (aT.other != 0) {
				tagStatus += " other/" + aT.otherContent + "(" + String(aT.other) + ")"
			}
			
			if tagStatus.isEmpty { return("error: tagStatus=nil") }
			else { return(tagStatus) }

}

class XMLTest : NSObject, XMLParserDelegate {


	func test(url: URL) {
		let parser = XMLParser(contentsOf: url)!
		parser.delegate = self
		let success = parser.parse()
		if success {
			print("done")
		} else {
			print("error \(parser.parserError!)")
		}
	}

	var depth = 0
	var depthIndent: String {
		return [String](repeating: "  ", count: self.depth).joined()
	}


	func parserDidStartDocument(_ parser: XMLParser) {
		aT.lineNumber = parser.lineNumber
		print("Start of the document")
		//print("Line number: \(parser.lineNumber)")
	}

	func parserDidEndDocument(_ parser: XMLParser) { 
		aT.lineNumber = parser.lineNumber
		let tS = debugTags()
		print("End of Document, tagStatus=[\(tS)]")
	}

	//start tags
		func parser(_ parser: XMLParser, didStartElement elementName: String, namespaceURI: String?, qualifiedName qName: String?, attributes attributeDict: [String : String] = [:]) {
			aT.lineNumber = parser.lineNumber
			switch elementName {
				case "titlePage":
					aT.titlePage += 1
				//	print("case titlePage")
				//	//data = attributeDict["title"] //this got an attibute inside of start tag
				case "title":
					aT.title += 1
					//data = attribueDict["name"]
					//print("case title")
				case "p":
					aT.p += 1
				case "list":
					aT.list += 1
				case "item":
					aT.item += 1
				case "reference":
					aT.reference += 1
				case "div":
					aT.div += 1
				case "table":
					aT.table += 1
				case "row":
					aT.row += 1
				case "cell":
					aT.cell += 1
				case "hi":
					aT.hi += 1
				case "foreign":
					aT.foreign += 1
				case "work":
					aT.work += 1
				case "creator":
					aT.creator += 1
				case "refSystem":
					aT.refSystem += 1
				case "figure":
					aT.figure += 1
				case "note":
					aT.note += 1
				case "header":
					aT.header += 1
				case "osisText":
					aT.osisText += 1
				case "osis":
					aT.osis += 1
				default:
					aT.other += 1
					aT.otherContent = elementName
					//print("case default elementName=\(elementName)")
					break
			}
			let tS = debugTags()
			print("\(self.depthIndent)start tag: \(elementName) tagStatus=[\(tS)]")
			self.depth += 1
		}

	//data inside tags
		func parser(_ parser: XMLParser, foundCharacters string: String) {
			aT.lineNumber = parser.lineNumber
			if (string.isEmpty || string == "\n") { print("\(self.depthIndent)tag data nil or nl; lineNumber=\(aT.lineNumber)") }
			else { print("\(self.depthIndent)string=\(string); lineNumber=\(aT.lineNumber)") }
		}

	//end tags
		func parser(_ parser: XMLParser, didEndElement elementName: String, namespaceURI: String?, qualifiedName qName: String?) {
			aT.lineNumber = parser.lineNumber
			switch elementName {
				case "titlePage":
					aT.titlePage -= 1
				//	print("case titlePage")
				//	//data = attributeDict["title"] //this got an attibute inside of start tag
				case "title":
					aT.title -= 1
					//data = attributeDict["name"]
					//print("case title")
				case "p":
					aT.p -= 1
				case "list":
					aT.list -= 1
				case "item":
					aT.item -= 1
				case "reference":
					aT.reference -= 1
				case "div":
					aT.div -= 1
				case "table":
					aT.table -= 1
				case "row":
					aT.row -= 1
				case "cell":
					aT.cell -= 1
				case "hi":
					aT.hi -= 1
				case "foreign":
					aT.foreign -= 1
				case "work":
					aT.work -= 1
				case "creator":
					aT.creator -= 1
				case "refSystem":
					aT.refSystem -= 1
				case "figure":
					aT.figure -= 1
				case "note":
					aT.note -= 1
				case "header":
					aT.header -= 1
				case "osisText":
					aT.osisText -= 1
				case "osis":
					aT.osis -= 1
				default:
					aT.other -= 1
					//print("case default elementName=\(elementName)")
					break
			}
			self.depth -= 1
			print("\(self.depthIndent)end tag: \(elementName); lineNumber=\(aT.lineNumber)")
		}
}



//deactivated code on hold
			/*
			if (elementName=="titlePage") {
				if (titlePage == false) {
					titlePage = true
				} else {
					titlePage = false
				}
			} else if (elementName == "title") {
				articleNth += 1
				//print("else if block for title")
				print("'\(elementName)' in the element number \(articleNth); data=\(data)")
			} else {
				print("else bock")
			}
			*/
			//print("\(self.depthIndent)<\(elementName)>")
			//print("\(self.depthIndent)\(qName ?? "n/a")") //default value was added for optional using ??
