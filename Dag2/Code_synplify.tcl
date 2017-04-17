#-- Lattice Semiconductor Corporation Ltd.
#-- Synplify OEM project file e:/byggern/dag2\Code_synplify.tcl
#-- Written on Wed Sep 17 09:04:51 2014


#device options
set_option -technology NoPkgName

#compilation/mapping options

#map options

#simulation options
set_option -write_verilog false
set_option -write_vhdl false

#timing analysis options
set_option -synthesis_onoff_pragma false

#-- add_file options
add_file -vhdl -lib work "code.vhd"

#-- top module name
set_option -top_module Code

#-- set result format/file last
project -result_file "Code.edi"

#-- error message log file
project -log_file code.srf

#-- run Synplify with 'arrange VHDL file'
project -run
