#include "rules/invalid_liblist.h"

#include <Surelog/Common/NodeId.h>
#include <Surelog/Design/FileContent.h>
#include <Surelog/ErrorReporting/ErrorContainer.h>
#include <Surelog/SourceCompile/SymbolTable.h>
#include <Surelog/SourceCompile/VObjectTypes.h>

#include "main/lint_rules.h"
#include "utils/ast_utils.h"
#include "utils/location_utils.h"

namespace SL = SURELOG;

void CheckInvalidLiblist(const SL::FileContent* fileContent,
                         SL::ErrorContainer* errors, SL::SymbolTable* symbols) {
  if (fileContent == nullptr || errors == nullptr || symbols == nullptr) {
    return;
  }

  SL::NodeId const kRoot = fileContent->getRootNode();
  if (!kRoot) {
    return;
  }

  for (SL::NodeId const kConfigDeclId : fileContent->sl_collect_all(
           kRoot, SL::VObjectType::paConfig_declaration)) {
    for (SL::NodeId const kLiblistId : fileContent->sl_collect_all(
             kConfigDeclId, SL::VObjectType::paLiblist_clause)) {
      SL::NodeId const kChild =
          fileContent->sl_get(kLiblistId, SL::VObjectType::slStringConst);
      if (kChild) {
        continue;
      }
      std::string const kConfigName =
          GetStringConst(fileContent, kConfigDeclId);
      ReportError(fileContent, kConfigDeclId, kConfigName,
                  verihogg_lint::LINT_INVALID_LIBLIST, errors, symbols);
    }
  }
}
