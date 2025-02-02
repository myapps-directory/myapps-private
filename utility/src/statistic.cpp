// myapps/private/utility/src/statistic.cpp

// This file is part of MyApps.directory project
// Copyright (C) 2020, 2021, 2022, 2023, 2024, 2025 Valentin Palade (vipalade @ gmail . com)

// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// at your option any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <https://www.gnu.org/licenses/>.

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
