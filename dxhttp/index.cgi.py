#!/usr/bin/python

import os
import sys
import cgi
import glob
import traceback
import cStringIO as StringIO

from utils import pre
import config

print "Content-Type: text/html; charset=utf-8"
print

query_string = cgi.parse_qs(os.environ['QUERY_STRING'])

if 'mod' in query_string:
    buffer = StringIO.StringIO()
    try:
        utils.get_mod(query_string['mod'][0]).main(buffer, query_string)
        print buffer.getvalue()
    except:
        print pre(traceback.format_exc())
else:
    print pre("Available modules")
    print pre(utils.list_mods())
