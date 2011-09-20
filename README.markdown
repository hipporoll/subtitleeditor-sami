## A SAMI subtitle plugin for subtitleeditor

## Overview

 Subtitleeditor <http://home.gna.org/subtitleeditor/> is a GUI subtitle editor on Linux desktop. This application, however, does not natively support the SAMI (\*.smi) subtitle format. But with this patch you can read and write SAMI files without any additional converting tool.

## Installation


### preparation

Install development packages to build subtitleeditor.

On Debian/Ubuntu systems:

```
$ sudo apt-get install gstreamer0.10-plugins-base intltool libenchant-dev libglibmm-2.4-dev libgstreamer0.10-dev libgstreamer-plugins-base0.10-dev libgstreamermm-0.10-dev libgtkmm-2.4-dev libxml++2.6-dev

```

On Fedora/RedHat systems:

```
$ sudo yum install enchant-devel glibmm24-devel gstreamer-devel gstreamer-plugins-base-devel gtkmm24-devel intltool libxml++-devel
```

### compile directly on trunk

```
$ git clone git@github.com:advance38/subtitleeditor-sami.git
$ cd subtitleeditor-sami/trunk
$ ./autogen.sh
$ ./configure
$ make
$ sudo make install
```

### patch against release 0.39

```
$ wget http://download.gna.org/subtitleeditor/0.39/subtitleeditor-0.39.0.tar.gz
$ wget https://github.com/advance38/subtitleeditor-sami/blob/master/subtitleeditor-sami-0.39.0.patch
$ tar xzf subtitleeditor-0.39.0
$ cd subtitleeditor-0.39.0
$ patch -p1 < ../subtitleeditor-sami-0.39.0.patch
$ ./configure
$ make
$ sudo make install
```

## Implementation

### read

The Sami subtitle format consists of multiple HTML-like tags, e.g. ``<Sync>, <P>, <Body>.``
Upon parsing, we can determine the current states according to the occurrence of predefined tags.

* INIT : the beginning of parsing
* SYNC_START : if any ``"<SYNC Start"`` appears, control flow goes into SYNC_START state, until we meet ``"<P>"`` or ``"<SYNC>"``.
* P_OPEN : if any ``"<P"`` appears, we go into P_OPEN state, until the P tag closes.
* P_CLOSE : if the P tag closes with ``">"``, then we go into P_CLOSE state, until the SYNC end delimiter appears.
* SYNC_END : if SYNC end delimiter appears, sync action finishes, and start parsing again from the INIT state.

### write

Write action is simpler than read, because the application simply writes subtitle entries from memory to file line by line. Only necessary syntax elements are stored together with subtitle text and sync timing information.

## Etc

 * Authors
  - Dongsu Park advance38@gmail.com

 * For the more infos, refer to http://home.gna.org/subtitleeditor/.

