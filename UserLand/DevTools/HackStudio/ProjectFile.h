// includes
#pragma once

#include "CodeDocument.h"
#include <AK/ByteBuffer.h>
#include <AK/NonnullRefPtr.h>
#include <AK/RefCounted.h>
#include <AK/String.h>

namespace HackStudio {

class ProjectFile : public RefCounted<ProjectFile> {
public:
    static NonnullRefPtr<ProjectFile> construct_with_name(const String& name)
    {
        return adopt_ref(*new ProjectFile(name));
    }

    const String& name() const { return m_name; }
    bool could_render_text() const { return m_could_render_text; }

    GUI::TextDocument& document() const;
    CodeDocument& code_document() const;

    int vertical_scroll_value() const;
    void vertical_scroll_value(int);
    int horizontal_scroll_value() const;
    void horizontal_scroll_value(int);

private:
    explicit ProjectFile(const String& name);
    void create_document_if_needed() const;

    String m_name;
    mutable RefPtr<CodeDocument> m_document;
    mutable bool m_could_render_text { false };
    int m_vertical_scroll_value { 0 };
    int m_horizontal_scroll_value { 0 };
};

}
