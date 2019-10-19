#ifndef XR_ROW_DESCRIPTION_h
#define XR_ROW_DESCRIPTION_h

#include <string>
#include <vector>

#include "XRFunctionDescription.h"

class XRRowDescription {
 private:
   const std::string m_name;
   const std::vector<XRFunctionDescription> m_funcs;

 public:
  XRRowDescription(const std::string name, const std::vector<XRFunctionDescription> funcs);

  const std::string rowName() const;
  const std::vector<XRFunctionDescription> funcs();
};

#endif