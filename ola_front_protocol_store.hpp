#pragma once
#include "ola/common/ola_front_protocol_main.hpp"

namespace ola {
namespace front {
namespace store{
constexpr size_t protocol_id = 3;

//the version is only transfered from client to server.
//the client will NOT know the server version
struct Version{
    static constexpr uint32_t version = 1;
    static constexpr uint32_t acquire_app_request = 1;
    static constexpr uint32_t init_request = 1;
        
    uint32_t version_ = version;
    uint32_t acquire_app_request_ = acquire_app_request;
    uint32_t init_request_ = init_request;
    
    void clear(){
        acquire_app_request_ = std::numeric_limits<uint32_t>::max();
    }
    
    bool operator<=(const Version& _rthat)const{
        return version_ <= _rthat.version_ &&
            acquire_app_request_ <= _rthat.acquire_app_request_ &&
            init_request_ <= _rthat.init_request_;
    }
    
    SOLID_REFLECT_V1(_s, _rthis, _rctx){
        _s.add(_rthis.version_, _rctx, 0, "version");
        _s.add([&_rthis](S& _s, solid::frame::mprpc::ConnectionContext& _rctx) {
            if constexpr (!S::is_const_reflector){
                if(_rthis.version > Version::version){
                    _rthis.clear();
                    return;
                }
            }
            if(_rthis.version_ == version){
                _s.add(_rthis.acquire_app_request_, _rctx, 2, "acquire_app_request");
                _s.add(_rthis.init_request_, _rctx, 3, "init_request");
            }
        },
            _rctx);
    }
    
};

constexpr Version version;

struct InitRequest : solid::frame::mprpc::Message {
    Version store_version_;
    core::Version core_version_;
    main::Version main_version_;
    utility::Version utility_version_;

    SOLID_REFLECT_V1(_s, _rthis, _rctx)
    {
        _s.add(_rthis.auth_version_, _rctx, 1, "store_version");
        _s.add([&_rthis](S& _s, solid::frame::mprpc::ConnectionContext& _rctx) {
            
            if(_rthis.version_.init_request_ == Version::init_request){
                _s.add(_rthis.core_version_, _rctx, 3, "core_version");
                _s.add(_rthis.main_version_, _rctx, 4, "main_version");
                _s.add(_rthis.utility_version_, _rctx, 5, "utility_version");
            }
        },
            _rctx);
    }
};


struct AcquireAppRequest : solid::frame::mprpc::Message {
    std::string app_id_;
    bool        acquire_ = true;

    SOLID_REFLECT_V1(_s, _rthis, _rctx)
    {
        _s.add(_rthis.app_id_, _rctx, 1, "app_id");
        _s.add(_rthis.acquire_, _rctx, 2,  "acquire");
    }
};

template <class Reg>
inline void configure_protocol(Reg _rreg)
{
    _rreg(protocol_id, 1, "InitRequest", solid::TypeToType<InitRequest>());
    _rreg(protocol_id, 2, "AcquireAppRequest", solid::TypeToType<AcquireAppRequest>());
}

} //namespace store
} //namespace front
} //namespace ola
