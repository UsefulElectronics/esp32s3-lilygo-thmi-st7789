file(GLOB_RECURSE IMAGE_SOURCES images/*.c)

file(GLOB_RECURSE DISPLAY_SOURCES display/*.c)

file(GLOB_RECURSE GPIO_SOURCES gpio/*.c)

file(GLOB_RECURSE UWB_SOURCES uwb/*.c)

idf_component_register(SRCS "main.c" 
						"lvgl_demo_ui.c" 
						"images/ui_img_useful_electronics_png.c" 
						${GPIO_SOURCES} 
						${IMAGE_SOURCES} 
						${DISPLAY_SOURCES}
						${UWB_SOURCES}
                       INCLUDE_DIRS ".")
