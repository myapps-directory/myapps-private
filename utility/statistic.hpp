// myapps/private/utility/statistic.hpp

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

#pragma once

#include "solid/reflection/reflection.hpp"
#include "solid/system/exception.hpp"
#include "solid/utility/common.hpp"
#include <string>
#include <vector>

namespace myapps {
namespace utility {
namespace statistic {

struct Description {
    std::string name_;
    std::string type_def_;
    std::string unit_;
    std::string description_;

    Description(
        const std::string& _name = "", const std::string _type_def = "", const std::string _unit = "", const std::string& _description = "")
        : name_(_name)
        , type_def_(_type_def)
        , unit_(_unit)
        , description_(_description)
    {
    }

    SOLID_REFLECT_V1(_r, _rthis, _rctx)
    {
        _r.add(_rthis.name_, _rctx, 1, "name");
        _r.add(_rthis.type_def_, _rctx, 2, "type_def");
        _r.add(_rthis.unit_, _rctx, 3, "unit");
        _r.add(_rthis.description_, _rctx, 4, "description");
    }
};

using DescriptionVectorT = std::vector<Description>;

template <class Enum, class ValType, class UContainer, class SContainer>
void append(const Enum _id, const DescriptionVectorT& _des_vec, const ValType& _val, UContainer& _ucontainer, SContainer& _scontainer)
{
    const auto index = solid::to_underlying(_id);
    solid_check(index < _des_vec.size());
    if (_des_vec[index].type_def_[0] == 'u') {
        if constexpr (std::is_same_v<ValType, uint64_t>) {
            _ucontainer.emplace_back(_val);
        } else {
            solid_throw("Invalid ValueType");
        }
    } else if (_des_vec[index].type_def_[0] == 's') {
        if constexpr (std::is_same_v<ValType, std::string>) {
            _scontainer.emplace_back(_val);
        } else {
            solid_throw("Invalid ValueType");
        }
    } else {
        solid_throw("Invalid type_def");
    }
}

std::ostream& print(std::ostream& _ros, uint64_t _value, const std::string& _type_def, const std::string& _unit);
std::ostream& print(std::ostream& _ros, const std::string& _value, const std::string& _type_def, const std::string& _unit);

template <class UIt, class SIt>
std::ostream& print(std::ostream& _ros, const DescriptionVectorT& _des_vec, const char _formatting, UIt _uit, const UIt& _uend, SIt _sit, const SIt& _send)
{
    solid_check(_formatting == 's', "Unknown formatting " << _formatting);
    for (const auto& des : _des_vec) {
        if (!des.type_def_.empty()) {
            _ros << '[' << des.name_ << ": ";
            if (des.type_def_[0] == 'u') {
                solid_check(_uit != _uend);
                print(_ros, *_uit, des.type_def_, des.unit_);
                ++_uit;
            } else if (des.type_def_[0] == 's') {
                solid_check(_sit != _send);
                print(_ros, *_sit, des.type_def_, des.unit_);
                ++_sit;
            } else {
                solid_throw("Unknown Type: " << des.type_def_);
            }
            _ros << ']';
        }
    }
    return _ros;
}

} // namespace statistic
} // namespace utility
} // namespace myapps
