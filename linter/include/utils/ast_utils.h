#pragma once

#include <cstdint>
#include <string>

#include "Surelog/Design/FileContent.h"
#include "Surelog/ErrorReporting/ErrorContainer.h"
#include "Surelog/SourceCompile/SymbolTable.h"
#include "Surelog/SourceCompile/VObjectTypes.h"

using namespace SURELOG;

const NodeId zeroId = NodeId(InvalidRawNodeId);

NodeId findEnclosingModule(const FileContent* fC, NodeId node);

bool isBuiltinClass(std::string className);

std::string getStringConst(const FileContent* fC, NodeId id);

std::string getPrefix(const FileContent* fC, NodeId id);

std::string getFullName(const FileContent* fC, NodeId id);
