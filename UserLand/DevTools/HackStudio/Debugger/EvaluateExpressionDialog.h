#pragma once

// includes
#include <LibGUI/Dialog.h>
#include <LibWeb/InProcessWebView.h>

namespace HackStudio {

class EvaluateExpressionDialog : public GUI::Dialog {
    C_OBJECT(EvaluateExpressionDialog);

public:
    explicit EvaluateExpressionDialog(Window* parent_window);

private:
    void build(Window* parent_window);
    void handle_evaluation(const String& expression);
    void set_output(const StringView& html);

    NonnullOwnPtr<JS::Interpreter> m_interpreter;
    RefPtr<GUI::TextBox> m_text_editor;
    RefPtr<Web::InProcessWebView> m_output_view;
    RefPtr<Web::DOM::Element> m_output_container;
    RefPtr<GUI::Button> m_evaluate_button;
    RefPtr<GUI::Button> m_close_button;
};

}
