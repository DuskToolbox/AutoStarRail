#ifndef DAS_CORE_FOREIGNINTERFACEHOST_IFOREIGNLANGUAGERUNTIME_H
#define DAS_CORE_FOREIGNINTERFACEHOST_IFOREIGNLANGUAGERUNTIME_H

#include <das/DasPtr.hpp>
#include <das/PluginInterface/IDasPlugin.h>
#include <das/Core/ForeignInterfaceHost/ForeignInterfaceHostEnum.h>
#include <variant>
#include <das/IDasBase.h>
#include <das/Utils/Expected.h>

DAS_CORE_FOREIGNINTERFACEHOST_NS_BEGIN

using CommonPluginPtr =
    std::variant<DasPtr<IDasPlugin>, DasPtr<IDasSwigPlugin>>;

/**
 * @brief 创建语言runtime必须的内容，一次性给够够全部语言runtime创建的信息
 *  未来可能被放入共享内存， 因此要注意内存分配问题
 */
struct ForeignLanguageRuntimeFactoryDesc
{
    ForeignInterfaceLanguage language;
};

DAS_INTERFACE IForeignLanguageRuntime : public IDasBase
{
    virtual auto LoadPlugin(const std::filesystem::path& path)
        -> DAS::Utils::Expected<CommonPluginPtr> = 0;
};

/**
 *
 * @param desc_base
 * @return DAS_E_NO_IMPLEMENTATION 意味着对应语言的接口未实现
 */
auto CreateForeignLanguageRuntime(
    const ForeignLanguageRuntimeFactoryDesc& desc_base)
    -> DAS::Utils::Expected<DasPtr<IForeignLanguageRuntime>>;

DAS_CORE_FOREIGNINTERFACEHOST_NS_END

#endif // DAS_CORE_FOREIGNINTERFACEHOST_IFOREIGNLANGUAGERUNTIME_H
