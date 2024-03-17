#!/usr/bin/bash

INCLUDE_DIR=include/parsegen
SOURCE_DIR=spets/parsegen

rm -f $INCLUDE_DIR/metaparser.hpp
mv $INCLUDE_DIR/metaparser_candidate.hpp $INCLUDE_DIR/metaparser.hpp

rm -f $SOURCE_DIR/metaparser.cpp
mv $SOURCE_DIR/metaparser_candidate.cpp $SOURCE_DIR/metaparser.cpp
