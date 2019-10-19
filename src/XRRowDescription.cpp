#include <string>
#include <vector>

#include "XRFunction.h"
#include "XRRowDescription.h"

// class XRRowDescription {
//  private:
//   const std::string name;
//   std::vector<XRFunction> functions;

//  public:
XRRowDescription::XRRowDescription(const std::string name) : name(){};

std::string XRRowDescription::rowName() { return name; }
// };
