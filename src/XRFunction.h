#include <string>

class XRFunction {
 private:
  const std::string m_name;
  const std::string m_oscAddr;

 public:
  const std::string oscAddr();
  const std::string name();
  XRFunction(const std::string oscAddr, const std::string name);
};