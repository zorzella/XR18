#include <OSCMessage.h>  // For OSC support
#include <string>

class XRFunction {
 private:
  const std::string m_oscAddr;

 public:
  XRFunction() : m_oscAddr() {}
  std::string oscAddr() { return m_oscAddr; }

  void plus(OSCMessage &outParam, OSCMessage &source) {
    // source.getAddress(buffer);
    outParam = source;
    // if (source.ge)
    // outParam
  }
};