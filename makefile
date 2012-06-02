all: client server

client: client.o libcom-example.so libcom.so
client: LDLIBS = -lboost_thread

server: server.o libcom-example.so libcom.so

libcom-example.so: example.o libcom.so
libcom-example.so: LDLIBS = -lboost_serialization -lboost_system -lpthread -lstdc++

libcom.so: com.o
libcom.so: LDLIBS = -lboost_system -lboost_thread -lpthread -lstdc++

DESTDIR ?= /usr/local

clean:; $(RM) $(wildcard *.a *.d *.o *.so) client server

install: $(addprefix $(DESTDIR)/include/com/,com.h com.hh stub.h stub.hh)
install: $(addprefix $(DESTDIR)/lib/,libcom.so)

$(DESTDIR)/include/com/% $(DESTDIR)/lib/%: %
	@ mkdir -p $(@D)
	install $< $@

%.so:; $(LINK.o) -shared -fpic -o $@ $^ $(LDLIBS)

include $(wildcard *.d)

CPPFLAGS +=\
	-MMD\
	-MP\
	-Wall\
	-Wextra\
	-Wconversion\
	-Werror\

LDFLAGS +=\
	-Wl,--unresolved-symbols=ignore-in-shared-libs\
	-Wl,--fatal-warnings\
	-Wl,--as-needed\

