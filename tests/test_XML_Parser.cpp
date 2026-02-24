#include <catch2/catch_test_macros.hpp>

#include "../XML/XML_Parser.hpp"
#include "../Types/TY_Blob.hpp"
#include "../Misc/SystemMessageError.hpp"

#include <xercesc/util/PlatformUtils.hpp>

#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>

namespace fs = std::filesystem;

// Initialize Xerces
struct XercesGuard {
    XercesGuard() { xercesc::XMLPlatformUtils::Initialize(); }
    ~XercesGuard() { xercesc::XMLPlatformUtils::Terminate(); }
};

/**
 * Ensure xerces is initialized for the tests.
 */
static void ensure_xerces() {
    static XercesGuard guard{};
}

TEST_CASE("XML_Parser ignores (wrong) XML declaration", "[xml][parser]") {
    ensure_xerces();

    const char* xml =
        R"(<?xml version="1.0" encoding="UTF-8?"?>
           <Functions>
             <Function>
                <ID>stringLength...f2128203875h-1761480648.5_1</ID>
                <Source>0.1</Source>
            </Function>
           </Functions>)";

    TY_Blob blob(xml, std::strlen(xml));
    XML_Parser parser_ignore_header(false, false, true);
    REQUIRE(parser_ignore_header.parseBlob(&blob));
    try {
        XML_Parser parser_default;
        parser_default.parseBlob(&blob);
        FAIL("Expected M_SystemMessage to be thrown");
    }
    catch (const SystemMessageError& error) {
        const auto& msg = error.message();
        CHECK(std::string(msg.getCode()) == "XMLLM_XR_REPOSITORYPARSER_NATIVE_FATAL_PARSE_ERROR");
        CHECK(std::string(msg.getDescription()).find("invalid XML encoding declaration 'UTF-8?'") != std::string::npos);
    }
}


class AttrLongTestParser final : public XML_Parser {
public:
    long attrValue = 0;
    bool foundTag = false;
    bool attrLongExists = false;

    bool startElementChar(const char* /*uri*/,
                          const char* /*localName*/,
                          const char* qName,
                          const xercesc::Attributes& attrs) override
    {
        if (std::string_view{qName} == "Functions") {
            foundTag = true;
            attrValue = getAttributeLong(attrs, "count", attrLongExists);
        }
        return true;
    }
};

TEST_CASE("XML_Parser accepts valid attribute value", "[xml][parser]") {
    ensure_xerces();

    const char* xml =
        R"(<Functions count="1000"></Functions>)";

    TY_Blob blob(xml, std::strlen(xml));

    AttrLongTestParser parser;
    parser.parseBlob(&blob);
    REQUIRE(parser.parseBlob(&blob));
    REQUIRE(parser.attrLongExists);
    REQUIRE(parser.attrValue == 1000);
}

TEST_CASE("XML_Parser ignores no attribute", "[xml][parser]") {
    ensure_xerces();

    const char* xml =
        R"(<Functions></Functions>)";

    TY_Blob blob(xml, std::strlen(xml));

    AttrLongTestParser parser;
    parser.parseBlob(&blob);
    REQUIRE(parser.parseBlob(&blob));
    REQUIRE_FALSE(parser.attrLongExists);
    REQUIRE_FALSE(parser.attrValue);
}

TEST_CASE("XML_Parser rejects invalid attribute value", "[xml][parser]") {
    ensure_xerces();

    const char* xml =
        R"(<Functions count="abc"></Functions>)";

    TY_Blob blob(xml, std::strlen(xml));

    AttrLongTestParser parser;
    parser.parseBlob(&blob);
    REQUIRE(parser.parseBlob(&blob));
    REQUIRE_FALSE(parser.attrLongExists); // fail!
    REQUIRE_FALSE(parser.attrValue);
}

TEST_CASE("XML_Parser ignores empty attribute value", "[xml][parser]") {
    ensure_xerces();

    const char* xml =
        R"(<Functions count=""></Functions>)";

    TY_Blob blob(xml, std::strlen(xml));

    AttrLongTestParser parser;
    parser.parseBlob(&blob);
    REQUIRE(parser.parseBlob(&blob));
    REQUIRE_FALSE(parser.attrLongExists); // fail!
    REQUIRE_FALSE(parser.attrValue);
}


