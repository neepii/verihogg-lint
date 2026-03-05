#include "utils/ast_utils.h"

#include "Surelog/Common/FileSystem.h"
#include "Surelog/Library/Library.h"
#include "Surelog/ErrorReporting/ErrorContainer.h"

using namespace SURELOG;

NodeId findEnclosingModule(const FileContent* fC, NodeId node) {
  NodeId current = fC->Parent(node);
  while (current) {
    if (fC->Type(current) == VObjectType::paModule_declaration) return current;
    current = fC->Parent(current);
  }
  return InvalidNodeId;
}

bool isBuiltinClass(std::string className) {
  return (className == "semaphore" || className == "process" ||
          className == "mailbox");
}

std::string getStringConst(const FileContent* fC, NodeId id) {
  NodeId stringId = fC->sl_get(id, VObjectType::slStringConst);
  std::string str{fC->SymName(stringId)};
  return str;
}

std::string getPrefix(const FileContent* fC, NodeId id) {
  std::vector<std::string> contexts;
  std::string libName{fC->getLibrary()->getName()};
  NodeId tempId = fC->Parent(id);

  while (tempId) {
    NodeId intermediateTempId;
    VObjectType type = fC->Type(tempId);

    switch (type) {
      case VObjectType::paProgram_declaration: {
        intermediateTempId =
            fC->sl_get(tempId, VObjectType::paProgram_ansi_header);
        break;
      }
      case VObjectType::paInterface_declaration: {
        intermediateTempId =
            fC->sl_get(tempId, VObjectType::paInterface_ansi_header);
        intermediateTempId =
            fC->sl_get(intermediateTempId, VObjectType::paInterface_identifier);
        break;
      }
      case VObjectType::paModule_declaration: {
        intermediateTempId =
            fC->sl_get(tempId, VObjectType::paModule_ansi_header);
        break;
      }
      case VObjectType::paClass_declaration:
      case VObjectType::paPackage_declaration: {
        intermediateTempId = fC->sl_get(tempId, VObjectType::slStringConst);
        break;
      }
      default: {
        tempId = fC->Parent(tempId);
        continue;
      }
    }

    std::string stringConst = getStringConst(fC, intermediateTempId);
    contexts.push_back(stringConst);
    tempId = fC->Parent(tempId);
  }

  std::string result = "";
  for (size_t i = 0; i < contexts.size(); i++) {
    if (i > 0) result += "::";
    result += contexts[i];
  }

  return libName + "@" + result;
}

std::string getFullName(const FileContent* fC, NodeId id) {
  const std::string name = getStringConst(fC, id);
  const std::string prefix = getPrefix(fC, id);
  if (name == "") {
    return prefix;
  }
  return prefix + name;
}

