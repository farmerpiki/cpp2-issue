
module;
#define CPP2_USE_SOURCE_LOCATION Yes
#define CPP2_USE_MODULES         Yes

#include "cpp2util.h"

export module pof;
import personal;

//=== Cpp2 type declarations ====================================================


#line 4 "/home/g-zu/projects/proofoffailure/public.pure.cpp2"
class private_shit;
 

#line 12 "/home/g-zu/projects/proofoffailure/public.pure.cpp2"
export class handle;
 

//=== Cpp2 type definitions and function declarations ===========================


#line 4 "/home/g-zu/projects/proofoffailure/public.pure.cpp2"
class private_shit {
 private: cpp2::u64 io_context; 

 public: explicit private_shit();
  
 public: private_shit(private_shit const&) = delete; /* No 'that' constructor, suppress copy */
 public: auto operator=(private_shit const&) -> void = delete;


#line 10 "/home/g-zu/projects/proofoffailure/public.pure.cpp2"
};

class handle {
 private: std::unique_ptr<private_shit> d {cpp2_new<private_shit>()}; 

 public: explicit handle();

        public: handle(handle const&) = delete; /* No 'that' constructor, suppress copy */
        public: auto operator=(handle const&) -> void = delete;
#line 16 "/home/g-zu/projects/proofoffailure/public.pure.cpp2"
};


//=== Cpp2 function definitions =================================================


#line 7 "/home/g-zu/projects/proofoffailure/public.pure.cpp2"
 private_shit::private_shit()
  : io_context{ 42 }
#line 7 "/home/g-zu/projects/proofoffailure/public.pure.cpp2"
 {

 }

#line 15 "/home/g-zu/projects/proofoffailure/public.pure.cpp2"
 handle::handle(){}

