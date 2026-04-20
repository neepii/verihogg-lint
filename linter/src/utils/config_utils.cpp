#include "utils/config_utils.h"

#include <Surelog/Common/NodeId.h>
#include <Surelog/Design/Design.h>
#include <Surelog/Design/FileContent.h>
#include <Surelog/SourceCompile/VObjectTypes.h>

#include <string>
#include <vector>

#include "utils/ast_utils.h"
#include "utils/design_utils.h"

namespace ConfigUtils {

auto CollectAllConfig(SL::Design* design) -> std::vector<ConfigInfo> {
  std::vector<ConfigInfo> allConfigs;

  DesignUtils::ForEachFileContent(design, [&](const SL::FileContent* fileCont) {
    SL::NodeId const kRoot = fileCont->getRootNode();
    if (!kRoot) {
      return;
    }

    for (SL::NodeId const kConfigDecl : fileCont->sl_collect_all(
             kRoot, SL::VObjectType::paConfig_declaration)) {
      std::string const kFullName = GetFullName(fileCont, kConfigDecl);
      allConfigs.push_back(ConfigInfo{
          .nodeid = kConfigDecl,
          .name = kFullName,
          .fileContent = fileCont,
      });
    }
  });

  return allConfigs;
}
}  // namespace ConfigUtils
