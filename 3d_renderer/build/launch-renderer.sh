#!/bin/sh
bindir=$(pwd)
cd /home/park/Workspace/interactive_pyramid_hologram/3d_renderer
export 

if test "x$1" = "x--debugger"; then
	shift
	if test "xYES" = "xYES"; then
		echo "r  " > $bindir/gdbscript
		echo "bt" >> $bindir/gdbscript
		/usr/bin/gdb -batch -command=$bindir/gdbscript --return-child-result /home/park/Workspace/interactive_pyramid_hologram/3d_renderer/build/renderer 
	else
		"/home/park/Workspace/interactive_pyramid_hologram/3d_renderer/build/renderer"  
	fi
else
	"/home/park/Workspace/interactive_pyramid_hologram/3d_renderer/build/renderer"  
fi
