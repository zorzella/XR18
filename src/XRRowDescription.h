#ifndef XR_ROW_DESCRIPTION_h
#define XR_ROW_DESCRIPTION_h

#include <string>
#include <vector>

class XRRowDescription {
 private:
   const std::string name;

 public:
  XRRowDescription(const std::string name);

  const std::string rowName() const;
};

#endif