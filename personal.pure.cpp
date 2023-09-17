
module;
#define CPP2_USE_SOURCE_LOCATION Yes
#define CPP2_USE_MODULES         Yes

#include "cpp2util.h"

export module personal;

//=== Cpp2 type declarations ====================================================



//=== Cpp2 type definitions and function declarations ===========================


#line 3 "/home/g-zu/projects/proofoffailure/personal.pure.cpp2"
export [[nodiscard]] auto read_regular_file(cpp2::in<std::filesystem::path> path) -> std::optional<std::string>;
  

//=== Cpp2 function definitions =================================================


#line 3 "/home/g-zu/projects/proofoffailure/personal.pure.cpp2"
[[nodiscard]] auto read_regular_file(cpp2::in<std::filesystem::path> path) -> std::optional<std::string>{
  std::ifstream t {path}; 
  if (!(CPP2_UFCS_0(is_open, t))) {
    std::cerr << "failed to read file contents: " + cpp2::to_string(path) + "\n";
    return std::nullopt; 
  }
  std::stringstream buffer {}; 
  buffer << CPP2_UFCS_0(rdbuf, std::move(t));

  return CPP2_UFCS_0(str, std::move(buffer)); 
}

