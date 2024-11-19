SET(SOURCES ui_comp_pumpcontainer.c
    ui_comp.c
    ui_Main.c
    ui_Settings.c
    ui_PumpConfig.c
    ui_WiFiSettings.c
    ui.c
    ui_comp_hook.c
    ui_helpers.c
    ui_img_no_wifi_png.c
    ui_img_temperature_png.c
    ui_img_humidity_png.c
    ui_img_plant_bad_png.c
    ui_img_plant_good_png.c
    ui_img_sun_png.c
    ui_img_moon_png.c
    ui_img_day_and_night_png.c
    ui_img_sprinkler_png.c
    ui_img_sprinkler_off_png.c
    ui_img_wifi_png.c)

add_library(ui ${SOURCES})
