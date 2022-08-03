#!/usr/bin/env bash
MOD_GUILDFUNDS_ROOT="$( cd "$( dirname "${BASH_SOURCE[0]}" )/" && pwd )"

source $MOD_GUILDFUNDS_ROOT"/conf/conf.sh.dist"

if [ -f $MOD_GUILDFUNDS_ROOT"/conf/conf.sh" ]; then
    source $MOD_GUILDFUNDS_ROOT"/conf/conf.sh"
fi
