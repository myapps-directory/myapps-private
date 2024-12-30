#include "myapps/private/utility/statistic.hpp"
#include <iomanip>
#include <ostream>
#include <string>

using namespace std;

namespace myapps {
namespace utility {
namespace statistic {

std::ostream& print(std::ostream& _ros, uint64_t _value, const std::string& _type_def, const std::string& _unit)
{
    if (_type_def[1] == '\0') {
        _ros << _value;

        if (!_unit.empty()) {
            _ros << ' ' << _unit;
        }
    } else if (_type_def[1] == 'f') {
        auto val  = _type_def.substr(2);
        auto uval = stoul(val);
        _ros << _value / uval << '.' << std::setfill('0') << std::setw(val.size() - 1) << _value % uval;
    } else {
        _ros << _value << '!';
    }
    return _ros;
}

std::ostream& print(std::ostream& _ros, const std::string& _value, const std::string& /*_type_def*/, const std::string& /*_unit*/)
{
    _ros << _value;
    return _ros;
}

} // namespace statistic
} // namespace utility
} // namespace myapps
