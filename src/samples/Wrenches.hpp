#ifndef BASE_SAMPLES_WRENCHES_HPP
#define BASE_SAMPLES_WRENCHES_HPP

#include <NamedVector.hpp>
#include <Wrench.hpp>
#include <Time.hpp>

namespace base { namespace samples {

    /**
     * A named vector of base::WrenchState with sampled Time.
     */
    struct Wrenches : public base::NamedVector< base::Wrench >
    {
        base::Time time;
    };
}}

#endif


