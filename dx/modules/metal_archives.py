import re
import sys
import urllib
import urllib2
from BeautifulSoup import BeautifulSoup
from urlparse import urlparse

import music

MA = 'http://www.metal-archives.com/'
MA_BAND = MA + 'band.php?'
MA_BAND_PARAMS = lambda id: urllib.urlencode({'id': id})

def best_album(args, internal=False):
    artist = search_band(args, internal=True)
    print
    soup = BeautifulSoup(urllib2.urlopen(MA_BAND + MA_BAND_PARAMS(artist)))
    releases = soup("a", href=re.compile("^/release.php"))
    list = []
    for release in releases:
        type = release.parent.parent.find("td", "album")
        reviews = release.parent.parent.find("a", href=re.compile("^/review.php"))
        if type and reviews:
            percent = int(reviews.string.split()[-1].strip(" %"))
            if int(reviews.string.split()[0]) == 1:
                # one review might be overrated
                percent *= 0.75

            list.append((release.string,
                         type.string.split()[-1],
                         percent))
    list.sort(key=lambda x: x[2], reverse=True) # x[2] is percent
    for release in list[:5]:
        print "%s (%s), reviews: %s%%" % release
    if len(list) > 5:
        print "...%s more" % (len(list) - 5)
    if internal:
        return list[0]

def get_best_album(args):
    artist = args[0]
    album, date, reviews = best_album(args, internal=True)
    print
    print "Getting %s (%s)" % (album, date)
    q = lambda x: '"%s"' % x
    music.search([q(artist), q(album)])

MA_SEARCH = MA + 'search.php?'
MA_SEARCH_PARAMS = lambda query: urllib.urlencode({'type': 'band', 'string': query})

def search_band(args, internal=False):
    artist = args[0]

    url = MA_SEARCH + MA_SEARCH_PARAMS(artist)
    rc = urllib2.urlopen(url)
    soup = BeautifulSoup(urllib2.urlopen(MA_SEARCH + MA_SEARCH_PARAMS(artist)))
    results = {}
    
    # if redirected
    script = soup.body.center.script
    if script and script.string.strip().startswith("location.href"):
        id = (script.string.split("band.php?id=")[1].split("'")[0])
        print "%s [%s]" % (artist, id)
        return id
    
    # a list of bands is shown
    for tag in soup.findAll("a", href=re.compile("^band.php")):
        id = int(tag['href'].split("?id=")[1])
        results[id] = tag.string

        if tag.string.lower() == artist.lower():
            # exact match, duh
            if not internal:
                print "%s [%s]" % (tag.string, id)
            return id

    if results:
        for id, name in results.iteritems():
            print "%s [%s]" % (name, id)
        return int(raw_input("Choose: "))
    else:
        print "Nothing found"
        sys.exit(1)

main = best_album
