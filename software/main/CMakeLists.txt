file(GLOB_RECURSE IMAGE_SOURCES images/*.c)

idf_component_register(SRCS "i80_controller_example_main.c" "lvgl_demo_ui.c" "images/ui_img_useful_electronics_png.c" ${IMAGE_SOURCES}
                       INCLUDE_DIRS ".")
