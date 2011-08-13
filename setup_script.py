#!/usr/bin/python

import os

name = "BlackBox LinDev"

OUT = open(name, "w")

print >> OUT, "#!/bin/bash"

cwd = os.getcwd()

print >> OUT, "mkdir "+cwd+"/hide"
print >> OUT, "mv "+cwd+"/Lin "+cwd+"/hide/"
print >> OUT, "mv "+cwd+"/System "+cwd+"/hide/"
print >> OUT, "mv "+cwd+"/Host "+cwd+"/hide/"
print >> OUT, "mv "+cwd+"/Code "+cwd+"/hide/"

print >> OUT, "wine "+cwd+"/bb16rc6win/BlackBox.exe -use \"`winepath -w \"$PWD\"`\" $* &"

print >> OUT, "sleep 1"

print >> OUT, "mv "+cwd+"/hide/Lin "+cwd+"/"
print >> OUT, "mv "+cwd+"/hide/System "+cwd+"/"
print >> OUT, "mv "+cwd+"/hide/Host "+cwd+"/"
print >> OUT, "mv "+cwd+"/hide/Code "+cwd+"/"
print >> OUT, "rm "+cwd+"/hide -R"

os.system('chmod +x "'+name+'"')

os.system('mv "'+name+'" "'+os.environ['HOME']+'/.gnome2/nautilus-scripts/" ')
