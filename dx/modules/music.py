import re
import sys
import urllib
import urllib2
import subprocess
import simplejson
from BeautifulSoup import BeautifulSoup
from urlparse import urlparse

HEADERS = {'Referer': 'http://www.kiace.com.ar'}
BASEURL = 'http://ajax.googleapis.com/ajax/services/search/blogs?v=1.0&'
PARAMS = lambda q, start: urllib.urlencode({'q': q, 'start': start})

SUPPORTED = ['rapidshare', 'mediafire', 'megaupload', 'sharebee',
             'zshare', 'sendspace']
PREFERRED = ['zshare', 'mediafire', 'sendspace', 'sharebee']
_get_host = lambda x: urlparse(x).netloc.split(".")[-2:][0]

VERBOSE = False

def search(args):
    query = []
    for arg in args:
        if arg.count(" "):
            arg = '"%s"' % arg
        query.append(arg)
    query = ' '.join(query)

    start = 0
    links = []

    while not links:
        req = urllib2.Request(BASEURL + PARAMS(query, start), headers=HEADERS)
        json = simplejson.load(urllib2.urlopen(req))
        
        for result in json['responseData']['results']:
            if VERBOSE:
                print "Trying: ", result['titleNoFormatting']
                print " ", result['postUrl']

            try:
                soup = BeautifulSoup(urllib2.urlopen(result['postUrl']))
            except urllib2.HTTPError, e:
                if VERBOSE:
                    print e
                continue
            
            # try to narrow it to blogger's post area
            posts_area = soup.find("div", {"class": re.compile("blog-posts")})
            if posts_area:
                soup = posts_area

            link = soup.findAll("a", 
                href=lambda x: x and _get_host(x) in SUPPORTED)
            
            if link and len(link) == 1:
                href = link[0]['href']
                if _get_host(href) in PREFERRED:
                    links = [(href, result['postUrl'])]
                    break
                else:
                    links.append((href, result['postUrl']))
        start += 4
    print

    if links:
        href, post_url = links[0]
        subprocess.Popen(["firefox", href])
        print post_url
        print href
    else:
        print "Nothing found"

main = search
