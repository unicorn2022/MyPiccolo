#include <assert.h>

#include "runtime/core/meta/json.h"
#include "runtime/core/meta/reflection/reflection.h"
#include "runtime/core/meta/reflection/reflection_register.h"
#include "runtime/core/meta/serializer/serializer.h"

#include "_generated/reflection/all_reflection.h"
#include "_generated/serializer/all_serializer.ipp"

namespace MyPiccolo {
    namespace Reflection {
        void TypeMetaRegister::Unregister() {
            // 调用接口的取消注册函数
            TypeMetaRegisterinterface::unregisterAll();
        }
    } // namespace Reflection
} // namespace MyPiccolo
