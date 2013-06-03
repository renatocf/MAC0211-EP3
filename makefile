# PROGRAMS #############################################################
AR := ar
CC := gcc
RM := rm -f
SED := sed
CAT := cat
MAKE += --no-print-directory
MKDIR := mkdir -p
RMDIR := rmdir --ignore-fail-on-non-empty

# DIRECTORIES ##########################################################
SRCDIR := src
OBJDIR := obj
BINDIR := bin
LIBDIR := lib
DOCDIR := doc
CONFDIR := conf
TESTDIR := test
VPATH = $(SRCDIR):$(LIBDIR):$(BINDIR):$(TESTDIR)

# SOURCE ###############################################################
BIN := ep2
SRC := $(shell ls $(SRCDIR))
LIB := $(CONFDIR)/libraries.mk
DEP := $(CONFDIR)/dependencies.mk

include $(LIB)
OBJ := $(filter-out $(ARLIB) $(SOLIB),$(SRC)) # Tira bibliotecas
OBJ := $(patsubst %.c,%.o,$(OBJ))             # Substitui .c por .o
OBJ := $(addprefix $(OBJDIR)/,$(OBJ))         # Adiciona diretório

# COMPILER #############################################################
CLIBS  := $(patsubst %,-I%,$(shell find include -type d))
CFLAGS := -ansi -Wall -pedantic -g -fPIC

# LINKER ###############################################################
LDLIBS  := -L$(LIBDIR)
LDFLAGS := -Wl,-rpath,$(LIBDIR)
LDFLAGS += -lm
LDFLAGS += $(filter -l%,$(patsubst lib%.a,-l%,$(LIBS))) \
 		   $(filter -l%,$(patsubst lib%.so,-l%,$(LIBS)))


.PHONY: all
all: $(DEP) $(BIN) $(DOC)
-include $(DEP)

.PHONY: doc
doc:
	$(MAKE) -C $(DOCDIR)

.PHONY: clean
clean:
	$(RM) $(OBJDIR)/*.o $(LIBDIR)/*.a $(LIBDIR)/*.so \
      $(CONFDIR)/dependencies.mk *~ gmon.out
	$(RMDIR) $(OBJDIR)
	@$(MAKE) clean -C $(DOCDIR)


# GAME #################################################################
$(BIN): $(OBJ) | $(LIBS) $(BINDIR)
	$(CC) $^ -o $(BINDIR)/$@ $(LDLIBS) $(LDFLAGS)

$(OBJ): | $(OBJDIR)

# BUILDS ###############################################################
lib%.so: $(SRCDIR)/%.c
	$(CC) -fPIC $(CFLAGS) $(CLIBS) -c $< -o $(OBJDIR)/$*.o
	$(CC) -o $(LIBDIR)/$@ $(SOFLAGS) $(OBJDIR)/$*.o $(OBJDIR)/utils.o

lib%.a: $(OBJDIR)/$(notdir %.o)
	$(AR) $(ARFLAGS) $(LIBDIR)/$@ $<

$(OBJDIR)/%.o: $(SRCDIR)/%.c
	$(CC) $(CFLAGS) $(CLIBS) -c $< -o $@
	
# TESTS ################################################################
%: $(TESTDIR)/%.c
	$(CC) $< $(CFLAGS) $(CLIBS) -o $(BINDIR)/test$* $(LDLIBS) $(LDFLAGS)
	
# DEPENDENCIES #########################################################
$(DEP): $(SRC) | $(OBJDIR) $(LIBDIR)
	$(CC) $(SRCDIR)/* $(CLIBS) -MM $(LDLIBS) > $@
	$(SED) -e 's/\.o/\.c/' -e 's/: .*c /: /' -i $@

# GENERATED DIR ########################################################
$(BINDIR):
	@ echo Criando diretório de binários "$@"
	-$(MKDIR) $@

$(OBJDIR):
	@ echo Criando diretório de objetos "$@"
	-$(MKDIR) $@

$(LIBDIR):
	@ echo Criando diretório de objetos "$@"
	-$(MKDIR) $@
