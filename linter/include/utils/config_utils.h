#include <Surelog/Design/Design.h>

#include <vector>

struct ConfigInfo {
  SURELOG::NodeId nodeid;
  std::string name;
  const SURELOG::FileContent* fileContent;
};

namespace ConfigUtils {

auto CollectAllConfig(SURELOG::Design* design) -> std::vector<ConfigInfo>;

}  // namespace ConfigUtils
