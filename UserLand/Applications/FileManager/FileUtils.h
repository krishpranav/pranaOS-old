#pragma once

#include <AK/String.h>
#include <LibCore/Forward.h>
#include <LibGUI/Forward.h>
#include <sys/stat.h>

namespace FileUtils {

enum class FileOperation {
    Copy = 0,
    Cut
};

void delete_path(const String&, GUI::Window*);
void delete_paths(const Vector<String>&, bool should_confirm, GUI::Window*);
}
