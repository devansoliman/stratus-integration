# ============================================================
# export_dfg.tcl — batch-friendly DFG text export
# Usage:
#   open_project project.tcl
#   set argv {BASIC thread_function}
#   set argc [llength $argv]
#   source export_dfg.tcl

# ============================================================

# ---- Argument parsing ----
if { $argc < 1 } {
    puts "Usage: stratus -f export_dfg.tcl <hls_config> [<behavior_name>]"
    exit 1
}
set config   [lindex $argv 0]
set behavior [expr { $argc >= 2 ? [lindex $argv 1] : "" }]

puts ">>> [clock format [clock seconds]] : export_dfg starting (config=$config, behavior=$behavior)"

# ---- Open project directly ----
open_project project.tcl

# ---- Find configuration ----
set hcfg [find -hls_config $config]
if { $hcfg eq "" } {
    puts "ERROR: hls_config '$config' not found."
    exit 1
}
load_results $hcfg -snapshot optim

# ---- Collect operations ----
if { $behavior ne "" } {
    set all_ops [find -op]
    # Keep only those whose full name includes the behavior name
    set ops {}
    foreach op $all_ops {
        set name [get_attr name $op]
        if {[string match *$behavior* $op]} {
            lappend ops $op
        }
    }
} else {
    set ops [find -op]
}

# ---- Helper ----
proc safe_get_attr {attr obj} {
    set val [get_attr $attr $obj]
    if { $val eq "unset" } { return "" }
    return $val
}

puts "Found [llength $ops] operations in behavior '$behavior'"

# ---- Nodes ----
puts "### DATA FLOW GRAPH NODES (op_id | name | kind | src_links) ###"
foreach op $ops {
    set name [safe_get_attr name $op]
    set kind [safe_get_attr kind $op]
    set srcs [safe_get_attr src_links $op]
    if {[llength $srcs] > 1} { set srcs [join $srcs "; "] }
    puts "$op | $name | $kind | $srcs"
}

# ---- Edges ----
puts "### DATA FLOW EDGES (src_op -> dst_op) ###"
foreach op $ops {
    set succs [get_attr reg_op $op]
    if {$succs eq "unset"} { continue }
    foreach dst $succs {
        puts "$op -> $dst"
    }
}

puts ">>> [clock format [clock seconds]] : export_dfg finished"
exit
