#include <string>
#include <vector>

#include "XRFunction.h"
#include "XRRowDescription.h"

XRRowDescription::XRRowDescription(
    const std::string name, const std::vector<XRFunctionDescription> funcs)
    : m_name(name), m_funcs(funcs){};

const std::string XRRowDescription::rowName() const { return m_name; }
const std::vector<XRFunctionDescription> XRRowDescription::funcs() { return m_funcs; }
