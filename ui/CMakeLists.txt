SET(SOURCES screens/ui_Screen1.c
    screens/ui_Screen2.c
    ui.c
    components/ui_comp_hook.c
    ui_helpers.c
    images/ui_img_gauge_empty_png.c
    images/ui_img_gauge_full_png.c
    images/ui_img_1062568172.c
    images/ui_img_1282837856.c
    images/ui_img_525640412.c
    images/ui_img_air_quality_gauge_png.c
    images/ui_img_gauge_pointer_png.c)

add_library(ui ${SOURCES})
