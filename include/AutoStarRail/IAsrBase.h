#ifndef ASR_BASE_H
#define ASR_BASE_H

#include <AutoStarRail/AsrConfig.h>
#include <AutoStarRail/AsrGuidHolder.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include <type_traits>

using AsrResult = int32_t;

// clang-format off
#ifdef SWIG
#define SWIG_IGNORE(x) %ignore x;
#define SWIG_ENABLE_DIRECTOR(x) %feature("director") x;
#define SWIG_POINTER_CLASS(x, y) %pointer_class(x, y);
#define SWIG_NEW_OBJECT(x) %newobject x;
#define SWIG_DEL_OBJECT(x) %delobject x::Release;
#define SWIG_UNREF_OBJECT(x) %feature("unref") x "if($this) { $this->Release(); }"
#define SWIG_ENABLE_SHARED_PTR(x) %shared_ptr(x);
#define SWIG_NO_DEFAULT_CTOR(x) %nodefaultctor x;
#define ASR_DEFINE_GUID(name, type, l, w1, w2, b1, b2, b3, b4, b5, b6, b7, b8) \
    static const AsrGuid name =                                                \
        {l, w1, w2, {b1, b2, b3, b4, b5, b6, b7, b8}};
#define ASR_DEFINE_CLASS_GUID(name, type, l, w1, w2, b1, b2, b3, b4, b5, b6, b7, b8) \
    static const AsrGuid name =                                                      \
        {l, w1, w2, {b1, b2, b3, b4, b5, b6, b7, b8}};
#define SWIG_PRIVATE private:
#define SWIG_PUBLIC public:
#else
#define SWIG_IGNORE(x)
#define SWIG_ENABLE_DIRECTOR(x)
#define SWIG_POINTER_CLASS(x, y)
#define SWIG_NEW_OBJECT(x)
#define SWIG_DEL_OBJECT(x)
#define SWIG_UNREF_OBJECT(x)
#define SWIG_ENABLE_SHARED_PTR(x)
#define SWIG_NO_DEFAULT_CTOR(x)
#define ASR_DEFINE_GUID(name, type, l, w1, w2, b1, b2, b3, b4, b5, b6, b7, b8) \
    static const AsrGuid name =                                                \
        {l, w1, w2, {b1, b2, b3, b4, b5, b6, b7, b8}};                         \
    ASR_DEFINE_GUID_HOLDER(type, l, w1, w2, b1, b2, b3, b4, b5, b6, b7, b8);
#define ASR_DEFINE_CLASS_GUID(type, l, w1, w2, b1, b2, b3, b4, b5, b6, b7, b8) \
    ASR_DEFINE_CLASS_GUID_HOLDER(type, l, w1, w2, b1, b2, b3, b4, b5, b6, b7, b8);
#define ASR_DEFINE_CLASS_IN_NAMESPACE(ns, type, l, w1, w2, b1, b2, b3, b4, b5, b6, b7, b8) \
    namespace ns { class type; }\
    ASR_DEFINE_CLASS_GUID_HOLDER_IN_NAMESPACE(ns ,type, l, w1, w2, b1, b2, b3, b4, b5, b6, b7, b8);
#define SWIG_PRIVATE
#define SWIG_PUBLIC
#endif
// clang-format on

template <class T>
void _asr_internal_DelayAddRef(T* pointer)
{
    pointer->AddRef();
}

template <class T>
void _asr_internal_DelayRelease(T* pointer) noexcept
{
    pointer->Release();
}

#define ASR_DEFINE_RET_TYPE(type_name, type)                                   \
    struct type_name                                                           \
    {                                                                          \
        SWIG_PRIVATE                                                           \
        AsrResult error_code{ASR_E_UNDEFINED_RETURN_VALUE};                    \
        type      value{};                                                     \
                                                                               \
    public:                                                                    \
        AsrResult GetErrorCode() noexcept { return error_code; }               \
        void      SetErrorCode(AsrResult in_error_code) noexcept               \
        {                                                                      \
            this->error_code = in_error_code;                                  \
        }                                                                      \
        type GetValue() { return value; }                                      \
        void SetValue(const type& input_value) { value = input_value; }        \
    }

/**
 * @brief 注意：GetValue将取得指针所有权
 *
 */
#define ASR_DEFINE_RET_POINTER(type_name, pointer_type)                        \
    struct type_name                                                           \
    {                                                                          \
        SWIG_PRIVATE                                                           \
        AsrResult                 error_code{ASR_E_UNDEFINED_RETURN_VALUE};    \
        ASR::AsrPtr<pointer_type> value{};                                     \
                                                                               \
    public:                                                                    \
        AsrResult GetErrorCode() noexcept { return error_code; }               \
        void      SetErrorCode(AsrResult in_error_code) noexcept               \
        {                                                                      \
            this->error_code = in_error_code;                                  \
        }                                                                      \
        pointer_type* GetValue() noexcept                                      \
        {                                                                      \
            auto* const result = value.Get();                                  \
            _asr_internal_DelayAddRef(result);                                 \
            return result;                                                     \
        }                                                                      \
        void SetValue(pointer_type* input_value) { value = input_value; }      \
    }

