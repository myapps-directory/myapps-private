#pragma once
#include "ola/common/ola_front_protocol_common.hpp"

namespace ola {
namespace front {


struct AcquireAppRequest : solid::frame::mprpc::Message {
    static constexpr uint32_t version = 1;

    uint32_t    version_ = version;
    std::string app_id_;
    bool        acquire_ = true;

    SOLID_PROTOCOL_V2(_s, _rthis, _rctx, _name)
    {
        solid::serialization::addVersion<AcquireAppRequest>(_s, _rthis.version_, "version");

        _s.add([&_rthis](S& _s, solid::frame::mprpc::ConnectionContext& _rctx, const char* /*_name*/) {
            _s.add(_rthis.app_id_, _rctx, "app_id");
            _s.add(_rthis.acquire_, _rctx, "acquire");
        },
            _rctx, _name);
    }
};

template <class R>
inline void protocol_setup_store(R _r, ProtocolT& _rproto)
{
    _r(_rproto, solid::TypeToType<AcquireAppRequest>(), {3, 1});
}


}//namespace front
}//namespace ola
