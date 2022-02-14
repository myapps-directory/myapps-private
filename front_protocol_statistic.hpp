#pragma once
#include "myapps/common/front_protocol_main.hpp"
#include "myapps/private/utility/statistic.hpp"

namespace myapps {
namespace front {
namespace statistic {
constexpr uint8_t protocol_id = 4;

//the version is only transfered from client to server.
//the client will NOT know the server version
struct Version {
    static constexpr uint32_t version             = 1;
    static constexpr uint32_t acquire_app_request = 1;
    static constexpr uint32_t init_request        = 1;

    uint32_t version_             = version;
    uint32_t acquire_app_request_ = acquire_app_request;
    uint32_t init_request_        = init_request;

    void clear()
    {
        acquire_app_request_ = std::numeric_limits<uint32_t>::max();
    }

    bool operator<=(const Version& _rthat) const
    {
        return version_ <= _rthat.version_ && acquire_app_request_ <= _rthat.acquire_app_request_ && init_request_ <= _rthat.init_request_;
    }

    SOLID_REFLECT_V1(_s, _rthis, _rctx)
    {
        _s.add(_rthis.version_, _rctx, 0, "version");
        _s.add([&_rthis](Reflector& _s, Context& _rctx) {
            if constexpr (!Reflector::is_const_reflector) {
                if (_rthis.version > Version::version) {
                    _rthis.clear();
                    return;
                }
            }
            if (_rthis.version_ == version) {
                _s.add(_rthis.acquire_app_request_, _rctx, 2, "acquire_app_request");
                _s.add(_rthis.init_request_, _rctx, 3, "init_request");
            }
        },
            _rctx);
    }
};

constexpr Version version;

struct InitRequest : solid::frame::mprpc::Message {
    Version          store_version_;
    core::Version    core_version_;
    main::Version    main_version_;
    utility::Version utility_version_;

    SOLID_REFLECT_V1(_s, _rthis, _rctx)
    {
        _s.add(_rthis.store_version_, _rctx, 1, "store_version");
        _s.add([&_rthis](Reflector& _s, Context& _rctx) {
            if (_rthis.store_version_.init_request_ == Version::init_request) {
                _s.add(_rthis.core_version_, _rctx, 3, "core_version");
                _s.add(_rthis.main_version_, _rctx, 4, "main_version");
                _s.add(_rthis.utility_version_, _rctx, 5, "utility_version");
            }
        },
            _rctx);
    }
};

struct FetchRequest : solid::frame::mprpc::Message {
    std::string              name_;
    std::vector<std::string> name_vec_;

    SOLID_REFLECT_V1(_r, _rthis, _rctx)
    {
        _r.add(_rthis.name_, _rctx, 1, "name");
        _r.add(_rthis.name_vec_, _rctx, 2, "name_vec");
    }
};

struct FetchResponse : solid::frame::mprpc::Message {
    std::vector<std::string> type_def_vec_;
    std::vector<uint64_t>    data_uint_vec_;
    std::vector<std::string> data_string_vec_;

    FetchResponse() = default;

    FetchResponse(const FetchRequest& _req)
        : solid::frame::mprpc::Message(_req)
    {
    }

    SOLID_REFLECT_V1(_r, _rthis, _rctx)
    {
        _r.add(_rthis.type_def_vec_, _rctx, 3, "type_def_vec");
        _r.add(_rthis.data_uint_vec_, _rctx, 4, "data_uint_vec");
        _r.add(_rthis.data_string_vec_, _rctx, 5, "data_string_vec");
    }
};

struct DescribeRequest : solid::frame::mprpc::Message {
    std::string name_;

    SOLID_REFLECT_V1(_r, _rthis, _rctx)
    {
        _r.add(_rthis.name_, _rctx, 2, "name");
    }
};

struct DescribeResponse : solid::frame::mprpc::Message {
    using DescriptionT = myapps::utility::statistic::Description;

    uint32_t                  error_ = 0;
    std::vector<DescriptionT> description_vec_;

    DescribeResponse() {}

    DescribeResponse(
        const solid::frame::mprpc::Message& _rreq)
        : solid::frame::mprpc::Message(_rreq)
    {
    }

    SOLID_REFLECT_V1(_r, _rthis, _rctx)
    {
        _r.add(_rthis.error_, _rctx, 1, "version");
        _r.add(_rthis.description_vec_, _rctx, 2, "description_vec");
    }
};

template <class Reg>
inline void configure_protocol(Reg _rreg)
{
    _rreg({protocol_id, 1}, "InitRequest", solid::TypeToType<InitRequest>());
    _rreg({protocol_id, 2}, "FetchRequest", solid::TypeToType<FetchRequest>());
    _rreg({protocol_id, 3}, "FetchResponse", solid::TypeToType<FetchResponse>());
    _rreg({protocol_id, 4}, "DescribeRequest", solid::TypeToType<DescribeRequest>());
    _rreg({protocol_id, 5}, "DescribeResponse", solid::TypeToType<DescribeResponse>());
}

} //namespace statistic
} //namespace front
} //namespace myapps