#define ASR_SWIG_DIRECTOR_ATTRIBUTE(x)                                         \
    SWIG_ENABLE_DIRECTOR(x)                                                    \
    SWIG_UNREF_OBJECT(x)

#define ASR_SWIG_EXPORT_ATTRIBUTE(x) SWIG_UNREF_OBJECT(x)

// 使用宏定义后SWIG会认为这些值必然是常量
#define ASR_S_OK 0
#define ASR_S_FALSE 1
#define ASR_E_RESERVED -1073741830
#define ASR_E_NO_INTERFACE -1073741831
#define ASR_E_UNDEFINED_RETURN_VALUE -1073741832
#define ASR_E_INVALID_STRING -1073741833
#define ASR_E_INVALID_STRING_SIZE -1073741834
#define ASR_E_NO_IMPLEMENTATION -1073741835
#define ASR_E_UNSUPPORTED_SYSTEM -1073741836
#define ASR_E_INVALID_JSON -1073741837
#define ASR_E_TYPE_ERROR -1073741838
#define ASR_E_INVALID_FILE -1073741839
#define ASR_E_INVALID_URL -1073741840
/**
 * @brief 返回此值可以表示枚举结束
 *
 */
#define ASR_E_OUT_OF_RANGE -1073741841
#define ASR_E_DUPLICATE_ELEMENT -1073741842
#define ASR_E_FILE_NOT_FOUND -1073741843
#define ASR_E_MAYBE_OVERFLOW -1073741844
#define ASR_E_OUT_OF_MEMORY -1073741845
#define ASR_E_INVALID_PATH -1073741846
#define ASR_E_INVALID_POINTER -1073741847
#define ASR_E_SWIG_INTERNAL_ERROR -1073741848
#define ASR_E_PYTHON_ERROR -1073741849
#define ASR_E_JAVA_ERROR -1073741850
#define ASR_E_CSHARP_ERROR -1073741851
#define ASR_E_INTERNAL_FATAL_ERROR -1073741852
#define ASR_E_INVALID_ENUM -1073741853
#define ASR_E_INVALID_SIZE -1073741854
#define ASR_E_OPENCV_ERROR -1073741855
#define ASR_E_ONNX_RUNTIME_ERROR -1073741856
#define ASR_E_TIMEOUT -1073741857
#define ASR_E_PERMISSION_DENIED -1073741859
#define ASR_E_SYMBOL_NOT_FOUND -1073741860
#define ASR_E_DANGLING_REFERENCE -1073741861
#define ASR_E_OBJECT_NOT_INIT -1073741862
#define ASR_E_UNEXPECTED_THREAD_DETECTED -1073741863

#ifdef ASR_WINDOWS
// MSVC
#ifdef _MSC_VER
#define ASR_STD_CALL __stdcall
#else
// GCC AND CLANG
#define ASR_STD_CALL __attribute__((__stdcall__))
#endif // _MSC_VER
#endif // ASR_WINDOWS

#ifndef ASR_STD_CALL
#define ASR_STD_CALL
#endif // ASR_STD_CALL

#define ASR_INTERFACE struct

#define ASR_METHOD virtual AsrResult ASR_STD_CALL
#define ASR_METHOD_(x) virtual x ASR_STD_CALL
#define ASR_IMPL AsrResult

/**
 * @brief NOTE: Be careful about the lifetime of this structure.\n
 *       If you want to keep it, you MUST make a copy of it.
 */
typedef struct _asr_GUID
{
#ifdef SWIG
private:
#endif // SWIG
    uint32_t data1;
    uint16_t data2;
    uint16_t data3;
    uint8_t  data4[8];
} AsrGuid;

ASR_DEFINE_RET_TYPE(AsrRetGuid, AsrGuid);

typedef char AsrBool;

#define ASR_TRUE 1
#define ASR_FALSE 0

/**
 * @brief input format should be "xxxxxxxx-xxxx-xxxx-xxxx-xxxxxxxxxxxx"
 *
 * @param p_guid_string
 * @return ASR_S_OK if success.
 */
ASR_API AsrRetGuid AsrMakeAsrGuid(const char* p_guid_string);

/**
 * @brief input format should be "xxxxxxxx-xxxx-xxxx-xxxx-xxxxxxxxxxxx"
 *
 * @param p_guid_string
 * @return ASR_S_OK if success.
 */
SWIG_IGNORE(AsrMakeAsrGuid)
ASR_C_API AsrResult
AsrMakeAsrGuid(const char* p_guid_string, AsrGuid* p_out_guid);

#ifndef SWIG

#ifdef __cplusplus
inline bool operator==(const AsrGuid& lhs, const AsrGuid& rhs) noexcept
{
    const auto result = ::memcmp(&lhs, &rhs, sizeof(lhs));
    return result == 0;
}

ASR_NS_BEGIN

