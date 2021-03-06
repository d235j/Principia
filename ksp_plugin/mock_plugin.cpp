#include "ksp_plugin/mock_plugin.hpp"

#include <vector>

#include "gmock/gmock.h"

namespace principia {
namespace ksp_plugin {

MockPlugin::MockPlugin()
    : Plugin(Instant(),
      Index(0),
      1 * SIUnit<GravitationalParameter>(),
      Angle()) {}

not_null<std::unique_ptr<RenderingTransforms>>
MockPlugin::NewBodyCentredNonRotatingTransforms(
    Index const reference_body_index) const {
  std::unique_ptr<RenderingTransforms> transforms;
  FillBodyCentredNonRotatingTransforms(reference_body_index, &transforms);
  return std::move(transforms);
}

not_null<std::unique_ptr<RenderingTransforms>>
MockPlugin::NewBarycentricRotatingTransforms(
    Index const primary_index,
    Index const secondary_index) const {
  std::unique_ptr<RenderingTransforms> transforms;
  FillBarycentricRotatingTransforms(primary_index,
                                    secondary_index,
                                    &transforms);
  return std::move(transforms);
}

void MockPlugin::AddVesselToNextPhysicsBubble(
    GUID const& vessel_guid,
    std::vector<IdAndOwnedPart> parts) {
  AddVesselToNextPhysicsBubbleConstRef(vessel_guid, parts);
}

}  // namespace ksp_plugin
}  // namespace principia
