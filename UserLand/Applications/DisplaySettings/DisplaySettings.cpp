// includes
#include "DisplaySettings.h"
#include <AK/StringBuilder.h>
#include <Applications/DisplaySettings/DisplaySettingsWindowGML.h>
#include <LibCore/ConfigFile.h>
#include <LibCore/DirIterator.h>
#include <LibGUI/Application.h>
#include <LibGUI/BoxLayout.h>
#include <LibGUI/Button.h>
#include <LibGUI/ComboBox.h>
#include <LibGUI/Desktop.h>
#include <LibGUI/FilePicker.h>
#include <LibGUI/ItemListModel.h>
#include <LibGUI/Label.h>
#include <LibGUI/MessageBox.h>
#include <LibGUI/RadioButton.h>
#include <LibGUI/WindowServerConnection.h>
#include <LibGfx/Palette.h>
#include <LibGfx/SystemTheme.h>

REGISTER_WIDGET(DisplaySettings, MonitorWidget)

DisplaySettingsWidget::DisplaySettingsWidget()
{
    create_resolution_list();
    create_wallpaper_list();

    create_frame();
    load_current_settings();
}

void DisplaySettingsWidget::create_resolution_list()
{
    
}