#!/usr/bin/env bash
DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
cd "$DIR"
OUTPUT_DIR="$DIR"
MAKE_LUABINDING="$QMM_ROOT"/bin/compile_luabinding.sh
$MAKE_LUABINDING -E MyClass -d "$OUTPUT_DIR" MobClickCppForLua_luabinding.tolua
