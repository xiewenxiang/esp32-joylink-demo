#
# Component Makefile
#

COMPONENT_ADD_INCLUDEDIRS := include jdinnet extern

COMPONENT_ADD_LDFLAGS += -L $(COMPONENT_PATH)/lib \
                         $(addprefix -l,$(LIBS)) \
                         $(LINKER_SCRIPTS)

COMPONENT_SRCDIRS := app ble extern jdinnet softap
