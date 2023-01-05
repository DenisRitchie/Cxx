#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include <Cxx/CompilerFeature.hpp>

using namespace Cxx::FeatureTesting;

TEST(LanguageFeatures, LanguageFeatures)
{
  // clang-format off

  if constexpr (PrintOptions::general_features) Show("C++ GENERAL", cxx_core);
  if constexpr (PrintOptions::cxx11 and PrintOptions::core_features) Show("C++11 CORE", cxx11_core);
  if constexpr (PrintOptions::cxx14 and PrintOptions::core_features) Show("C++14 CORE", cxx14_core);
  if constexpr (PrintOptions::cxx14 and PrintOptions::lib_features ) Show("C++14 LIB" , cxx14_lib);
  if constexpr (PrintOptions::cxx17 and PrintOptions::core_features) Show("C++17 CORE", cxx17_core);
  if constexpr (PrintOptions::cxx17 and PrintOptions::lib_features ) Show("C++17 LIB" , cxx17_lib);
  if constexpr (PrintOptions::cxx20 and PrintOptions::core_features) Show("C++20 CORE", cxx20_core);
  if constexpr (PrintOptions::cxx20 and PrintOptions::lib_features ) Show("C++20 LIB" , cxx20_lib);
  if constexpr (PrintOptions::cxx23 and PrintOptions::core_features) Show("C++23 CORE", cxx23_core);
  if constexpr (PrintOptions::cxx23 and PrintOptions::lib_features ) Show("C++23 LIB" , cxx23_lib);
  if constexpr (PrintOptions::cxx26 and PrintOptions::core_features) Show("C++26 CORE", cxx26_core);
  if constexpr (PrintOptions::cxx26 and PrintOptions::lib_features ) Show("C++26 LIB" , cxx26_lib);
  if constexpr (PrintOptions::attributes) Show("ATTRIBUTES", attributes);

  // clang-format on
}
