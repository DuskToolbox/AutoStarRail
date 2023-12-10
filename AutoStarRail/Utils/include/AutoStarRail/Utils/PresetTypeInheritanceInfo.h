#ifndef ASR_UTILS_PRESETTYPEINHERITANCEINFO_H
#define ASR_UTILS_PRESETTYPEINHERITANCEINFO_H

#include <AutoStarRail/Utils/InternalTypeList.hpp>
#include <AutoStarRail/IAsrBase.h>

ASR_UTILS_NS_BEGIN

template <class T>
struct PresetTypeInheritanceInfo;

#define ASR_UTILS_DEFINE_PRESET_INHERITANCE_INFO(EndType, ...)                 \
    ASR_INTERFACE EndType;                                                     \
    ASR_UTILS_NS_BEGIN using EndType##InheritanceInfo =                        \
        ASR::Utils::internal_type_holder<__VA_ARGS__, EndType>;                \
    template <>                                                                \
    struct PresetTypeInheritanceInfo<EndType>                                  \
    {                                                                          \
        using TypeInfo = EndType##InheritanceInfo;                             \
    };                                                                         \
    ASR_UTILS_NS_END

// IAsrBase.h
using IAsrBaseInheritanceInfo = internal_type_holder<IAsrBase>;
template <>
struct PresetTypeInheritanceInfo<IAsrBase>
{
    using TypeInfo = IAsrBaseInheritanceInfo;
};

ASR_UTILS_NS_END

// IAsrInspectable.h
ASR_UTILS_DEFINE_PRESET_INHERITANCE_INFO(IAsrInspectable, IAsrBase);
ASR_UTILS_DEFINE_PRESET_INHERITANCE_INFO(IAsrIidVector, IAsrBase);
// AsrReadOnlyString.hpp
ASR_UTILS_DEFINE_PRESET_INHERITANCE_INFO(IAsrReadOnlyString, IAsrBase);
ASR_UTILS_DEFINE_PRESET_INHERITANCE_INFO(
    IAsrString,
    IAsrBase,
    IAsrReadOnlyString);

// PluginInterface/IAsrCapture.h
ASR_UTILS_DEFINE_PRESET_INHERITANCE_INFO(
    IAsrCapture,
    IAsrBase,
    IAsrInspectable);
ASR_UTILS_DEFINE_PRESET_INHERITANCE_INFO(
    IAsrCaptureFactory,
    IAsrBase,
    IAsrInspectable);
// PluginInterface/IAsrPlugin.h
ASR_UTILS_DEFINE_PRESET_INHERITANCE_INFO(IAsrPlugin, IAsrBase);
// PluginInterface/IAsrErrorLens.h
ASR_UTILS_DEFINE_PRESET_INHERITANCE_INFO(IAsrErrorLens, IAsrBase);
// PluginInterface/IAsrTask.h
ASR_UTILS_DEFINE_PRESET_INHERITANCE_INFO(IAsrTask, IAsrBase);

// ExportInterface/IAsrCaptureManager.h
ASR_UTILS_DEFINE_PRESET_INHERITANCE_INFO(IAsrCaptureManager, IAsrBase);
// ExportInterface/IAsrSettings.h
ASR_UTILS_DEFINE_PRESET_INHERITANCE_INFO(IAsrSettings, IAsrBase);
ASR_UTILS_DEFINE_PRESET_INHERITANCE_INFO(IAsrSwigSettings, IAsrSwigBase);

#endif // ASR_UTILS_PRESETTYPEINHERITANCEINFO_H
