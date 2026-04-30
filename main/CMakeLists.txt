# idf_component_register(SRCS "hello_world_main.c"
#                     PRIV_REQUIRES esp_driver_gpio
#                     INCLUDE_DIRS ".")



# THIS IS THE FILE YOU WANT TO MODIFY
idf_component_register(
    SRCS "hello_world_main.c" # DESIGNATED FILE GOES HERE
    INCLUDE_DIRS "."
    PRIV_REQUIRES esp_driver_ledc esp_driver_gpio
)