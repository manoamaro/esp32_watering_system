#include "utils.hpp"

namespace utils
{
    char *getMacAddress()
    {
        char *ssid = new char[23];
        snprintf(ssid, 23, "MCUDEVICE-%llX", ESP.getEfuseMac());
        return ssid;
    }
} // namespace utils