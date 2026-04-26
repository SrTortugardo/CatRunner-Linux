TARGET = catrunner
CC = gcc # not tested on clang

PREFIX ?= /usr
BINDIR = $(PREFIX)/bin
DATADIR = $(PREFIX)/share/catrunner

CFLAGS = -Wall -Wextra -O2 -Icore/gif -Icore/gpu
LDFLAGS = -lX11 -lm

SRC = main.c \
      core/gif/gif.c \
      core/gpu/gpu.c

all: $(TARGET)

$(TARGET):
	$(CC) $(CFLAGS) $(SRC) -o $(TARGET) $(LDFLAGS)


install: $(TARGET)
	mkdir -p $(DESTDIR)$(BINDIR)
	mkdir -p $(DESTDIR)$(DATADIR)

	install -m 755 $(TARGET) $(DESTDIR)$(BINDIR)/$(TARGET)
	cp -r resources/frame_*.png $(DESTDIR)$(DATADIR)/

install-desktop:
	mkdir -p $(HOME)/.local/share/applications
	cp catrunner.desktop $(HOME)/.local/share/applications/
	chmod +x $(HOME)/.local/share/applications/catrunner.desktop

clean:
	rm -f $(TARGET)

re: clean all
