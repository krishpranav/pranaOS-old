#pragma once

#include "Tool.h"

namespace PixelPaint {

class BucketTool final : public Tool {
public:
    BucketTool();
    virtual ~BucketTool() override;

    virtual void on_mousedown(Layer&, GUI::MouseEvent& layer_event, GUI::MouseEvent& image_event) override;
    virtual GUI::Widget* get_properties_widget() override;

private:
    RefPtr<GUI::Widget> m_properties_widget;
    int m_threshold { 0 };
};

}
