#pragma once

#if defined(KERNEL)
#    include <Kernel/Assertions.h>
#else
#    include <assert.h>
#    ifndef __serenity__
#        define VERIFY assert
#        define VERIFY_NOT_REACHED() assert(false)
#        define TODO VERIFY_NOT_REACHED
#    endif
#endif