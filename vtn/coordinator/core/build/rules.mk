#
# Copyright (c) 2010-2014 NEC Corporation
# All rights reserved.
# 
# This program and the accompanying materials are made available under the
# terms of the Eclipse Public License v1.0 which accompanies this
# distribution, and is available at http://www.eclipse.org/legal/epl-v10.html
#

##
## Common rules for build.
## This makefile is designed to be included after config.mk is included.
##

ifndef	BUILD_RULES_MK_INCLUDED

BUILD_RULES_MK_INCLUDED	:= 1

ifndef	UNC_BUILD
include $(BLDDIR)/pflow-rules.mk
endif	# !UNC_BUILD

# Compile ALIAS_C_SOURCES with specifying CC_NO_ALIAS option.
ifneq	($(strip $(ALIAS_C_SOURCES)),)

ALIAS_C_OBJS	= $(ALIAS_C_SOURCES:%.c=$(OBJDIR)/%.o)

$(ALIAS_C_OBJS):	CFLAGS += $(CC_NO_ALIAS)

endif	# !empty(ALIAS_C_SOURCES)

# Compile ALIAS_CXX_SOURCES with specifying CXX_NO_ALIAS option.
ifneq	($(strip $(ALIAS_CXX_SOURCES)),)

ALIAS_CXX_OBJS	= $(ALIAS_CXX_SOURCES:%.cc=$(OBJDIR)/%.o)

$(ALIAS_CXX_OBJS):	CXXFLAGS += $(CXX_NO_ALIAS)

endif	# !empty(ALIAS_CXX_SOURCES)

# Remove C source files generated by cfdefc.
OBJECTS		+= $(CFDEF_FILES:%.cfdef=$(CFDEF_PREFIX)%.o)
CLEANFILES	+= $(CFDEF_SOURCES)

ifneq	($(strip $(LINK_COPYRIGHT)),)

# Link copyright.o which contains copyright notice.
OBJECTS		+= copyright.o
CLEANFILES	+= $(OBJDIR)/copyright.c $(OBJDIR)/copyright.o

endif	# !empty(LINK_COPYRIGHT)

# If C++ source exists and boost library is specified by BOOST_LIBS,
# boost library is passed to linker.
ifneq	($(strip $(BOOST_LIBS)),)
CXX_LDLIBS		= $(BOOST_LIBDIR:%=-L%) $(BOOST_LIBS)
EXTRA_RUNTIME_DIR	+= $(BOOST_LIBDIR)
endif	# !empty(BOOST_LIBS)

# make clean: Remove files generated by build process.
clean:	$(CLEAN_DEPS) FRC
	$(RM) -rf $(CLEANFILES)

# make clobber: Remove object directory for sub make.
clobber:	$(CLOBBER_DEPS)	FRC
	$(RM) -rf $(CLOBBERFILES)

OBJ_CMDDIR	:= $(OBJDIR)/$(CMDDIR)

#
# Execute command if command line is the same as previous execution.
# $(1):	Makefile variable name which defines command line to be executed.
#	Its name must have "CMDRULE_" as prefix, and the name must be specified
#	without prefix. For instance, if you want to execute "CMDRULE_FOO",
#	you must specify "FOO".
# $(2):	Path to file which preserves command line.
# $(3):	Build prerequisites newer than the target.
#	Command will be executed if this is not empty.
#
CMD_EXECUTE	=							\
	tmpfile=$(2).tmp.$$$$;						\
	echo "$(CMDRULE_$(1))" > $$tmpfile;				\
	prereq="$(strip $(filter-out $(PHONY_TARGET),$(3)))";		\
	if [ -z "$$prereq" -a -f $(2) ]; then				\
	    if $(DIFF) $(2) $$tmpfile > /dev/null 2>&1; then		\
		$(RM) $$tmpfile;					\
		exit 0;							\
	    fi;								\
	fi;								\
	$(MV) $$tmpfile $(2) || exit 1;					\
	$(CMDRULE_$(1))

#
# Generate object file from C source.
#
OBJ_DEPDIR	:= $(OBJDIR)/$(DEPDIR)
CPPFLAGS_DEP	= -Wp,-MD,$(1),-MP
CMDRULE_CC_O	=							\
	depfile="$(OBJ_DEPDIR)/$*.o.d";					\
	deparg=$(call CPPFLAGS_DEP,$$depfile);				\
	echo $(CC) $(CPPFLAGS) $(CFLAGS) -c -o $@ $<;			\
	$(CC) $$deparg $(CPPFLAGS) $(CFLAGS) -c -o $@ $<;		\
	status=$$?;							\
	if [ $$status -eq 0 ]; then					\
	    $(DEPFIX) $$depfile;					\
	else								\
	    $(RM) $@ $$depfile;						\
	fi;								\
	exit $$status

# Compile C source in the current directory.
$(OBJDIR)/%.o $(OBJ_CMDDIR)/%.c.cmd:	%.c FRC
	@$(call CMD_EXECUTE,CC_O,$(OBJ_CMDDIR)/$*.c.cmd,$?)

# Compile auto-generated C source in the object directory.
$(OBJDIR)/%.o $(OBJ_CMDDIR)/%.c.cmd:	$(OBJDIR)/%.c FRC
	@$(call CMD_EXECUTE,CC_O,$(OBJ_CMDDIR)/$*.c.cmd,$?)

