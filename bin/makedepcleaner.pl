#!/usr/bin/perl
#
# $Id: makedepcleaner.pl 1.1 02/01/03 00:59:23-00:00 twouters@ $
#
# Author: Markus Stenberg <fingon@iki.fi>
#
#  Copyright (c) 1998 Markus Stenberg
#       All rights reserved
#
# Created: Sat Jun  6 14:46:57 1998 fingon
# Last modified: Sat Jun  6 14:55:10 1998 fingon
#

# Cleans mess made by makedepend(1) by eliminating system includes from
# the file, and generally prettifying the output.

$lfn = "";
$lft = "";
$lc=0;
$nlc=0;

sub
maybeprint
{
    if ($lfn ne "")
    {
	print "$lfn: $lft\n";
	$nlc++;
    }
    $lfn="";
    $lft="";
}

sub
update
{
    local($fn,$dep)=@_;
    if ($lfn eq $fn)
    {
	$lft .= " $dep";
	return;
    }
    maybeprint;
    $lfn=$fn;
    $lft=$dep;
}
print "# Cleaned by MakeDepCleaner " . scalar(localtime) . "\n";
while (<>)
{
    chomp;
    s/ \/\S+//g;
    # Strip absolute paths
    if (/^(\S+): (\S.*)$/)
    {
	update($1,$2);
    }
    $lc++;
#    print "$_\n";
}
maybeprint;
print "# Original lines: $lc - New lines: $nlc\n";
