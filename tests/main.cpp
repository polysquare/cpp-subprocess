/* /tests/main.cpp
 *
 * A test entrypoint which enables the use of the tap listener.
 *
 * See /LICENCE.md for Copyright information */

#define GTEST_TAP_PRINT_TO_STDOUT
#define GTEST_TAP_13_DIAGNOSTIC

#include <memory>  // IWYU pragma: keep
#include <string>  // IWYU pragma: keep
#include <tuple>  // IWYU pragma: keep
#include <vector>  // IWYU pragma: keep

#include <gtest/gtest.h>
#include <boost/program_options.hpp>   // IWYU pragma: keep
#include <boost/program_options/errors.hpp>               // for program_opt...
#include <boost/program_options/options_description.hpp>  // for options_des...
#include <boost/program_options/variables_map.hpp>        // for variables_map

#include <gtest-tap-listener-mocha/tap.h>

namespace po = boost::program_options;

namespace
{
    char const *PROGRAM_NAME = "test";

    std::tuple <int, std::vector <char const *>>
    GetUnrecognizedStrv (po::parsed_options const &parsed)
    {
        auto unrecognised = po::collect_unrecognized (parsed.options,
                                                      po::include_positional);

        std::vector <char const *> unrecognisedStrv;
        unrecognisedStrv.reserve (unrecognised.size () + 1);
        unrecognisedStrv.push_back (PROGRAM_NAME);

        for (auto &str : unrecognised)
        {
            unrecognisedStrv.push_back (str.c_str ());
        }

        int unrecognisedStrvLen = unrecognisedStrv.size ();

        return std::make_tuple (unrecognisedStrvLen, unrecognisedStrv);
    }
}

int main (int argc, char **argv)
{
    po::options_description desc ("CPP-Subprocess Tests Entrypoint");
    desc.add_options () ("reporter",
                         po::value <std::string> ()->default_value ("default"),
                         "Test reporter to use");

    po::variables_map program_variables;
    auto parsed = po::command_line_parser (argc, argv).options (desc)
                                                      .allow_unregistered ()
                                                      .run ();

    po::store (po::parse_command_line (argc, argv, desc), program_variables);
    po::notify (program_variables);

    std::unique_ptr <::testing::TestEventListener> listener;

    if (program_variables.count ("reporter"))
    {
        if (program_variables["reporter"].as <std::string> () ==
            std::string ("tap"))
        {
            listener.reset (new ::tap::mocha::TapListener ());
        }
    }

    std::vector <const char *> unrecognisedStrv;
    int unrecognisedStrvLen;

    std::tie (unrecognisedStrvLen,
              unrecognisedStrv) = GetUnrecognizedStrv (parsed);

    ::testing::InitGoogleTest (&argc, argv);

    auto &listeners = ::testing::UnitTest::GetInstance ()->listeners ();

    if (listener)
    {
        delete listeners.Release (listeners.default_result_printer ());
        listeners.Append (listener.release ());
    }
    

    return RUN_ALL_TESTS ();
}