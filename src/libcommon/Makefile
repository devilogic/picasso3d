INCLUDE = ../../include
OUTPUT = ../../lib/
ARFLAGS = cr
CXXPPFLAGS = -D_OPENGL
CXXFLAGS = -I$(INCLUDE) -Wno-deprecated
RM = -rm
RMFLAGS = -rf


OBJECTS = container.o crc32.o error.o file.o memdata_converter.o \
memory_pool.o string.o

libcommon.a: $(OBJECTS)
	$(AR) $(ARFLAGS) $(OUTPUT)/$@ $%

# 编译命令
# $(CXX) -o $(OUTPUT)$@ $(CXXFLAGS) $(CXXPPFLAGS) -c $^

container.o: container.cpp ../../include/libcommon/container.hpp \
 ../../include/libcommon/base/base.hpp \
 ../../include/libcommon/base/configure.hpp \
 ../../include/libcommon/base/typedef.hpp \
 ../../include/libcommon/base/except.hpp \
 ../../include/libcommon/string.hpp \
 ../../include/libcommon/memory_pool.hpp \
 ../../include/libcommon/./base/base.hpp \
 ../../include/libcommon/crc32.hpp
	$(CXX) $(OUTPUT)$@ $(CXXFLAGS) $(CXXPPFLAGS) -c $^

crc32.o: crc32.cpp ../../include/libcommon/crc32.hpp \
 ../../include/libcommon/base/base.hpp \
 ../../include/libcommon/base/configure.hpp \
 ../../include/libcommon/base/typedef.hpp \
 ../../include/libcommon/base/except.hpp
	$(CXX) $(OUTPUT)$@ $(CXXFLAGS) $(CXXPPFLAGS) -c $^

error.o: error.cpp ../../include/libcommon/error.hpp \
 ../../include/libcommon/base/base.hpp \
 ../../include/libcommon/base/configure.hpp \
 ../../include/libcommon/base/typedef.hpp \
 ../../include/libcommon/base/except.hpp
	$(CXX) $(OUTPUT)$@ $(CXXFLAGS) $(CXXPPFLAGS) -c $^
file.o: file.cpp ../../include/libcommon/file.hpp \
 ../../include/libcommon/base/base.hpp \
 ../../include/libcommon/base/configure.hpp \
 ../../include/libcommon/base/typedef.hpp \
 ../../include/libcommon/base/except.hpp \
 ../../include/libcommon/error.hpp
	$(CXX) $(OUTPUT)$@ $(CXXFLAGS) $(CXXPPFLAGS) -c $^

memdata_converter.o: memdata_converter.cpp \
 ../../include/libcommon/memdata_converter.hpp \
 ../../include/libcommon/base/base.hpp \
 ../../include/libcommon/base/configure.hpp \
 ../../include/libcommon/base/typedef.hpp \
 ../../include/libcommon/base/except.hpp
	$(CXX) $(OUTPUT)$@ $(CXXFLAGS) $(CXXPPFLAGS) -c $^

memory_pool.o: memory_pool.cpp ../../include/libcommon/memory_pool.hpp \
 ../../include/libcommon/./base/base.hpp \
 ../../include/libcommon/./base/configure.hpp \
 ../../include/libcommon/./base/typedef.hpp \
 ../../include/libcommon/./base/except.hpp
	$(CXX) $(OUTPUT)$@ $(CXXFLAGS) $(CXXPPFLAGS) -c $^

string.o: string.cpp ../../include/libcommon/string.hpp \
 ../../include/libcommon/base/base.hpp \
 ../../include/libcommon/base/configure.hpp \
 ../../include/libcommon/base/typedef.hpp \
 ../../include/libcommon/base/except.hpp
	$(CXX) $(OUTPUT)$@ $(CXXFLAGS) $(CXXPPFLAGS) -c $^

base.o: base/base.cpp ../../include/libcommon/base/base.hpp \
 ../../include/libcommon/base/configure.hpp \
 ../../include/libcommon/base/typedef.hpp \
 ../../include/libcommon/base/except.hpp
	$(CXX) $(OUTPUT)$@ $(CXXFLAGS) $(CXXPPFLAGS) -c $^

clean:
	$(RM) $(RMFLAGS) $(OUTPUT)$(OBJECTS) libcommon.a
