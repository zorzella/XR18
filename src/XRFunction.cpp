#include <OSCMessage.h>  // For OSC support
#include <string>

// class XRFunction {
//  private:
  const std::string m_name;
  const std::string m_oscAddr;

//  public:
  XRFunction::XRFunction() : m_name(), m_oscAddr() {}

  const std::string name() { return m_name; }

  const std::string oscAddr() { return m_oscAddr; }

  void plus(OSCMessage &outParam, OSCMessage &source) {
    // source.getAddress(buffer);
    outParam = source;
    // if (source.ge)
    // outParam
  }
// };