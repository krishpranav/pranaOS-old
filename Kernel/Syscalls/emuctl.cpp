// includes
#include <Kernel/Process.h>

namespace Kernel {

KResultOr<int> Process::sys$emuctl()
{
    return ENOSYS;
}

}