#!/usr/bin/python

import os
import sys
import cgi
import glob
import traceback
import cStringIO as StringIO

import utils
import config
from utils import pre

query_string = cgi.parse_qs(os.environ['QUERY_STRING'])
if 'plain' in query_string:
    print 'Content-Type: text/plain; charset=UTF-8'
    utils.pre = lambda text: text
else:
    print 'Content-Type: text/html; charset=UTF-8'
print

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
