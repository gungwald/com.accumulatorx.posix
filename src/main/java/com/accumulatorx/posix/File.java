package com.accumulatorx.posix;

public class File {
    
    public class Stat {
    	int deviceId;			/* Must hold an int32_t for Darwin */
    	long inode;				/* Must hold a uint32_t for Darwin */
        int protectionMode;		/* Must hold a uint16_t for Darwin */
    }
    
    public static native Stat stat(String path);
}
