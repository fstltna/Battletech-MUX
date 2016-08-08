#!/usr/local/bin/perl
# $Id: generate_limited.pl 1.1 02/01/03 00:59:23-00:00 twouters@ $
#
# Author: Markus Stenberg <fingon@iki.fi>
#
#  Copyright (c) 1998 Markus Stenberg
#       All rights reserved
#
# Created: Mon Jun  8 19:54:41 1998 fingon
# Last modified: Mon Jun  8 20:16:58 1998 fingon
#

# _Inclusive_ selection of files to put under the new exported release

$relname = "muxsrc";

system("rm -rf $relname"); # Eeep!
system("cvs export -Dnow $relname");
open(FOO, "find $relname -print |");
while (<FOO>)
{
    chomp;
    if (/\/nicestuff\//)
    {
	# Always delete these
	unlink($_);
	next;
    }
    next if /.cvsmapfs/;
    next if /\.h$/;          # All include files
    next if /\.pl$/;         # All Perl scripts
    next if /\/bin\//;       # other fun stuff in bin, too
    next if /\/.cvsignore$/; # cvsignore
    next if /mux.general$/;  # general rules
    next if /generate_changelog$/;
    next if /\/hcode\/btech\//;# BT hcode
    print "Deleting: $_\n";
    unlink($_);
}
close(FOO);
system("tar cvfz limited.tgz muxsrc");
system("rm -rf muxsrc")
