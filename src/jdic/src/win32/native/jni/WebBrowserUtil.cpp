/*
 * Copyright (C) 2004 Sun Microsystems, Inc. All rights reserved. Use is
 * subject to license terms.
 * 
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the Lesser GNU General Public License as
 * published by the Free Software Foundation; either version 2 of the
 * License, or (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307
 * USA.
 */ 

#include <jawt_md.h>
#include <jawt.h>
#include "WebBrowserUtil.h"
#include <stdlib.h>
#include <stdio.h>

/*
 * Class:     org_jdesktop_jdic_browser_WebBrowserUtil
 * Method:    nativeGetBrowserPath
 * Signature: ()Ljava/lang/String;
 */
JNIEXPORT jstring JNICALL Java_org_jdesktop_jdic_browser_WebBrowserUtil_nativeGetBrowserPath
  (JNIEnv *env, jobject)
{
    HKEY hkey;
    DWORD type, cb;
    char *p, browserPath[256] = "\0";
    
#ifdef DEBUG
    // use mozilla if MOZILLA_FIVE_HOME defined (this is useful for debugging)
    p = getenv("MOZILLA_FIVE_HOME");
    if (p) {
        strncpy(browserPath, p, 244);
        free(p);
        strcat(browserPath, "\\mozilla.exe");
        return env->NewStringUTF(browserPath);
    }
#endif

    // get the default http protocol handler
    if (RegOpenKey(HKEY_CLASSES_ROOT, "http\\shell\\open\\command", &hkey) != ERROR_SUCCESS)
        return 0;
    
    cb = sizeof(browserPath);
    if (RegQueryValueEx(hkey, "", 0, &type, (LPBYTE)browserPath, &cb) != ERROR_SUCCESS)
        return 0;
    //remove the trailing part after the first space character at the rear of .exe
    char* lwrBrowserPath = _strlwr(_strdup(browserPath));
    char* exeStr = strstr(lwrBrowserPath, ".exe");
    int exeIndex = 0;
    if (exeStr != NULL)
    {
        exeIndex = exeStr - lwrBrowserPath;
    }
    cb = strlen(browserPath);
    for (int i = exeIndex; i < (int)cb; i++)
    {
        if (browserPath[i] == ' ') 
        {
            browserPath[i] = '\0';
            break;   
        }
    }
    
    RegCloseKey(hkey);
   
    p = strstr(strlwr(browserPath), "mozilla.exe");
    if (p == NULL) {
        _putenv("JAVA_PLUGIN_WEBCONTROL_ENABLE=1");
    }
    return env->NewStringUTF(browserPath);
}

/*
 * Class:     org_jdesktop_jdic_browser_WebBrowserUtil
 * Method:    nativeSetEnv
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_org_jdesktop_jdic_browser_WebBrowserUtil_nativeSetEnv
  (JNIEnv *env, jclass)
{
    _putenv("JAVA_PLUGIN_WEBCONTROL_ENABLE=1");
}