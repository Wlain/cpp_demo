#!/usr/bin/env bash

cd `dirname $0`
MY_DIR=`pwd`


cppcheck	-j12 --file-list="tools/codecheck/file_list.txt" \
			--enable=warning \
			--template="[{severity}][{id}] {message} {callstack} \(On {file}:{line}\)" \
			--verbose	\
			--quiet	\
			--force	\
			--std=c++17	\
			--language=c++	\
			--error-exitcode=1 \
			--suppressions-list="tools/codecheck/cppcheck_suppressions.txt" \


# 错误规则，按需添加到命令中
# --enable=warning,performance,portability,information,missingInclude \

# 规则描述
# error：出现的错误
# warning：为了预防bug防御性编程建议信息
# style：编码格式问题（没有使用的函数、多余的代码等）
# portablity：移植性警告。该部分如果移植到其他平台上，可能出现兼容性问题
# performance：建议优化该部分代码的性能
# information：一些有趣的信息，可以忽略不看的。