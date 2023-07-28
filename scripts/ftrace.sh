#!/usr/bin/env bash
prepare_val=0
filter_val=0
trace_val=0

comment_prepare="[-p ftrace prepare]"
comment_filter="[-f hook the driver filter]"
comment_trace="[-t 0/1 trace enable]"


while getopts ":pft:" option; do
    case "${option}" in
        p)
            prepare_val=1
            ;;
        f)
            filter_val=1
            ;;
        t)
            trace_val=1
            trace_on="$OPTARG"
            echo "trace_on=$trace_on"
            ;;
        :)
            echo "Error: -${OPTARG} requires an argument"
            exit 1
            ;;
        *)
            echo "Usage: $(basename $0) $comment_prepare $comment_filter $comment_trace"
            exit 1
            ;;
    esac
done


echo "p = ${prepare_val}"
echo "f = ${filter_val}"
echo "t = ${trace_val}"

if [ $prepare_val -eq 1 ]; then
    mount -t debugfs nodev /sys/kernel/debug
    echo 0 > /sys/kernel/debug/tracing/tracing_on
    echo 0 > /sys/kernel/debug/tracing/trace
    echo 65535 > /sys/kernel/debug/tracing/buffer_size_kb
    echo "mono" > /sys/kernel/debug/tracing/trace_clock
fi
# Set ftrace filter
if [ $prepare_val -eq 1 ]; then
    echo :mod:wlan_mt7923_pcie > /sys/kernel/debug/tracing/set_ftrace_filter
    echo function_graph > /sys/kernel/debug/tracing/current_tracer
fi


# Start trace
if [ $prepare_val -eq 1 ]; then
    echo $trace_on > /sys/kernel/debug/tracing/trace_on
fi
