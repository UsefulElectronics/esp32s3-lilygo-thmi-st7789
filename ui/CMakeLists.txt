SET(SOURCES screens/ui_Screen1.c
    ui.c
    components/ui_comp_hook.c
    ui_helpers.c
    images/ui_img_gauge_empty_png.c
    images/ui_img_gauge_full_png.c)

add_library(ui ${SOURCES})
