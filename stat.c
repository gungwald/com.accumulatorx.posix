#include <jni.h>
#include <stdlib.h>     /* getenv */
#include <errno.h>

#include <sys/types.h>
#include <sys/stat.h>	/* struct stat */

#include "com_accumulatorx_posix_File.h"

enum StatResult { STAT_FAILURE = -1, STAT_SUCCESS = 0 };

static jclass
findStatClass(JNIEnv *env)
{
    return (*env)->FindClass(env, "com/accumulatorx/posix/File$Stat");
}

static jobject
new_Stat(JNIEnv *env, jclass statClass)
{
    jmethodID statConstructor = NULL;
    jobject jFileStatus = NULL;

    statConstructor = (*env)->GetMethodID(env, statClass, "<init>", "()V");
    if (statConstructor != NULL) {
    	jFileStatus = (*env)->NewObject(env, statClass, statConstructor);
        if (jFileStatus == NULL) {
        	/* Handle error */
        }
    }
    else {
    	/* Handle error */
    }
    return jFileStatus;
}

static jobject
convertCStructStatToJava(JNIEnv *env, struct stat *fileStatus)
{
    jclass statClass = NULL;
    jobject jFileStatus = NULL;
    jfieldID deviceIdField = NULL;
    jfieldID inodeField = NULL;
    jfieldID protectionModeField = NULL;

    if ((statClass = findStatClass(env)) != NULL) {
        if ((jFileStatus = new_Stat(env, statClass)) != NULL) {

            deviceIdField = (*env)->GetFieldID(env, statClass, "deviceId", "I");
            (*env)->SetIntField(env, jFileStatus, deviceIdField, fileStatus->st_dev);

            inodeField = (*env)->GetFieldID(env, statClass, "inode", "L");
            (*env)->SetLongField(env, jFileStatus, inodeField, fileStatus->st_ino);

            protectionModeField = (*env)->GetFieldID(env, statClass, "protectionMode", "I");
            (*env)->SetIntField(env, jFileStatus, protectionModeField, fileStatus->st_mode);
        }
    }
    return jFileStatus;
}

JNIEXPORT jobject JNICALL
Java_com_accumulatorx_posix_File_stat(
    JNIEnv *env, 
    jclass cls, 
    jstring jPathOfFileToStat)
{
    const char *pathOfFileToStat;
    struct stat fileStatus;
    jobject jFileStatus = NULL;

    pathOfFileToStat = (*env)->GetStringUTFChars(env, jPathOfFileToStat, NULL);
    if (pathOfFileToStat != NULL) {
        if (stat(pathOfFileToStat, &fileStatus) == STAT_SUCCESS) {
            jFileStatus = convertCStructStatToJava(&fileStatus);
        }
        else {
        	if (errno == ENOTDIR) {
        		/* Throw NotDirectoryPosixException */
        	}
        }
        (*env)->ReleaseStringUTFChars(env, jPathOfFileToStat, pathOfFileToStat);
    }
    return jFileStatus;
}

