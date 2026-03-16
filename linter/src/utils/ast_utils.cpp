#include "utils/ast_utils.h"

#include <iostream>
#include <sstream>

#include "Surelog/Library/Library.h"

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
  if (stringId == zeroId) return "";
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

  if (contexts.size() > 0) {
    result += "::";
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

std::unordered_map<std::string, NodeId> getClassIds(const FileContent* fC) {
  std::unordered_map<std::string, NodeId> classes;
  const std::vector<NodeId> classDeclarations =
      fC->sl_collect_all(fC->getRootNode(), VObjectType::paClass_declaration);

  for (NodeId classId : classDeclarations) {
    const std::string className = getFullName(fC, classId);
    if (isBuiltinClass(className)) continue;
    classes[className] = classId;
  }
  return classes;
}

std::string removeFilePrefix(std::string str) {
  size_t i = 0;
  while (str[i++] != '@');
  return std::string(str).substr(i, str.size());
}

std::vector<std::string> getClassScope(const FileContent* fC,
                                       NodeId funcBodyId) {
  std::cout << fC->printSubTree(funcBodyId) << std::endl;
  std::vector<std::string> scopes;
  const NodeId classScopeId =
      fC->sl_collect(funcBodyId, VObjectType::paClass_scope);
  const NodeId classTypeId =
      fC->sl_get(classScopeId, VObjectType::paClass_type);

  if (classTypeId == zeroId) return scopes;
  const std::vector<NodeId> ids =
      fC->sl_collect_all(funcBodyId, VObjectType::slStringConst);

  for (auto& id : ids) {
    std::string str{fC->SymName(id)};
    scopes.push_back(str);
  }

  return scopes;
}

std::unordered_set<std::string> getInterfaceClassSet(const FileContent* fC) {
  const std::vector<NodeId> interfaceClassDeclarations = fC->sl_collect_all(
      fC->getRootNode(), VObjectType::paInterface_class_declaration);
  std::unordered_set<std::string> interfaceClassSet;
  for (auto& interfaceClass : interfaceClassDeclarations) {
    std::string interfaceClassName = getStringConst(fC, interfaceClass);
    interfaceClassSet.insert(interfaceClassName);
  }
  return interfaceClassSet;
}

std::unordered_set<std::string> getClassSet(const FileContent* fC) {
  const std::vector<NodeId> classDeclarations =
      fC->sl_collect_all(fC->getRootNode(), VObjectType::paClass_declaration);
  std::unordered_set<std::string> classSet;
  for (auto& classId : classDeclarations) {
    std::string className = getStringConst(fC, classId);
    classSet.insert(className);
  }
  return classSet;
}

std::string getFullNameFromScope(const FileContent* fC, NodeId id) {
  std::stringstream sstream;
  std::string prefix = getPrefix(fC, id);
  sstream << prefix;

  const NodeId tempId = fC->sl_get(id, VObjectType::paClass_type);
  const std::vector<NodeId> strIds =
      fC->sl_collect_all(tempId, VObjectType::slStringConst);
  assert(strIds.size() > 0);

  std::string firstString{fC->SymName(strIds[0])};
  sstream << firstString;

  for (size_t i = 1; i < strIds.size(); i++) {
    const NodeId stringId = strIds[i];
    std::string scopeName{fC->SymName(stringId)};
    sstream << "::" << scopeName;
  }
  return sstream.str();
}

std::string getSuperclassString(const FileContent* fC, NodeId id) {
  assert(fC->Type(id) != VObjectType::paClass_declaration);

  const NodeId classType = fC->sl_get(id, VObjectType::paClass_type);
  if (classType == zeroId) return "";
  return getStringConst(fC, classType);
}
