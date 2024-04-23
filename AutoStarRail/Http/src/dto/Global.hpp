#ifndef Global_DTOs_hpp
#define Global_DTOs_hpp

#include "oatpp/core/Types.hpp"
#include "oatpp/core/macro/codegen.hpp"

#include OATPP_CODEGEN_BEGIN(DTO)

/**
 *  API全局类型
 *  API global type
 */

// 定义统一响应包装类型
// Define unified response wrapper type
template <class T>
class ApiResult : public oatpp::DTO
{

    DTO_INIT(ApiResult, DTO)

    DTO_FIELD(Int32, code);
    DTO_FIELD(String, message);

    DTO_FIELD(T, data);
};

#include OATPP_CODEGEN_END(DTO)

#endif