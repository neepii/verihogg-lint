#include <gtest/gtest.h>

#include <filesystem>

#include "main/lint_rules.h"
#include "rules/invalid_liblist.h"
#include "utils.h"

namespace fs = std::filesystem;

namespace {
auto BasePath() -> const fs::path {
  return fs::current_path() / ".." / ".." / "tests" / "Configurations";
}

TEST(InvalidLiblistTest, NoError) {
  const fs::path tests_path{BasePath() / "InvalidLiblist" / "NoError"};

  test::CheckWithNoErrorsExpected(tests_path, CheckInvalidLiblist);
}

TEST(InvalidLiblistTest, RaiseError) {
  const fs::path tests_path{BasePath() / "InvalidLiblist" / "RaiseError"};

  std::unordered_set<SURELOG::ErrorDefinition::ErrorType> ignoreList{
      SURELOG::ErrorDefinition::COMP_UNDEFINED_BASE_CLASS};

  test::CheckWithErrorsExpected(tests_path, verihogg_lint::LINT_INVALID_LIBLIST,
                                ignoreList, CheckInvalidLiblist);
}
}  // namespace
auto main(int argc, char** argv) -> int {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
