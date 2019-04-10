#!/bin/bash

<$1 tr ' ' '\n' | awk '{ if ( length >a ) { a = length; b=$0 }}END{ print a" "b }'
