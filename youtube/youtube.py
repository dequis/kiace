#!/usr/bin/env python

import sys
import urllib

class YoutubeError(Exception):
    pass

def get(url):
    rc = urllib.urlopen(url)
    raw = rc.read()
    try:
        args = raw.split("var swfArgs = {")[1].split("};")[0]
    except IndexError:
        raise YoutubeError("swfArgs not found")

    result = {}
    for arg in args.split(','):
        key, value = tuple(arg.split(":", 1))
        
        result[ key.strip('"\' ') ] = value.strip('"\' ')
    try:
        assert 'video_id' in result
        assert 't' in result
    except AssertionError:
        raise YoutubeError("Some required parameters in swfArgs "
            "couldn't be found.")

    redir = 'http://www.youtube.com/get_video?video_id=%(video_id)s&t=%(t)s' %\
        result
    return redir


def main():
    if len(sys.argv) < 2 or sys.argv[1].count("youtube.com/") != 1:
        sys.stderr.write("Usage: %s [http://www.]youtube.com/(blah)\n" % \
            sys.argv[0])
        return
    url = 'http://www.youtube.com/' + \
          sys.argv[1].split("youtube.com/", 1)[1]
    try:
        print get(url)
    except YoutubeError, description:
        sys.stderr.write('''Couldn't get FLV url
 - Check if the url is valid
 - Check if the video is viewable by anonymous users
 - Youtube could have changed/ofuscated the code.
  That would mean that this script is broken, sorry\n''')
        sys.stderr.write("Detailed error: " + str(description) + "\n")

if __name__ == '__main__':
    main()
