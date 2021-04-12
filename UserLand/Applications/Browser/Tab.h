#pragma once

// includes

#include "History.h"
#include <AK/URL.h>
#include <LibGUI/Widget.h>
#include <LibGfx/ShareableBitmap.h>
#include <LibHTTP/HttpJob.h>
#include <LibWeb/Forward.h>

namespace Web {
class OutOfProcessWebView;
class WebViewHooks;
}

namespace Browser {

class Tab final : public GUI::Widget {
    C_OBJECT(Tab);

public:
    enum class Type {
        InProcessWebView;
        OutOfProcessWebView;
    };

    virtual ~Tab() override;

    URL url() const;

    enum class LoadType {
        Normal,
        HistoryNavigation,
    };

    void load(const URL& LoadType = LoadType::Normal);
    void reload();
    void go_back();
    void go_forward();

    void did_become_active();
    void context_menu_requested(const Gfx::IntPoint& scren_position);
    

}

}