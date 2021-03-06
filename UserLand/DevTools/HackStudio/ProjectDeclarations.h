#pragma once

// includes
#include <AK/Function.h>
#include <AK/HashMap.h>
#include <AK/Noncopyable.h>
#include <AK/String.h>
#include <LibGUI/AutocompleteProvider.h>
#include <LibGUI/Icon.h>

namespace HackStudio {

class ProjectDeclarations {
    AK_MAKE_NONCOPYABLE(ProjectDeclarations);

public:
    static ProjectDeclarations& the();
    template<typename Func>
    void for_each_declared_symbol(Func);

    void set_declared_symbols(const String& filename, const Vector<GUI::AutocompleteProvider::Declaration>&);

    static Optional<GUI::Icon> get_icon_for(GUI::AutocompleteProvider::DeclarationType);

    Function<void()> on_update = nullptr;

private:
    ProjectDeclarations() = default;
    HashMap<String, Vector<GUI::AutocompleteProvider::Declaration>> m_document_to_declarations;
};

template<typename Func>
void ProjectDeclarations::for_each_declared_symbol(Func f)
{
    for (auto& item : m_document_to_declarations) {
        for (auto& decl : item.value) {
            f(decl);
        }
    }
}

}
