#include "ErrorLensImpl.h"
#include "PluginImpl.h"

#include <AutoStarRail/ExportInterface/AsrLogger.h>
#include <AutoStarRail/Utils/QueryInterface.hpp>
#include <AutoStarRail/Utils/StringUtils.h>
#include <AutoStarRail/Utils/fmt.h>

ASR_NS_BEGIN

ASR_NS_ANONYMOUS_DETAILS_BEGIN

template <class T, class Out>
AsrResult ReturnPointerInMap(T& it, Out** pp_out_object)
{
    auto p_result = it->second;
    p_result->AddRef();
    if constexpr (std::is_pointer_v<decltype(p_result)>)
    {
        *pp_out_object = p_result;
    }
    else
    {
        *pp_out_object = p_result.Get();
    }
    return ASR_S_OK;
}

ASR_NS_ANONYMOUS_DETAILS_END

AdbCaptureErrorLens::AdbCaptureErrorLens() { ASR::AdbCaptureAddRef(); }

AdbCaptureErrorLens::~AdbCaptureErrorLens() { ASR::AdbCaptureRelease(); }

int64_t AdbCaptureErrorLens::AddRef() { return ref_counter_.AddRef(); }

int64_t AdbCaptureErrorLens::Release() { return ref_counter_.Release(this); }

AsrResult AdbCaptureErrorLens::QueryInterface(
    const AsrGuid& iid,
    void**         pp_out_object)
{
    return ASR::Utils::QueryInterface<IAsrErrorLens>(this, iid, pp_out_object);
}

AsrResult AdbCaptureErrorLens::GetSupportedIids(
    IAsrReadOnlyGuidVector** pp_out_iids)
{
    AsrPtr<IAsrGuidVector> p_iids{};
    if (const auto error_code =
            ::CreateIAsrGuidVector(iids_.data(), iids_.size(), p_iids.Put());
        IsFailed(error_code))
    {
        const auto error_message = ASR_FMT_NS::format(
            "Create IAsrGuidVector failed. Error code = {}.",
            error_code);
        ASR_LOG_ERROR(error_message.c_str());
        return error_code;
    }

    return p_iids->ToConst(pp_out_iids);
}

AsrResult AdbCaptureErrorLens::GetErrorMessage(
    IAsrReadOnlyString*  locale_name,
    AsrResult            error_code,
    IAsrReadOnlyString** out_string)
{
    AsrPtr locale_name_ptr{locale_name};
    if (const auto locale_it = map_.find(locale_name_ptr);
        locale_it != map_.end())
    {
        const auto& error_code_map = locale_it->second;
        if (const auto it = error_code_map.find(error_code);
            it != error_code_map.end())
        {
            return Details::ReturnPointerInMap(it, out_string);
        }
    }

    const auto& error_code_map = map_.at(p_default_locale_name);
    if (const auto it = error_code_map.find(error_code);
        it != error_code_map.end())
    {
        return Details::ReturnPointerInMap(it, out_string);
    }

    const auto error_message =
        ASR::fmt::format("No explanation for error code {}", error_code);
    AsrPtr<IAsrReadOnlyString> error_message_ptr{};
    ::CreateIAsrReadOnlyStringFromUtf8(
        error_message.c_str(),
        error_message_ptr.Put());
    *out_string = error_message_ptr.Get();
    return ASR_S_OK;
}

AsrResult AdbCaptureErrorLens::RegisterErrorCode(
    const AsrResult            error_code,
    AsrPtr<IAsrReadOnlyString> locale_name,
    AsrPtr<IAsrReadOnlyString> p_explanation)
{
    map_[locale_name][error_code] = p_explanation;
    return ASR_S_OK;
}

AsrResult AdbCaptureErrorLens::AddSupportedIid(const AsrGuid& iid)
{
    try
    {
        iids_.push_back(iid);
    }
    catch (std::bad_alloc&)
    {
        return ASR_E_OUT_OF_MEMORY;
    }
    return ASR_S_OK;
}

ASR_DEFINE_VARIABLE(AdbCaptureErrorLens::p_default_locale_name) = []()
{
    constexpr auto& default_locale_name =
        ASR_UTILS_STRINGUTILS_DEFINE_U8STR("en");
    AsrPtr<IAsrString> result{};
    ::CreateIAsrStringFromUtf8(default_locale_name, result.Put());
    return result;
}();

ASR_DEFINE_VARIABLE(
    AdbCaptureErrorLens::error_code_not_found_explanation_generator) =
    +[](AsrResult error_code, IAsrReadOnlyString** pp_out_explanation)
{
    constexpr auto& template_string = ASR_UTILS_STRINGUTILS_DEFINE_U8STR(
        "No explanation found for error code {} .");

    auto result = ASR::fmt::format(template_string, error_code);
    ::CreateIAsrReadOnlyStringFromUtf8(result.c_str(), pp_out_explanation);
    return result;
};

ASR_NS_END