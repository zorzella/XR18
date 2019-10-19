#include <OSCMessage.h>  // For OSC support
#include <string>

#include "XRFunction.h"

// class XRFunction {
//  private:

//  public:
XRFunction::XRFunction(const std::string oscAddr, const std::string name)
    : m_name(), m_oscAddr() {}

const std::string XRFunction::name() { return m_name; }

const std::string XRFunction::oscAddr() { return m_oscAddr; }

void plus(OSCMessage &outParam, OSCMessage &source) {
  // source.getAddress(buffer);
  outParam = source;
  // if (source.ge)
  // outParam
}
// };