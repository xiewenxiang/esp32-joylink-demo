#
# Component Makefile
#

# LIBS := esp_joylink joylink_ble


COMPONENT_ADD_INCLUDEDIRS := include jdinnet extern

COMPONENT_ADD_LDFLAGS += -L $(COMPONENT_PATH)/lib \
                         $(addprefix -l,$(LIBS)) \
                         $(LINKER_SCRIPTS)
			 
			 
# SRCDIRS := app ble extern jdinnet

COMPONENT_SRCDIRS := app ble extern jdinnet softap


