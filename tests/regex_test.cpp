#include <iostream>
#include <regex>
#include <string>

int main()
{
    std::string test_file = R"(
#version 330

vec4 pattern(vec2 vec)
{
    return vec4(vec, vec);
}

#include "noise_.aA997-common.vert"

int main()
{
    vec2 somevec = (1,2);
    vec4 avec = pattern(somevec);

})";

    std::string replace_file = R"(
vec10000 replacing_this_bitch(vec99)
{
    asdf
    asdf
    asdf
    asdf
    asdf
    asdf
    asdf;
}
)";

    std::string match_pattern_for_include = R"(#include)";

    std::cout << match_pattern_for_include << std::endl;
    std::cout << test_file << std::endl;
    std::cout << replace_file << std::endl;

    std::regex re_pattern_for_include(match_pattern_for_include);
    std::smatch re_match_for_include;

    if (std::regex_search(test_file, re_match_for_include, re_pattern_for_include))
    {
        std::cout << "Found #include statement" << std::endl;

        std::string match_pattern_for_file = R"(#include \"([a-zA-Z0-9\.\-_]+)\")";
        std::cout << match_pattern_for_file << std::endl;

        std::regex re_pattern_for_file(match_pattern_for_file);
        std::smatch re_match_for_file;

        if (std::regex_search(test_file, re_match_for_file, re_pattern_for_file))
        {
            std::cout << re_match_for_file[0] << std::endl;
            std::cout << re_match_for_file[1] << std::endl;

            std::string test_code =
                test_file.substr(0, re_match_for_file.position(0)) +
                replace_file +
                test_file.substr(re_match_for_file.position(0) + re_match_for_file.length(0));

            std::cout << test_code << std::endl;
        }
        else
        {
            std::cout << "Failed to find filename" << std::endl;
        }
    }
    else
    {
        std::cout << "Failed to find #include" << std::endl;
    }

    return 0;
}