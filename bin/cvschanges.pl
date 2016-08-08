#!/usr/bin/perl
# $Id: cvschanges.pl 1.1 02/01/03 00:59:23-00:00 twouters@ $
#
# Author: Markus Stenberg <fingon@iki.fi>
#
#  Copyright (c) 1998 Markus Stenberg
#       All rights reserved
#
# Created: Sat Jun  6 23:06:51 1998 fingon
# Last modified: Fri Jun 12 23:08:20 1998 fingon
#

$nowdir="";
$oldmsg="";
$oldtime="";
$olddate="";
$olduser="";
$olddir="";
$oldcnt=0;

@data = ();
$text = "";

$rcsempty = "*** empty log message ***\n";
$nomsg = "-*No message given*-\n";

%Deleted = ();
%Changed = ();
%Added = ();

sub numeric_time
{
    local($time,$date)=@_;
    local($tb)=(0);
    if ($time =~ /^(\d+)\:(\d+)$/)
    {
	$tb = $1 * 60 + $2;
	if ($date =~ /^(\d+)\.(\d+)$/)
	{
	    $tb = $tb + 86400 * ($1 + 31 * $2);
	    return $tb;
	}
    }
    # Okay, it bugs a bit in change of month, but well.. can't have everything,
    # neh?
    return 0;
}

sub issame
{
    local($time, $date, $user, $dir)=@_;

    return 0 if ($user ne $olduser);
    return 0 if ($olddir ne $dir);
    return 0 if (numeric_time($time,$date) > (1 + numeric_time($oldtime, $olddate)));
    return 1;
}

sub
getmsg
{
    local($time, $date, $user, $name,$dir,$ver)=@_;
    local($smokin,$logmsg);

    return $oldmsg if issame($time, $date, $user, $dir);
    $olddir = $dir;
    $olduser = $user;
    $oldtime = $time;
    $olddate = $date;
    $smokin=0;
    $logmsg="";
#    print "Checking: $name - $dir - $ver\n";
    open(FOO,"(cd $dir ; cvs -n log -r$ver $name) |");
    while (<FOO>)
    {
	$smokin = 0 if (/^\={60,}$/);
	$logmsg .= $_ if $smokin > 1;
#	$logmsg .= ">>$_" if $smokin <= 1;
	if (/^revision (\S+)\s*$/ && $smokin == 0)
	{
	    $smokin = 1 if ($1 eq $ver);
	}
	if (/^date:/ && $smokin==1)
	{
	    $smokin = 2;
	}
    }
    close(FOO);
    return $logmsg;
}
sub new_dir
{
    local($time,$date,$user,$dir)=@_;

    push(@data, $text);
    $text = "=" x 78 . "\n";
    $text .= "$time / $date - $user @ $dir:\n";
    $oldmsg="";
    $oldtime="";
    $olddate="";
    $olduser="";
    $olddir="";
    $oldcnt=0;
    $nowdir = $dir;
    %Deleted = ();
    %Changed = ();
    %Added = ();
}

sub
    dumpold
{
    local($cnt);

    return if $oldcnt==0;
    $cnt=0;
    # Dump affected files:
    foreach $a (keys %Deleted)
    {
	$text .=  "Removed: " if ($cnt++)==0;
	$text .= "$a ";
    }
    $text .= "\n" if $cnt>0;
    $cnt=0;
    foreach $a (keys %Changed)
    {
	$text .= "Changed: " if ($cnt++)==0;
	$text .= "$a ";
    }
    $text .= "\n" if $cnt>0;
    $cnt=0;
    foreach $a (keys %Added)
    {
	$text .= "Added: " if ($cnt++)==0;
	$text .= "$a ";
    }
    $text .= "\n" if $cnt>0;
    if ($oldcnt > 0)
    {
	$oldmsg = $nomsg if $oldmsg eq "" || $oldmsg eq $rcsempty;
	return if $oldmsg eq $nomsg; # Truly, no point
	foreach $a (split(/\n/,$oldmsg))
	{
	    chomp $a;
	    $text .= "\t$a\n";
	}
    }
}

sub
    handlelogmsg
{
    local($time,$date,$user,$name,$dir,$ver)=@_;
    
#    print ">$dir/$name\n";
    $msg = getmsg($time, $date, $user, $name, $dir, $ver);
    if ($msg ne $oldmsg)
    {
	dumpold;
	new_dir($time,$date,$user,$dir) if $oldcnt > 0;
    }
#    print "Test: $msg";
    $oldmsg = $msg;
    $oldcnt++;
}

%known = ("M" => "Changed", "de" => "Deleted", "A" => "Added");	    
%Ignore = ();

foreach (@ARGV)
{
    $Ignore{$_}=1;
}

while (<STDIN>)
{
    if (/^(\w+)\s(\d\d)\/(\d\d)\s(\d\d)\:(\d\d) [\+\-]\d+\s(\S+)\s+(\S+)\s+(\S+)\s+(\S+)\s+==\s+(\S+)\s*$/)
	{
	    $act = $1;
	    $mo = $2;
	    $da = $3;
	    $hr = $4;
	    $mi = $5;
	    $user = $6;
	    $ver = $7;
	    $name = $8;
	    $dir = $9;

#	    print "Debug: $act $name $dir\n";
	    next if $Ignore{$name} == 1;
#	    print "Debug2: $act $name $dir\n";
	    if ($nowdir ne $dir)
	    {
		dumpold;
		new_dir("$hr:$mi","$da.$mo",$user,$dir);
	    }
# 	    $Changed{$name}=1 if $act eq "M";
# 	    $Deleted{$name}=1 if $act eq "de";
# 	    $Added{$name}=1 if $act eq "A";
	    handlelogmsg("$hr:$mi","$da.$mo",$user,$name,$dir,$ver);
	    if ($known{$act} eq "")
	    {
		print STDERR "Uknown mode: $act for $name ($dir)!\n";
	    }
	    else
	    {
		$ev = "\$$known{$act}\{\"$name\"\}=1;";
#		print "Eval: $ev\n";
		eval $ev;
	    }
#	    print "Debug3: $act $name $dir\n";
	}
	else
	{
	    print STDERR "Parse error: $_";
	}
}
#print "DumpOld:\n";
dumpold;
push(@data, $text);
while ($#data >= 0)
{
    $foo = pop(@data);
    print $foo;
}
