#include <string>
#include <vector>

#include "XRFunction.h"
#include "XRRowDescription.h"

XRRowDescription::XRRowDescription(const std::string name) : name(){};

const std::string XRRowDescription::rowName() const { return name; }
