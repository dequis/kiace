#!/usr/bin/env python
# -*- coding: utf-8 -*-

# licensed under GNU GPL 2.0
# http://www.gnu.org/licenses/old-licenses/gpl-2.0.html

import sys
import time
import socket
import urllib
import urllib2

headers = { 'User-Agent' : 'Mozilla/4.0 (compatible; MSIE 5.5; Windows NT) ' \
    'Alexa MEGAUPLOAD' } #(?)

def doit(url):
    # first step
    text = step(url)

    if text.count("<!-- E#4 -->"):
        print " [x] File removed"
        return True
    elif text.count("<h1>Error</h1>"):
        print " [x] Rapidshare unknown error"
        return True

    try:
        size = int(text.split('<font style="color:#8E908F;">| ')[1] \
                       .split('KB')[0])
        print " [+] File size:", size / 1024, "MB"
    except:
        pass
    
    # second step
    url = text.split('<form id="ff" action="')[1].split('" method="post">')[0]
    text = step(url, {'dl.start': 'Free'})

    
    if text.count("already downloading a file."):
        print " [x] IP already downloading a file"
        return False
    
    elif text.count('<form name="dlf" '):
        url = text.split('<form name="dlf" action="')[1] \
                  .split('" method="post">')[0]
        print " [i] Direct link (may expire):"
        print url
        wait = int(text.split("var c=")[1].split("\n")[0].strip(" ;\r"))
        print " [i] Waiting", wait, "seconds..."
        time.sleep(wait)

        return download_url(url)

    else:
        print " [x] wtf? HTML dumped to wtf.log"
        open("wtf.log","w").write(text)
        return False
    return True

def step(url, data=None):
    if data:
        request = urllib2.Request(url, urllib.urlencode(data), headers)
    else:
        request = urllib2.Request(url, headers=headers)
    rc = urllib2.urlopen(request)
    
    text = ''
    while True:
        buffer = rc.read()
        if buffer == '':
            break
        text += buffer
    return text

def download_url(url):
    postdata = {'mirror': 'on', 'x': 40, 'y': 50}
    request = urllib2.Request(url, urllib.urlencode(postdata), headers)
    rc = urllib2.urlopen(request)
    filename = url.split('/')[-1]
    print " [i] Downloading to ", filename
    print
    local = open(filename, 'wb')
    size = 0
    lastsize = 0
    while True:
        buffer = rc.read(1024)
        if buffer == '':
            break
        size += len(buffer)
        local.write(buffer)
        if (size - 1024*8) > lastsize:
            sys.stdout.write(str(size/1024) + "kb downloaded...\r")
            lastsize = size
    
    print "Done! Open", filename
    return True

if __name__ == '__main__':
    if len(sys.argv) > 1:
        url = sys.argv[1]
    else:
        print "Rapid ésta (@karloz svn)"
        print "Usage: %s <url>" % sys.argv[0]
        raise SystemExit

    for url in sys.argv[1:]:
        if not doit(url):
            sys.exit(1)
