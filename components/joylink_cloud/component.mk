#
# Component Makefile
#

ifdef CONFIG_JOYLINK_ENABLE

JOYLINK_SDK ?= joylink_dev_sdk_2.0_2018_04_24
JOYLINK_SMNT ?= joylink_smnt_v3.0.11
JOYLINK_SOFTAP ?= joylink_softap_v3.0.4

COMPONENT_ADD_INCLUDEDIRS :=
INCLUDEDIRS := agent auth joylink json list
SRCDIRS := agent auth joylink json list
COMPONENT_ADD_INCLUDEDIRS += $(addprefix adaptation/$(JOYLINK_SDK)/,$(INCLUDEDIRS)) \
				adaptation/$(JOYLINK_SDK)
COMPONENT_SRCDIRS += $(addprefix adaptation/$(JOYLINK_SDK)/,$(SRCDIRS))

COMPONENT_ADD_INCLUDEDIRS += adaptation/$(JOYLINK_SMNT)
COMPONENT_SRCDIRS += adaptation/$(JOYLINK_SMNT)


LIBS += joylink_ble

COMPONENT_ADD_LDFLAGS += -L $(COMPONENT_PATH)/lib \
                           $(addprefix -l,$(LIBS))

COMPONENT_ADD_INCLUDEDIRS += adaptation/joylink_ble/include

JOYLINK_SDK_C_FILES =   agent/joylink_adapter_net.c		\
			agent/joylink_agent.c			\
			agent/joylink_agent_devs.c		\
			agent/joylink_agent_gw_2_cloud.c	\
			agent/joylink_agent_json.c		\
								\
								\
			auth/joylink3_auth_uECC.c		\
			auth/joylink_aes.c			\
			auth/joylink_auth_crc.c			\
			auth/joylink_auth_md5.c			\
			auth/joylink_auth_uECC.c		\
			auth/joylink_crypt.c			\
								\
								\
			joylink/joylink_dev_lan.c		\
			joylink/joylink_dev_sdk.c		\
			joylink/joylink_dev_server.c		\
			joylink/joylink_dev_timer.c		\
			joylink/joylink_join_packet.c		\
			joylink/joylink_packets.c		\
			joylink/joylink_security.c		\
			joylink/joylink_sub_dev.c		\
			joylink/joylink_utils.c			\
								\
								\
			json/joylink_json.c			\
			json/joylink_json_sub_dev.c		\
								\
								\
			list/joylink_list.c


JOYLINK_SMNT_C_FILES = joylink_smnt.c

JOYLINK_SOFTAP_C_FILES =    softap/joylink_softap.c \
                            softap/joylink_util.c \
			    auth/joylink_auth_uECC.c

COMPONENT_ADD_INCLUDEDIRS += adaptation/$(JOYLINK_SOFTAP)/softap adaptation/$(JOYLINK_SOFTAP)/auth
COMPONENT_SRCDIRS += adaptation/$(JOYLINK_SOFTAP)/softap adaptation/$(JOYLINK_SOFTAP)/auth

COMPONENT_OBJS += $(addprefix adaptation/$(JOYLINK_SDK)/,$(JOYLINK_SDK_C_FILES:%.c=%.o))
COMPONENT_OBJS += $(addprefix adaptation/$(JOYLINK_SMNT)/,$(JOYLINK_SMNT_C_FILES:%.c=%.o))
COMPONENT_OBJS += $(addprefix adaptation/$(JOYLINK_SOFTAP)/,$(JOYLINK_SOFTAP_C_FILES:%.c=%.o))

CFLAGS += -D__LINUX_UB2__ -D__LINUX__  -DJLdevice_aes_decrypt=device_aes_decrypt -D_GET_HOST_BY_NAME_
CFLAGS += -Wno-error=unused-label -Wno-error=maybe-uninitialized -Wno-error=implicit-function-declaration -Wno-error=pointer-sign -Wno-error=char-subscripts -Wno-error=sizeof-pointer-memaccess -Wno-error=format
endif
