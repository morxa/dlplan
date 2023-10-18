#ifndef DLPLAN_INCLUDE_DLPLAN_COMMON_PARSERS_CONFIG_HPP_
#define DLPLAN_INCLUDE_DLPLAN_COMMON_PARSERS_CONFIG_HPP_

#include <boost/spirit/home/x3.hpp>
#include <boost/spirit/home/x3/support/ast/position_tagged.hpp>
#include <boost/spirit/home/x3/support/utility/error_reporting.hpp>


/// @brief Defines types of our parsers.
///        The configuration is relevant when reusing the parsers instantiated by the library.
///        The additional parsing context can be modified if additional information is needed.
namespace dlplan::common::parsers
{
    namespace x3 = boost::spirit::x3;

    // Our iterator type
    typedef std::string::const_iterator iterator_type;


    /* X3 error handler utility */
    template <typename Iterator>
    using error_handler = x3::error_handler<Iterator>;

    using error_handler_tag = x3::error_handler_tag;

    typedef error_handler<iterator_type> error_handler_type;


    /* In case we need to provide context in the future. */
    struct parsing_context_tag;

    struct parsing_context_type {
        bool error_reported = false;
    };

    /* The phrase parse context */
    typedef
        x3::phrase_parse_context<x3::ascii::space_type>::type
    phrase_context_type;


    /* Combined error handler, parsing, and phrase parse Context */
    typedef x3::context<
            error_handler_tag,
            std::reference_wrapper<error_handler_type>,
            x3::context<
                parsing_context_tag,
                std::reference_wrapper<parsing_context_type>,
                phrase_context_type>>
    context_type;

}

#endif