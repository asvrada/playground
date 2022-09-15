TOP=$(abspath ../..)
include $(TOP)/defs.mak

APPDIR = appdir
CFLAGS = -fPIC -g
LDFLAGS = -Wl,-rpath=$(MUSL_LIB)

OPTS = --app-config-path config.json

ifdef STRACE
	OPTS += --strace
endif

# Move this folder to Mystikos/tests to run

all:
	$(MAKE) myst
	$(MAKE) rootfs

rootfs: main.c
	mkdir -p $(APPDIR)/bin
	$(MUSL_GCC) $(CFLAGS) main.c -o $(APPDIR)/bin/main $(LDFLAGS)
	$(MUSL_GCC) $(CFLAGS) echo.c -o $(APPDIR)/bin/echo $(LDFLAGS)
	$(MYST) mkcpio $(APPDIR) rootfs

tests:
	$(RUNTEST) $(MYST_EXEC) rootfs $(OPTS) /bin/main $(ARGS)

myst:
	$(MAKE) -C $(TOP)/tools/myst

clean:
	rm -rf $(APPDIR) rootfs

gdb:
	$(MYST_GDB) --args $(MYST_EXEC) rootfs $(OPTS) /bin/main $(ARGS)
