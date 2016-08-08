#!/usr/bin/env python
# $Id: mux.py 1.1 02/01/03 01:03:26-00:00 twouters@ $
#
"""

Main mux stuff

@fo me=@python print int([get(me/x)])
=> 3 [&x me=3]
@fo me=@python print '[get(me/last)]'
=> Thu Sep 30 07:14:51 1999



"""

import muxc

import whrandom
import cPickle 
import types
import string

## Quick-hack security measures.
## XXX This should be done using Bastions or by overloading __import__.
## And it should use a whitelist, not a blacklist.

import sys
import empty_mod
sys.modules['os'] = empty_mod
sys.modules['posix'] = empty_mod
sys.modules['socket'] = empty_mod
sys.modules['select'] = empty_mod

import __builtin__
del __builtin__.open
del __builtin__.SystemExit

del __builtin__, empty_mod, sys


from muxc import *

###################################################################################
# Hooks

def load():
    # What to do right after db load
    pass

def save():
    # What to do right before db save
    pass

def update():
    # What to do each update
    pass



###################################################################################
# Utility functions

def function(str):
    return muxc.function(getCause(), str)

def getObject(dbref):
    return muxc.getObject(getRef(dbref))

def getPickleObject(dbref):
    return MUXPickleObject(getRef(dbref))

def notify(dbref, text):
    return muxc.notify(getRef(dbref), text)



def getRef(ref):
    # conver #num -> num, if supplied (otherwise just num
    if type(ref) == types.StringType:
        if len(ref) == 0: return -1
        try:
            if ref[0] == "#":
                return int(ref[1:])
            return int(ref)
        except ValueError:
            return -1
    return int(ref)

###################################################################################
# Wrapped MUX functions

def who():
    # Return list of the numeric (int) dbrefs of people online
    return map(getRef, string.split(function('lwho()')))

def contents(dbref):
    # Return list of contents of an object
    ref = getRef(dbref)
    str = function('lcon(#%d)' % ref)
    if str == None: return []
    return map(getRef, string.split(str))

def owner(dbref):
    return getRef(function('owner(#%d)' % getRef(dbref)))

###################################################################################
# Classes

PICKLE_MAGIC_CHAR='_'

class MUXPickleObject:
    def __init__(self, ref):
        obj = getObject(ref)
        self.__dict__['_o'] = obj # Inefficient, ugh
    def __repr__(self):
        return "<%s instance: dbref=#%d>" % (self.__class__.__name__, self.__dict__['_o'].dbref)
    def __getattr__(self, name):
        #if name[0] != PICKLE_MAGIC_CHAR: name = PICKLE_MAGIC_CHAR+name
        v = getattr(self.__dict__['_o'], name)
        if type(v) == types.StringType:
            return pickle.loads(v)
        return v
    def __setattr__(self, name, value):
        #if name[0] != PICKLE_MAGIC_CHAR: name = PICKLE_MAGIC_CHAR+name
        setattr(self.__dict__['_o'], name, pickle.dumps(value))


###################################################################################
# Have to demonstrate _something_ useful; therefore, here's a "guess the number" game:
#

"""
@create Parent:PythonObject=10
@Startup Parent:PythonObject=@fo me={&instance me=[get(me/class)]('[num(me)]')};@fo me=@python [get(me/instance)]

@create Parent:GuessTheNumberGame=5
@Desc Parent:GuessTheNumberGame=[pythonmethodcall(get(me/instance),desc)]
@Idesc Parent:GuessTheNumberGame=[pythonmethodcall(get(me/instance),desc)]
&GUESS Parent:GuessTheNumberGame=$guess *:think pythonmethodcall(get(me/instance),guess,%0)
&NEWGAME Parent:GuessTheNumberGame=$newgame:think pythonmethodcall(get(me/instance),newGame)
&CLASS Parent:GuessTheNumberGame=mux.GuessTheNumberGame
@set Parent:GuessTheNumberGame=INHERIT
@set Parent:GuessTheNumberGame=COMMANDS
@parent Parent:GuessTheNumberGame=Parent:PythonObject

@create TestBoard=10
@set TestBoard=INHERIT
@set TestBoard=COMMANDS
@parent TestBoard=Parent:GuessTheNumberGame

"""
class GuessTheNumberGame(MUXPickleObject):
    def __init__(self, ref):
        MUXPickleObject.__init__(self, ref)
        if not hasattr(self, 'number') or not hasattr(self, 'guesses'):
            # New instance
            self.newGame()
    def newGame(self):
        self.number = whrandom.randint(1,29)
        self.guesses = 0
        notify(self.__dict__['_o'].location, "** Guess the number game has been reset.")
    def desc(self):
        return """
Hello! What you are looking at is Guess The Number game!

It is the latest in the high tech gaming technology ;-)

Computer has picked a number in 1-29 range. Your mission, Jim, should
you choose to accept it, is to guess it. Computer'll provide some
hints about the direction you went wrong.

Usage:
  guess <number>
  newgame

Guesses used: %d.

""" % self.guesses
    def guess(self, number):
        self.guesses = self.guesses + 1
        ref = getCause()
        if number > self.number:
            notify(ref, "Smaller.. ")
        elif number < self.number:
            notify(ref, "Larger.. ")
        else:
            notify(ref, "Dead on! And it took just %d guesses! %s" % (
                self.guesses, {1:"AWESOME!", 2:"Not bad at all!"}.get(self.guesses, "Could do better, though.")))
            self.newGame()
