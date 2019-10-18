#include <XRFunction.h>
#include <string>
#include <vector>

class XRRowDescription {
 private:
  const std::string name;
//   std::vector<XRFunction> functions;

 public:
  XRRowDescription(std::string name) : name(){};

  std::string rowName() { return name; }
};
