#include <filesystem>
#include <iostream>
#include <string>

#include "catch.hpp"

#include "application/tools/alexarank/utilities.h"


TEST_CASE("Test alexa rank functions", "[alexarank]")
{
    // Good urls
    const std::string text = R"(
<!--  Need more Alexa data?  Find our APIs here: https://aws.amazon.com/alexa/  -->
<ALEXA VER="0.9" URL="google.com/" HOME="0" AID="=" IDN="google.com/">
<link type="text/css" id="dark-mode" rel="stylesheet" 
href="chrome-extension://cjjfaljaaljdjknnmhoajnmgbdcchmpp/content_script/general/dark_1.css"/>
<style type="text/css" id="dark-mode-custom-style"/>
<SD>
<POPULARITY URL="google.com/" TEXT="1" SOURCE="panel"/>
<REACH RANK="1"/>
<RANK DELTA="+0"/>
<COUNTRY CODE="US" NAME="United States" RANK="1"/>
</SD>
</ALEXA>
    )";
    auto rank = extract_alexa_rank(text);
    REQUIRE(rank == 1);
}

