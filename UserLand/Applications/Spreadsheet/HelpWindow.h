#pragma once

// includes
#include <AK/JsonObject.h>
#include <LibGUI/Dialog.h>
#include <LibGUI/Widget.h>
#include <LibGUI/Window.h>
#include <LibWeb/OutOfProcessWebView.h>

namespace Spreadsheet {

class HelpWindow : public GUI::Window {
    C_OBJECT(HelpWindow);

public:
    static NonnullRefPtr<HelpWindow> the(GUI::Window* window)
    {
        if (s_the)
            return *s_the;

        return *(s_the = adopt_ref(*new HelpWindow(window)));
    }

    virtual ~HelpWindow() override;

    void set_docs(JsonObject&& docs);

private:
    static RefPtr<HelpWindow> s_the;
    String render(const StringView& key);
    HelpWindow(GUI::Window* parent = nullptr);

    JsonObject m_docs;
    RefPtr<Web::OutOfProcessWebView> m_webview;
    RefPtr<GUI::ListView> m_listview;
};

}
