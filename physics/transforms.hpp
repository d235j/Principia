#pragma once

#include <functional>
#include <map>
#include <memory>
#include <utility>
#include <vector>

#include "base/not_null.hpp"
#include "physics/frame_field.hpp"
#include "physics/trajectory.hpp"

namespace principia {

using base::not_null;

namespace physics {

// This class represent a pair of transformations of a trajectory from
// |FromFrame| to |ToFrame| with an intermediate representation in
// |ThroughFrame|.  Note that the trajectory in |ToFrame| is not the trajectory
// of a body since its past changes from moment to moment.  The type |Mobile|
// holds one or more trajectories which are selected using a |LazyTrajectory|.
template<typename Mobile,
         typename FromFrame, typename ThroughFrame, typename ToFrame>
class Transforms {
  static_assert(FromFrame::is_inertial && ToFrame::is_inertial,
                "Both FromFrame and ToFrame must be inertial");

 public:
  // The trajectories are evaluated lazily because they may be extended or
  // deallocated/reallocated between the time when the transforms are created
  // and the time when they are applied.  Thus, the lambdas couldn't capture the
  // trajectories by value nor by reference.  Instead, they capture an |Mobile|
  // by reference and a pointer-to-member function by copy.
  // This technique also makes it possible to dynamically select the trajectory
  // that's used for the |Mobile|: it is the one denoted by the same member
  // function that was passed to |first| or |first_on_or_after|.
  template<typename Frame>
  using LazyTrajectory = Trajectory<Frame> const& (Mobile::*)() const;

  // A factory method where |ThroughFrame| is defined as follows: it has the
  // same axes as |FromFrame| and the body of |centre_trajectory| is the origin
  // of |ThroughFrame|.
  static not_null<std::unique_ptr<Transforms>> BodyCentredNonRotating(
      Mobile const& centre,
      LazyTrajectory<ToFrame> const& to_trajectory);

  // A factory method where |ThroughFrame| is defined as follows: its X axis
  // goes from the primary to the secondary bodies, its Y axis is in the plane
  // of the velocities of the bodies in their barycentric frame, on the same
  // side of the X axis as the velocity of the primary body, its Z axis is such
  // that it is right-handed.  The barycentre of the bodies is the origin of
  // |ThroughFrame|.
  static not_null<std::unique_ptr<Transforms>> BarycentricRotating(
      Mobile const& primary,
      Mobile const& secondary,
      LazyTrajectory<ToFrame> const& to_trajectory);

  // Use this only for testing!
  static not_null<std::unique_ptr<Transforms>> DummyForTesting();

  // Indicates that the given |trajectory| is cacheable (for all |Mobile|
  // objects).  By default, lazy trajectories are not cacheable.
  void set_cacheable(LazyTrajectory<FromFrame> const& trajectory);

  typename Trajectory<FromFrame>::template TransformingIterator<ThroughFrame>
  first(Mobile const& mobile,
        LazyTrajectory<FromFrame> const& from_trajectory);

  typename Trajectory<FromFrame>::template TransformingIterator<ThroughFrame>
  first_on_or_after(Mobile const& mobile,
                    LazyTrajectory<FromFrame> const& from_trajectory,
                    Instant const& time);

  typename Trajectory<ThroughFrame>:: template TransformingIterator<ToFrame>
  second(Trajectory<ThroughFrame> const& through_trajectory);

  // The coordinate frame of |ThroughFrame|, expressed in the coordinates of
  // |ToFrame| at the current time.
  FrameField<ToFrame> coordinate_frame() const;

 private:
  // Just like a |Trajectory::Transform|, except that the first parameter is
  // only bound when we know which trajectory to extract from the |Mobile|.
  template<typename Frame1, typename Frame2>
  using LazyTransform = std::function<DegreesOfFreedom<Frame2>(
                            LazyTrajectory<Frame1> const&,
                            Instant const&,
                            DegreesOfFreedom<Frame1> const&,
                            not_null<Trajectory<Frame1> const*> const)>;

  LazyTransform<FromFrame, ThroughFrame> first_;
  typename Trajectory<ThroughFrame>::template Transform<ToFrame> second_;

  // A simple cache with no eviction, which monitors the hit rate.
  template<typename Frame1, typename Frame2>
  class Cache {
   public:
    bool Lookup(not_null<Trajectory<Frame1> const*> const trajectory,
                Instant const& time,
                not_null<DegreesOfFreedom<Frame2>**> degrees_of_freedom);

    void Insert(not_null<Trajectory<Frame1> const*> const trajectory,
                Instant const& time,
                DegreesOfFreedom<Frame2> const& degrees_of_freedom);

   private:
    std::map<std::pair<not_null<Trajectory<Frame1> const*>, Instant const>,
             DegreesOfFreedom<Frame2>> map_;
    std::map<not_null<Trajectory<Frame1> const*>, std::int64_t>
        number_of_lookups_;
    std::map<not_null<Trajectory<Frame1> const*>, std::int64_t> number_of_hits_;
  };

  // Using a vector, not a set, because (1) this is small and (2) writing a
  // comparator or a hasher for |LazyTrajectory| is complicated.
  std::vector<LazyTrajectory<FromFrame>> cacheable_;

  // A cache for the result of the |first_| transform.  This cache assumes that
  // the iterator is never called with the same time but different degrees of
  // freedom.
  Cache<FromFrame, ThroughFrame> first_cache_;

  FrameField<ToFrame> coordinate_frame_;
};

}  // namespace physics
}  // namespace principia

#include "physics/transforms_body.hpp"
