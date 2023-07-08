import XCTest
@testable import ex1



final class bdtFile: XCTestCase {
    func testExample() throws {
		XMLTest().test(url: URL(fileURLWithPath: "modules/genbook/rawgenbook/baptistconfession1646/baptistconfession1646.bdt"))


    }
}
