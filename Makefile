SUBDIRS=proto mgmt ds client
BUILD_HDR=build_
SUBDIRS_BUILD := $(addprefix $(BUILD_HDR), $(SUBDIRS))

all: $(SUBDIRS_BUILD)
clean: $(SUBDIRS_BUILD)

$(SUBDIRS_BUILD):
	make -C $(subst $(BUILD_HDR),,$@) $(MAKECMDGOALS)
