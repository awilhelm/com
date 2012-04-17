all: client server

client: client.o libcom-example.so

server: server.o libcom-example.so

libcom-example.so: example.o libcom.so
libcom-example.so: LDLIBS = -lboost_serialization-mt -lboost_system-mt -lpthread -lstdc++

libcom.so: com.o
libcom.so: LDLIBS = -lboost_system-mt -lboost_thread-mt -lpthread -lstdc++

clean:; $(RM) $(wildcard *.a *.d *.o *.so) client server

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
	-Wl,-rpath,$(CURDIR)\