# Compile EXP_C_SOURCES with exception support.
EXP_C_OBJECTS	:= $(EXP_C_SOURCES:%.c=$(OBJDIR)/%.o)

ifneq	($(strip $(EXP_C_OBJECTS)),)

$(EXP_C_OBJECTS):	EXTRA_CFLAGS += -fexceptions

endif	# !empty(EXP_C_OBJECTS)

#
# Generate object file from C++ source.
#
CMDRULE_CXX_O	=							\
	depfile="$(OBJ_DEPDIR)/$*.o.d";					\
	deparg=$(call CPPFLAGS_DEP,$$depfile);				\
	echo $(CXX) $(CXX_CPPFLAGS) $(CPPFLAGS) $(CXXFLAGS) -c -o $@ $<; \
	$(CXX) $$deparg $(CXX_CPPFLAGS) $(CPPFLAGS) $(CXXFLAGS) -c -o $@ $<; \
	status=$$?;							\
	if [ $$status -eq 0 ]; then					\
	    $(DEPFIX) $$depfile;					\
	else								\
	    $(RM) $@ $$depfile;						\
	fi;								\
	exit $$status

# Compile C++ source in the current directory.
$(OBJDIR)/%.o $(OBJ_CMDDIR)/%.cc.cmd:	%.cc FRC
	@$(call CMD_EXECUTE,CXX_O,$(OBJ_CMDDIR)/$*.cc.cmd,$?)

# Compile auto-generated C++ source in the object directory.
$(OBJDIR)/%.o $(OBJ_CMDDIR)/%.cc.cmd:	$(OBJDIR)/%.cc FRC
	@$(call CMD_EXECUTE,CXX_O,$(OBJ_CMDDIR)/$*.cc.cmd,$?)

# Preserve auto-generated sources.
.PRECIOUS:	$(OBJDIR)/%.c $(OBJDIR)/%.cc

#
# Generate object file from assembly language source.
#
CMDRULE_S_O	=							\
	depfile="$(OBJ_DEPDIR)/$*.o.d";					\
	deparg=$(call CPPFLAGS_DEP,$$depfile);				\
	echo $(CC) $(AS_CPPFLAGS) $(ASFLAGS) $(CFLAGS) -c -o $@ $<;	\
	$(CC) $$deparg $(AS_CPPFLAGS) $(ASFLAGS) $(CFLAGS) -c -o $@ $<;	\
	status=$$?;							\
	if [ $$status -eq 0 ]; then					\
	    $(DEPFIX) $$depfile;					\
	else								\
	    $(RM) $@ $$depfile;						\
	fi;								\
	exit $$status

# Compile assembly language source in the current directory.
$(OBJDIR)/%.o $(OBJ_CMDDIR)/%.S.cmd:	%.S FRC
	@$(call CMD_EXECUTE,S_O,$(OBJ_CMDDIR)/$*.S.cmd,$?)

#
# Rules to compile PFC configuration file definition.
#
CMDRULE_CFDEF_C	=							\
	set -e;								\
	d="$(OBJ_DEPDIR)/$(notdir $<).d";				\
	echo $(CFDEFC) $(CFDEFC_FLAGS) -D $$d -o $@ $<;			\
	$(CFDEFC) $(CFDEFC_FLAGS) -D $$d -o $@ $<

$(OBJDIR)/$(CFDEF_PREFIX)%.c $(OBJ_CMDDIR)/%.cfdef.cmd:	%.cfdef FRC
	@$(call CMD_EXECUTE,CFDEF_C,$(OBJ_CMDDIR)/$*.cfdef.cmd,$?)

.PRECIOUS:	$(OBJDIR)/$(CFDEF_PREFIX)%.c

ifneq	($(strip $(LINK_COPYRIGHT)),)

# Generate C source file which defines copyright notice.
$(OBJDIR)/copyright.c:	FRC
	@$(GENCOPY) $(OLDEST_YEAR:%=-O %) $(GENCOPY_FLAGS) $@

.PRECIOUS:	$(OBJDIR)/copyright.c

endif	# !empty(LINK_COPYRIGHT)

# Rules to update IPC headers generated by IPC struct template compiler.
ifdef	SKIP_IPC_UPDATE

ipc-header-install:

else	# !SKIP_IPC_UPDATE

ipc-header-install:	FRC
	@$(MAKE) -C $(IPC_TMPLDIR) install

ifneq	($(strip $(OBJ_OBJECTS)),)

$(OBJ_OBJECTS):	$(IPC_HEADERS)

endif	# !empty(OBJ_OBJECTS)

$(IPC_HEADERS):	$(OBJ_IPC_BIN)

$(OBJ_IPC_BIN):	FRC
	@$(MAKE) -C $(IPC_TMPLDIR)

endif	# SKIP_IPC_UPDATE

ifdef	EXPORT_LIBS
# Create symbolic links to library file under EXPORT_LIBDIR.
$(EXPORT_LIBDIR)/%:	$(OBJDIR)/%
	@echo "=== Creating $@";					\
	$(LN_S) $< $@
endif	# EXPORT_LIBS

#
# Import header dependencies.
#
DEPFILES	:= $(OBJECTS:%=$(OBJ_DEPDIR)/%.d) $(CFDEF_DEPFILES)

ifneq	($(strip $(DEPFILES)),)
-include $(DEPFILES)
endif	# !empty(DEPFILES)

FRC:

.PHONY:	FRC

endif	# !BUILD_RULES_MK_INCLUDED
