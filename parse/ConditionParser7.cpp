#include "ConditionParserImpl.h"

#include "ParseImpl.h"
#include "ValueRefParser.h"
#include "ValueRefParserImpl.h"
#include "../universe/Condition.h"

#include <boost/spirit/include/phoenix.hpp>


namespace qi = boost::spirit::qi;
namespace phoenix = boost::phoenix;


#if DEBUG_CONDITION_PARSERS
namespace std {
    inline ostream& operator<<(ostream& os, const std::vector<ValueRef::ValueRefBase<StarType>*>&) { return os; }
}
#endif

namespace {
    struct condition_parser_rules_7 {
        condition_parser_rules_7() {
            const parse::lexer& tok = parse::lexer::instance();

            qi::_1_type _1;
            qi::_a_type _a;
            qi::_b_type _b;
            qi::_c_type _c;
            qi::_val_type _val;
            using phoenix::new_;
            using phoenix::push_back;

            ordered_bombarded_by
                =    tok.OrderedBombardedBy_
                >   -parse::detail::label(Condition_token) > parse::detail::condition_parser
                     [ _val = new_<Condition::OrderedBombarded>(_1) ]
                ;

            contains
                =    tok.Contains_
                >   -parse::detail::label(Condition_token) > parse::detail::condition_parser
                [ _val = new_<Condition::Contains>(_1) ]
                ;

            contained_by
                =    tok.ContainedBy_
                >   -parse::detail::label(Condition_token) > parse::detail::condition_parser
                [ _val = new_<Condition::ContainedBy>(_1) ]
                ;

            star_type
                =    tok.Star_
                >    parse::detail::label(Type_token)
                >    (
                            ('[' > +parse::detail::star_type_rules().expr [ push_back(_a, _1) ] > ']')
                        |    parse::detail::star_type_rules().expr [ push_back(_a, _1) ]
                     )
                [ _val = new_<Condition::StarType>(_a) ]
                ;

            location
                =   (tok.Location_
                >    parse::detail::label(Type_token) >
                    (
                        tok.Building_   [ _a = Condition::CONTENT_BUILDING ]
                    |   tok.Species_    [ _a = Condition::CONTENT_SPECIES ]
                    |   tok.Hull_       [ _a = Condition::CONTENT_SHIP_HULL ]
                    |   tok.Part_       [ _a = Condition::CONTENT_SHIP_PART ]
                    |   tok.Special_    [ _a = Condition::CONTENT_SPECIAL ]
                    |   tok.Focus_      [ _a = Condition::CONTENT_FOCUS ]
                    )
                >    parse::detail::label(Name_token)   > parse::string_value_ref() [ _b = _1 ]
                >  -(parse::detail::label(Name_token)   > parse::string_value_ref() [ _c = _1 ]))
                [ _val = new_<Condition::Location>(_a, _b, _c) ]
                ;

            owner_has_shippart_available
                =   (tok.OwnerHasShipPartAvailable_
                    >>  (parse::detail::label(Name_token)
                         > parse::string_value_ref() [ _val = new_<Condition::OwnerHasShipPartAvailable>(_1) ]
                        )
                    )
                ;

            start
                %=   ordered_bombarded_by
                |    contains
                |    contained_by
                |    star_type
                |    location
                |    owner_has_shippart_available
                ;

            ordered_bombarded_by.name("OrderedBombardedBy");
            contains.name("Contains");
            contained_by.name("ContainedBy");
            star_type.name("StarType");
            location.name("Location");
            owner_has_shippart_available.name("OwnerHasShipPartAvailable");

#if DEBUG_CONDITION_PARSERS
            debug(ordered_bombarded_by);
            debug(contains);
            debug(contained_by);
            debug(star_type);
            debug(location);
            debug(owner_has_shippart_available);
#endif
        }

        typedef parse::detail::rule<
            Condition::ConditionBase* (),
            qi::locals<std::vector<ValueRef::ValueRefBase<StarType>*>>
        > star_type_vec_rule;

        typedef parse::detail::rule<
            Condition::ConditionBase* (),
            qi::locals<
                Condition::ContentType,
                ValueRef::ValueRefBase<std::string>*,
                ValueRef::ValueRefBase<std::string>*
            >
        > string_ref_rule;

        parse::condition_parser_rule            ordered_bombarded_by;
        parse::condition_parser_rule            contains;
        parse::condition_parser_rule            contained_by;
        star_type_vec_rule                      star_type;
        string_ref_rule                         location;
        parse::condition_parser_rule            owner_has_shippart_available;
        parse::condition_parser_rule            start;
    };
}

namespace parse { namespace detail {
    const condition_parser_rule& condition_parser_7() {
        static condition_parser_rules_7 retval;
        return retval.start;
    }
} }
