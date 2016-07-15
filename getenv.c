#include <jni.h>
#include <stdlib.h>     /* getenv */
#include "com_accumulatorx_posix_Environment.h"

JNIEXPORT jstring JNICALL
Java_com_accumulatorx_posix_Environment_getenv(
    JNIEnv *env, 
    jclass cls, 
    jstring jname)
{
    const char *name;
    char *value = NULL;
    jstring jvalue = NULL;

    name = (*env)->GetStringUTFChars(env, jname, NULL);
    if (name != NULL) {
        value = getenv(name);
        if (value != NULL) {
            jvalue = (*env)->NewStringUTF(env, value);
        }
        (*env)->ReleaseStringUTFChars(env, jname, name);
    }
    return jvalue;
}

