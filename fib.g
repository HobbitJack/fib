args	"-F fib -c g.c -H g.h -uTARGET --func-name ggo --show-required --default-optional --no-help --no-version -G"

package "fib"
version "4.1.1"

description	"Print the TARGETth Fibonacci number.\nIf no TARGETs, read from standard input."

section "Options"
option	"base" b "Output in base BASE, [-32, -2] or [2, 62]" int typestr="BASE" default="10"
option	"loose-exit-status" l "Exit with 0 even if invalid input encountered"
option	"max-value" m "Print largest Fibonacci number less than TARGET"
option	"print-all" p "Print all Fibonacci numbers up to result"
option	"quiet" q "Don't print TARGET before output"
option	"silent" s "Don't print error messages"
section	"Getting help"
option	"help" h "Print this help message and exit"
option	"version" v "Print version information and exit"
text	"\nTry 'man fib' for more information."

versiontext	"Copyright (C) 2026 Jack Renton Uteg.\nLicense GPLv3+: GNU GPL version 3 or later <https://gnu.org/licenses/gpl.html>.\nThis is free software: you are free to change and redistribute it.\nThere is NO WARRANTY, to the extent permitted by law.\n\nWritten by Jack R. Uteg."
