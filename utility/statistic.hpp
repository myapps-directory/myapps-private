#pragma once

#include <string>
#include <vector>
#include "solid/utility/common.hpp"
#include "solid/system/exception.hpp"
#include "solid/reflection/reflection.hpp"

namespace myapps{
namespace utility{
namespace statistic{

struct Description{
    std::string name_;
    std::string type_def_;
    std::string description_;
    
    SOLID_REFLECT_V1(_r, _rthis, _rctx)
    {
        _r.add(_rthis.name_, _rctx, 1, "name");
        _r.add(_rthis.type_def_, _rctx, 1, "type_def");
        _r.add(_rthis.description_, _rctx, 1, "description");
    }
};

using DescriptionVectorT = std::vector<Description>;

template <class Enum, class ValType, class UContainer, class SContainer>
void append(const Enum _id, const DescriptionVectorT &_des_vec, const ValType &_val, UContainer &_ucontainer, SContainer &_scontainer){
    const auto index = solid::to_underlying(_id);
    solid_check(index < _des_vec.size());
    if(_des_vec[index].type_def_.empty() || _des_vec[index].type_def_[0] == 'u'){
        if constexpr (std::is_same_v<ValType, uint64_t>){
            _ucontainer.emplace_back(_val);
        }else{
            solid_throw("Invalid ValueType");
        }
    }else if(_des_vec[index].type_def_[0] == 's'){
        if constexpr (std::is_same_v<ValType, std::string>){
            _scontainer.emplace_back(_val);
        }else{
            solid_throw("Invalid ValueType");
        }
    }else{
        solid_throw("Invalid type_def");
    }
}
}//namespace statistic
}//namespace utility
}//namespace myapps
