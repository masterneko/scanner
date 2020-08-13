# GNU Make workspace makefile autogenerated by Premake

ifndef config
  config=debug
endif

ifndef verbose
  SILENT = @
endif

ifeq ($(config),debug)
  lexer_config = debug
endif
ifeq ($(config),release)
  lexer_config = release
endif

PROJECTS := lexer

.PHONY: all clean help $(PROJECTS) 

all: $(PROJECTS)

lexer:
ifneq (,$(lexer_config))
	@echo "==== Building lexer ($(lexer_config)) ===="
	@${MAKE} --no-print-directory -C . -f lexer.make config=$(lexer_config)
endif

clean:
	@${MAKE} --no-print-directory -C . -f lexer.make clean

help:
	@echo "Usage: make [config=name] [target]"
	@echo ""
	@echo "CONFIGURATIONS:"
	@echo "  debug"
	@echo "  release"
	@echo ""
	@echo "TARGETS:"
	@echo "   all (default)"
	@echo "   clean"
	@echo "   lexer"
	@echo ""
	@echo "For more information, see https://github.com/premake/premake-core/wiki"