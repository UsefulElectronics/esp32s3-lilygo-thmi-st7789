file(GLOB_RECURSE IMAGE_SOURCES images/*.c)

file(GLOB_RECURSE DISPLAY_SOURCES display/*.c)

file(GLOB_RECURSE GPIO_SOURCES gpio/*.c)

file(GLOB_RECURSE UWB_SOURCES uwb/*.c)

file(GLOB_RECURSE SGP_SOURCES sgp40/*.c)

file(GLOB_RECURSE HDC1080_SOURCES hdc1080/*.c)

file(GLOB_RECURSE WIRELESS wireless/*.c)


idf_component_register(SRCS "main.c" 
						"lvgl_demo_ui.c" 
						"images/ui_img_useful_electronics_png.c" 
						"images/ui_img_air_quality_gauge_png.c" 
						"images/ui_img_gauge_pointer_png.c" 
						"images/ui_img_mqtt_passive_png.c" 
						"images/ui_img_mqtt_active_png.c" 
						"images/ui_img_wifi_passive_png.c" 
						"images/ui_img_wifi_active_png.c" 
						${GPIO_SOURCES} 
						${IMAGE_SOURCES} 
						${DISPLAY_SOURCES}
						${UWB_SOURCES}
						${SGP_SOURCES}
						${HDC1080_SOURCES}
						${WIRELESS}
                       INCLUDE_DIRS ".")
