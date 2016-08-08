#!/usr/local/bin/perl
# $Id: include_complete.pl 1.1 02/01/03 00:59:23-00:00 twouters@ $
#
# Author: Markus Stenberg <fingon@iki.fi>
#
#  Copyright (c) 1998 Markus Stenberg
#       All rights reserved
#
# Created: Sat Jun  6 16:41:20 1998 fingon
# Last modified: Sat Jun  6 22:21:32 1998 fingon
#


# Complete the set of includes in given file
#  (note: this is akin to black magic, and VERY ugly ;
#  I use it -Wall compile's results and protomaker)

%Paths = ();

sub
possibly_update_includes
{
  local($src,$inc)=@_;
  local($linl, $line);

  $linl = 0; # Last line we find include on (line <= 100)
  $line = 0;
  open(BAR, "$src");
  while (<BAR>)
    {
      if (/^#include \"(\S+)\"\s*$/)
	{
	  if ($1 eq "$inc")
	    {
	      close(BAR);
	      return;
	    }
	  $linl = $line if $line <= 100;
	}
      $line++;
    }
  close(BAR);
  # Need to add.. thank ghod we know where to add, though.
  print "Adding $inc to $src..\n";    
#  return;
  open(BAR, "$src");
  open(BAZ, ">${src}.new");
  $line = 0;
  while (<BAR>)
    {
      if ($line == ($linl + 1))
	{
	  print BAZ "#include \"$inc\"\n";
	}
      print BAZ $_;
      $line++;
    }
  close(BAZ);
#  exit;
  rename("${src}.new",$src);
}

$inc = shift @ARGV;
  
foreach (split(/\-I/,$inc))
{
    s/^\s//g;
    s/\s$//g;
    next if /^\s*$/;
    s/^\-I//;
    $Paths{$_}=1;
#    print "Inc: $_\n";
}
#exit;

$protos = "";
foreach $a (keys %Paths)
{
    if ($a ne ".")
    {
	$protos .= "$a/";
    }
    $protos .= "p.*.h ";
}
#print "test: $protos\n";
#exit;

while (<>)
{
  if (/^(\S+)\:\d+\: warning: implicit declaration of function `(\S+)'$/) #`
  {
    $file = $1;
    $missing = $2;
    print "$file is missing $missing.. propably. looking into it.\n";
    open(FOO, "grep $missing $protos |");
    while (<FOO>)
      {
	if (/^(\S+):(\S+|(const)\s\S+)\s+\**([A-Za-z]\S+)\(/)
	  {
	    $tfile = $1;
	    if ($4 eq $missing)
	      {
		# Found what we were looking for.. Now, possibly add it
		# to the $file:
		$tfile = `basename $tfile`;
		chomp $tfile;
		possibly_update_includes($file, $tfile);
		break;
	      }
	    else
	    {
		print "$4 != $missing\n";
	    }
	}
      }
    close(FOO);
  }
}