inline bool IsOk(const AsrResult result) { return result >= 0; }
inline bool IsFailed(const AsrResult result) { return result < 0; }
inline AsrResult GetErrorCodeFrom(const AsrResult result) {return result;}

#ifdef __cplusplus

template <class T>
concept is_asr_ret_type = requires { T::error_code; };

template <is_asr_ret_type T>
bool IsOk(const T& t)
{
    return t.error_code >= 0;
}

template <is_asr_ret_type T>
bool IsFailed(const T& t)
{
    return t.error_code < 0;
}

template <is_asr_ret_type T>
AsrResult GetErrorCodeFrom(const T& t)
{
    return t.error_code;
}

#endif // __cplusplus

ASR_NS_END

#endif // __cplusplus

ASR_DEFINE_GUID(
    ASR_IID_BASE,
    IAsrBase,
    0x00000000,
    0x0000,
    0x0000,
    0xc0,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x46)
ASR_INTERFACE IAsrBase
{
    virtual int64_t AddRef() = 0;
    virtual int64_t Release() = 0;
    ASR_METHOD      QueryInterface(const AsrGuid& iid, void** pp_object) = 0;
};

ASR_C_API AsrResult
CreateIAsrReadOnlyStringVector(AsrGuid** p_in_guid_array, const size_t size);

#endif // SWIG

ASR_API inline bool IsAsrGuidEqual(
    const AsrGuid& lhs,
    const AsrGuid& rhs) noexcept
{
    return lhs == rhs;
}

ASR_INTERFACE IAsrSwigBase;

/**
 * @brief 这个类持有的指针生命周期非常特殊：
 *      1.
 * 获取指针时它不获取指针所有权，但它确实持有指针所有权，即它专用于QueryInterface产生的指针
 *      2. 因此释放时内部会Release指针
 */
struct AsrRetSwigBase
{
    SWIG_PRIVATE
    AsrResult error_code{ASR_E_UNDEFINED_RETURN_VALUE};
    // IAsrSwigBase
    void* value{};

    [[nodiscard]]
    void* GetVoidNoAddRef() const noexcept;

    void SetValueAddRef(void* value_need_add_ref);

    IAsrSwigBase* InternalAddRef()
    {
        auto* const p = static_cast<IAsrSwigBase*>(value);
        if (p)
        {
            _asr_internal_DelayAddRef(p);
        }
        return p;
    }

    void InternalRelease() noexcept
    {
        auto* const p = static_cast<IAsrSwigBase*>(value);
        if (p)
        {
            _asr_internal_DelayRelease(p);
        }
    }

    AsrRetSwigBase(AsrResult error_code, void* value);
    AsrRetSwigBase(AsrResult error_code);

public:
    AsrRetSwigBase() = default;
    ~AsrRetSwigBase() { InternalRelease(); }

#ifndef SWIG
    ASR_API                 AsrRetSwigBase(const AsrRetSwigBase& rhs);
    ASR_API AsrRetSwigBase& operator=(const AsrRetSwigBase& rhs);

    ASR_API                 AsrRetSwigBase(AsrRetSwigBase&& other) noexcept;
    ASR_API AsrRetSwigBase& operator=(AsrRetSwigBase&& other) noexcept;
#endif // SWIG

    AsrResult GetErrorCode() noexcept { return error_code; }

    void SetErrorCode(AsrResult in_error_code) noexcept
    {
        this->error_code = in_error_code;
    }

    IAsrSwigBase* GetValue() { return InternalAddRef(); }

    void SetValue(IAsrSwigBase* input_value) { value = input_value; }
};

// {FAF64DEB-0C0A-48CC-BA10-FCDE420350A2}
ASR_DEFINE_GUID(
    ASR_IID_SWIG_BASE,
    IAsrSwigBase,
    0xfaf64deb,
    0xc0a,
    0x48cc,
    0xba,
    0x10,
    0xfc,
    0xde,
    0x42,
    0x3,
    0x50,
    0xa2)
ASR_SWIG_DIRECTOR_ATTRIBUTE(IAsrSwigBase)
ASR_INTERFACE IAsrSwigBase
{
    virtual int64_t AddRef() = 0;
    virtual int64_t Release() = 0;
    /**
     * @brief Implementation should only return ASR_S_OK or
     * ASR_E_NO_INTERFACE. NOTICE: If returned value is not equal to
     * ASR_S_OK, then the interface is considered not supported.
     *
     * @param iid
     * @return AsrResult
     */
    virtual AsrRetSwigBase QueryInterface(const AsrGuid& iid) = 0;
#ifdef SWIG
    /*
     * @brief Avoid SWIG warning.
     */
    virtual ~IAsrSwigBase() {}
#endif // SWIG
};

ASR_DEFINE_RET_TYPE(AsrRetBool, bool);

ASR_DEFINE_RET_TYPE(AsrRetInt, int64_t);

ASR_DEFINE_RET_TYPE(AsrRetUInt, uint64_t);

ASR_DEFINE_RET_TYPE(AsrRetFloat, float);

#endif // ASR_BASE_H
