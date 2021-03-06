#pragma once

#include <LibGUI/Dialog.h>

namespace PixelPaint {

class CreateNewLayerDialog final : public GUI::Dialog {
    C_OBJECT(CreateNewLayerDialog);

public:
    const Gfx::IntSize& layer_size() const { return m_layer_size; }
    const String& layer_name() const { return m_layer_name; }

private:
    CreateNewLayerDialog(const Gfx::IntSize& suggested_size, GUI::Window* parent_window);

    Gfx::IntSize m_layer_size;
    String m_layer_name;

    RefPtr<GUI::TextBox> m_name_textbox;
};

}
