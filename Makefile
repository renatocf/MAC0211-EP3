#######################################################################
#                             OPTIONS                                 #
#######################################################################

# PROGRAMS #############################################################
AR := ar
CC := gcc
CP := cp -f
RM := rm -f
SED := sed
FMT := fmt -1
CAT := cat
LEX  := flex
YACC := bison
FIND = find
MAKE += --no-print-directory
MKDIR := mkdir -p
RMDIR := rmdir --ignore-fail-on-non-empty

# DIRECTORIES ##########################################################
SRCDIR := src
OBJDIR := obj
BINDIR := bin
LIBDIR := lib
DOCDIR := man
CONFDIR := conf
TESTDIR := test
HEADDIR := include

-include $(CONFDIR)/directories.mk
VPATH = $(CONFDIR):$(SRCDIR):$(LIBDIR):$(BINDIR):$(TESTDIR):$(HEADDIR)

# SOURCE ###############################################################
BIN := ep3
SRC := $(notdir $(shell ls $(SRCDIR)/*.c))
LIB := $(CONFDIR)/libraries.mk
DEP := $(addprefix $(CONFDIR)/,$(SRC:.c=.d))

-include $(LIB)
OBJ := $(filter-out $(ARLIB) $(SOLIB),$(SRC)) # Tira bibliotecas
OBJ := $(patsubst %.c,%.o,$(OBJ))             # Substitui .c por .o
OBJ := $(addprefix $(OBJDIR)/,$(OBJ))         # Adiciona diretório

# COMPILATION ##########################################################
CLIBS  := -I. $(patsubst %,-I%,$(filter-out .%,$(shell $(FIND) $(HEADDIR) -type d)))
CFLAGS := -ansi -Wall -pedantic -g -fPIC
CFLAGS  += -Wno-implicit-function-declaration

# LINKAGE ##############################################################
LDLIBS   = -L. $(patsubst %,-L%,$(filter-out .%,$(shell $(FIND) $(LIBDIR) -type d)))
LDFLAGS := -lm
LDFLAGS += -lallegro -lallegro_primitives
LDFLAGS += -Wl,-rpath,$(LIBDIR)
LDFLAGS += $(filter -l%,$(patsubst lib%.a,-l%,$(LIBS))) \
 		   $(filter -l%,$(patsubst lib%.so,-l%,$(LIBS)))

########################################################################
#                               BUILD                                  #
########################################################################

# OPTIONS ##############################################################
.PHONY: all
all: $(DEP) $(addprefix $(BINDIR)/,$(BIN))
-include $(DEP)

.PHONY: doc
doc:
	$(MAKE) -C $(DOCDIR)

# CLEAN ################################################################
.PHONY: clean
clean:
	$(RM) $(OBJDIR)/*.o $(LIBDIR)/*.a $(LIBDIR)/*.so
	$(RM) $(SRCDIR)/*~ $(HEADDIR)/*~
	$(RM) $(DEP)
	-$(RMDIR) $(OBJDIR) 2> /dev/null
ifneq ($(BINDIR),.)
	@-$(MAKE) clean -C $(DOCDIR)
endif

.PHONY: distclean
distclean:
	$(RM) $(BINDIR)/$(BIN) $(CONFDIR)/*.d
	-$(RMDIR) $(BINDIR) $(LIBDIR) 2> /dev/null

# EXECUTABLE ###########################################################
$(BINDIR)/$(BIN): $(OBJ) | $(LIBS) $(BINDIR)
	echo $(LDLIBS)
	$(CC) $^ -o $@ $(LDLIBS) $(LDFLAGS)

$(OBJDIR)/%.o: $(addprefix $(SRCDIR)/,%.c)
	$(CC) $(CLIBS) $(CFLAGS) -c $< -o $@

$(OBJ): | $(OBJDIR)
$(LIBS): | $(LIBDIR)

# DEPENDENCIES #########################################################
$(CONFDIR)/%.d: $(addprefix $(SRCDIR)/,%.c)
	$(CC) $(CLIBS) -MM $< 1> $@
	@$(CP)  $@ $@.tmp
	@$(SED) -e 's/.*:/$(OBJDIR)\/$*.o:/' -i $@
	@$(SED) -e 's/.*://' -e 's/\\$$//' < $@.tmp | $(FMT) | \
	  $(SED) -e 's/^ *//' -e 's/$$/:/' >> $@
	@$(RM) $@.tmp

# STATIC LIBRARIES #####################################################
lib%.a: $(OBJDIR)/$(notdir %.o)
	$(AR) $(ARFLAGS) $(LIBDIR)/$@ $<

# SHARED LIBRARIES #####################################################
lib%.so: $(SRCDIR)/%.c
	$(CC) -fPIC $(CFLAGS) $(CLIBS) -c $< -o $(OBJDIR)/$*.o
	$(CC) -o $(LIBDIR)/$@ $(SOFLAGS) $(OBJDIR)/$*.o 

# TESTS ################################################################
%: $(TESTDIR)/%.c
	$(CC) $< $(CFLAGS) $(CLIBS) -o $(BINDIR)/test$* $(LDLIBS) $(LDFLAGS)


########################################################################
#                            GENERATE DIRS                             #
########################################################################

ifneq ($(SRCDIR),.)
$(SRCDIR):
	@ echo Creating directory for source "$@"
	-$(MKDIR) $@
endif

ifneq ($(OBJDIR),.)
$(OBJDIR):
	@ echo Creating directory for objects "$@"
	-$(MKDIR) $@
endif

ifneq ($(BINDIR),.)
$(BINDIR):
	@ echo Creating directory for binaries "$@"
	-$(MKDIR) $@
endif

ifneq ($(LIBDIR),.)
$(LIBDIR):
	@ echo Creating directory for libraries "$@"
	-$(MKDIR) $@
endif

ifneq ($(DOCDIR),.)
$(DOCDIR):
	@ echo Creating directory for documents "$@"
	-$(MKDIR) $@
endif

ifneq ($(CONFDIR),.)
$(CONFDIR):
	@ echo Creating directory for config files "$@"
	-$(MKDIR) $@
endif

ifneq ($(TESTDIR),.)
$(TESTDIR):
	@ echo Creating directory for tests "$@"
	-$(MKDIR) $@
endif

ifneq ($(HEADDIR),.)
$(HEADDIR):
	@ echo Creating directory for headers "$@"
	-$(MKDIR) $@
endif
