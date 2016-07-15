#
# Builds which.jar with standard Java or OpenJDK and MinGW gcc.
# Make sure the bit length of Java and gcc match 64 with 64 or 32 with 32.
#
# Warning: Windows will not grab the getenv.dll in the local directory
# by default if there is another one in the PATH. Argh. I though I was
# generating a 32-bit getenv.dll for a long time because it was still
# grabing the old 32-bit version at runtime. Use "depends" to check the
# bitness of the getenv.dll file.
#

CC=gcc
JNI_INC="-I/System/Library/Frameworks/JavaVM.framework/Versions/CurrentJDK/Headers" "-I/Developer/SDKs/MacOSX10.6.sdk/System/Library/Frameworks/JavaVM.framework/Versions/A/Headers"
JAVA_CLASSES=build/classes/main
TARGET=libposix.jnilib

all: $(TARGET)

$(TARGET): getenv.o stat.o
	$(CC) -dynamiclib -o $(TARGET) getenv.o stat.o

getenv.o: getenv.c com_accumulatorx_posix_Environment.h
	$(CC) -Wall -D_JNI_IMPLEMENTATION_ $(JNI_INC) -c getenv.c
	
stat.o: stat.c com_accumulatorx_posix_File.h
	$(CC) -Wall -D_JNI_IMPLEMENTATION_ $(JNI_INC) -c stat.c
	
com_accumulatorx_posix_File.h: $(JAVA_CLASSES)/com/accumulatorx/posix/File.class
	javah -classpath $(JAVA_CLASSES) -jni com.accumulatorx.posix.File

com_accumulatorx_posix_Environment.h: $(JAVA_CLASSES)/com/accumulatorx/posix/Environment.class
	javah -classpath $(JAVA_CLASSES) -jni com.accumulatorx.posix.Environment

clean:
	rm *.o $(TARGET) com_accumulatorx_posix_*.h

