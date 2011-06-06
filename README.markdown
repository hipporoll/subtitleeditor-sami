## A SAMI subtitle plugin for subtitleeditor
- 

## Overview

 Subtitleeditor <http://home.gna.org/subtitleeditor/> is a GUI subtitle editor on Linux desktop. This application, however, does not natively support the SAMI (\*.smi) subtitle format. But with this patch you can read and write SAMI files without any additional converting tool.

## Installation

### release 0.38

```
$ wget http://download.gna.org/subtitleeditor/0.38/subtitleeditor-0.38.0.tar.gz
$ wget https://github.com/advance38/subtitleeditor-sami/blob/master/subtitleeditor-sami-0.38.0.patch
$ tar xzf subtitleeditor-0.38.0
$ cd subtitleeditor-0.38.0
$ patch -p1 < ../subtitleeditor-sami-0.38.0.patch
$ ./configure
$ make
# make install
```

### trunk version

```
$ svn checkout http://svn.gna.org/svn/subtitleeditor/trunk subtitleeditor
$ wget https://github.com/advance38/subtitleeditor-sami/blob/master/subtitleeditor-sami-trunk.patch
$ cd subtitleeditor
$ patch -p1 < ../subtitleeditor-sami-trunk.patch
$ ./autogen.sh
$ make
# make install
```

## Implementation

### read

```
```

### write

```
```

## Etc

 * Authors
  - Dongsu Park advance38@gmail.com

 * For the more infos, refer to http://home.gna.org/subtitleeditor/.

