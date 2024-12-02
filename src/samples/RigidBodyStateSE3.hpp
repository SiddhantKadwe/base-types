#ifndef BASE_SAMPLES_RIGID_BODY_STATE_SE3_HPP
#define BASE_SAMPLES_RIGID_BODY_STATE_SE3_HPP

#include <RigidBodyStateSE3.hpp>
#include <Time.hpp>

namespace base{ namespace samples {

/** RigidBodyStateSE3 with sampled time and frame ID*/
struct RigidBodyStateSE3 : public base::RigidBodyStateSE3
{
    base::Time time;
    /** Common coordinate frame for all quantities of the rigid body */
    std::string frame_id;
};

}}

#endif
