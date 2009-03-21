#!/usr/bin/env python

import urllib
import xml.dom.minidom

API_KEY = "b25b959554ed76058ac220b7b2e0a026"
BASE_URL = "http://ws.audioscrobbler.com/2.0/?"

params = {
    "method": "tasteometer.compare",
    "type1": "user",
    "type2": "user",
    "limit": 200,
    "api_key": API_KEY,
}

def main(buffer, query_string):
    params['value1'] = query_string['v1'][0]
    params['value2'] = query_string['v2'][0]
    url = BASE_URL + urllib.urlencode(params)
    xmlstring = urllib.urlopen(url).read()
    doc = xml.dom.minidom.parseString(xmlstring)
    buffer.write("%s and %s\n\n" % (params['value1'], params['value2']))
    for artist in doc.getElementsByTagName("artist"):
        buffer.write(artist.getElementsByTagName("name")[0].firstChild.nodeValue.encode("utf-8") + "\n")


