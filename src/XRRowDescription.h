// #include "XRFunction.h"
#include <string>
#include <vector>

class XRRowDescription {
 private:
   const std::string name;

 public:
  XRRowDescription(const std::string name);

  std::string rowName();
};
