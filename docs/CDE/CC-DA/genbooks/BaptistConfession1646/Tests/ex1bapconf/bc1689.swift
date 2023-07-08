import XCTest
@testable import ex1



final class bc1689: XCTestCase {
    func testExample() throws {
        // The test attempts to run swift argument parser for variables approximately similar to diatheke.
        // results.
        //let d = cli.parseOrExit()
        // Use XCTAssert might verify your tests produce the correct
        //XCTAssertEqual(theStruct.parseOrExit(), "Hello, World!")


		//This was copied out of the example ...
		// This path is not considered API; it was just a convenient XML file
		// lying around on my hard disk that I used for this example.

		//XMLTest().test(url: URL(fileURLWithPath: "/home2/usr/local/bible/swift/draft/xmlparser/Tests/ex1bapconf/tutorial.xml")) //not a baptist confession
		XMLTest().test(url: URL(fileURLWithPath: "Tests/sword/modules/genbook/rawgenbook/baptistconfession1689/baptistconfession1689.bdt"))

    }
}

//copied from another example test, diatheke
//import XCTest
//@testable import diatheke

//final class test1: XCTestCase {
//    func testExample() throws {
//        // The test attempts to run swift argument parser for variables approximately similar to diatheke.
//        // results.
//        let d = cli.parseOrExit()
//        // Use XCTAssert might verify your tests produce the correct
//        //XCTAssertEqual(theStruct.parseOrExit(), "Hello, World!")
//    }
//}

