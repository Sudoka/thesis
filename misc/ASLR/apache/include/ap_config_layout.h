/* Licensed to the Apache Software Foundation (ASF) under one or more
 * contributor license agreements.  See the NOTICE file distributed with
 * this work for additional information regarding copyright ownership.
 * The ASF licenses this file to You under the Apache License, Version 2.0
 * (the "License"); you may not use this file except in compliance with
 * the License.  You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

/**
 * @file  ap_config_layout.h
 * @brief Apache Config Layout
 */

#ifndef AP_CONFIG_LAYOUT_H
#define AP_CONFIG_LAYOUT_H

/* Configured Apache directory layout */
#define DEFAULT_PREFIX "/home/dcashman/thesis/misc/ASLR/apache"
#define DEFAULT_EXP_EXEC_PREFIX "/home/dcashman/thesis/misc/ASLR/apache"
#define DEFAULT_REL_EXEC_PREFIX ""
#define DEFAULT_EXP_BINDIR "/home/dcashman/thesis/misc/ASLR/apache/bin"
#define DEFAULT_REL_BINDIR "bin"
#define DEFAULT_EXP_SBINDIR "/home/dcashman/thesis/misc/ASLR/apache/bin"
#define DEFAULT_REL_SBINDIR "bin"
#define DEFAULT_EXP_LIBEXECDIR "/home/dcashman/thesis/misc/ASLR/apache/modules"
#define DEFAULT_REL_LIBEXECDIR "modules"
#define DEFAULT_EXP_MANDIR "/home/dcashman/thesis/misc/ASLR/apache/man"
#define DEFAULT_REL_MANDIR "man"
#define DEFAULT_EXP_SYSCONFDIR "/home/dcashman/thesis/misc/ASLR/apache/conf"
#define DEFAULT_REL_SYSCONFDIR "conf"
#define DEFAULT_EXP_DATADIR "/home/dcashman/thesis/misc/ASLR/apache"
#define DEFAULT_REL_DATADIR ""
#define DEFAULT_EXP_INSTALLBUILDDIR "/home/dcashman/thesis/misc/ASLR/apache/build"
#define DEFAULT_REL_INSTALLBUILDDIR "build"
#define DEFAULT_EXP_ERRORDIR "/home/dcashman/thesis/misc/ASLR/apache/error"
#define DEFAULT_REL_ERRORDIR "error"
#define DEFAULT_EXP_ICONSDIR "/home/dcashman/thesis/misc/ASLR/apache/icons"
#define DEFAULT_REL_ICONSDIR "icons"
#define DEFAULT_EXP_HTDOCSDIR "/home/dcashman/thesis/misc/ASLR/apache/htdocs"
#define DEFAULT_REL_HTDOCSDIR "htdocs"
#define DEFAULT_EXP_MANUALDIR "/home/dcashman/thesis/misc/ASLR/apache/manual"
#define DEFAULT_REL_MANUALDIR "manual"
#define DEFAULT_EXP_CGIDIR "/home/dcashman/thesis/misc/ASLR/apache/cgi-bin"
#define DEFAULT_REL_CGIDIR "cgi-bin"
#define DEFAULT_EXP_INCLUDEDIR "/home/dcashman/thesis/misc/ASLR/apache/include"
#define DEFAULT_REL_INCLUDEDIR "include"
#define DEFAULT_EXP_LOCALSTATEDIR "/home/dcashman/thesis/misc/ASLR/apache"
#define DEFAULT_REL_LOCALSTATEDIR ""
#define DEFAULT_EXP_RUNTIMEDIR "/home/dcashman/thesis/misc/ASLR/apache/logs"
#define DEFAULT_REL_RUNTIMEDIR "logs"
#define DEFAULT_EXP_LOGFILEDIR "/home/dcashman/thesis/misc/ASLR/apache/logs"
#define DEFAULT_REL_LOGFILEDIR "logs"
#define DEFAULT_EXP_PROXYCACHEDIR "/home/dcashman/thesis/misc/ASLR/apache/proxy"
#define DEFAULT_REL_PROXYCACHEDIR "proxy"

#endif /* AP_CONFIG_LAYOUT_H */
