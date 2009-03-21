import os
import cgi
import glob
import traceback
import BaseHTTPServer

import utils
import config
from utils import pre

class Handler(BaseHTTPServer.BaseHTTPRequestHandler):
    def do_GET(self):
        self.send_response(200)
        
        query_string = cgi.parse_qs(utils.get_qs(self.raw_requestline))
        if 'plain' in query_string:
            self.send_header('Content-Type', 'text/plain; charset=UTF-8')
            utils.pre = lambda text: text
        else:
            self.send_header('Content-Type', 'text/html; charset=UTF-8')
        self.end_headers()

        if 'mod' in query_string:
            try:
                utils.get_mod(query_string['mod'][0]).main(self.wfile, query_string)
            except:
                self.wfile.write(pre(traceback.format_exc()))
        else:
            self.wfile.write(pre("Available modules\n" +
                utils.list_mods()))

if __name__ == '__main__':
    server = BaseHTTPServer.HTTPServer(('', config.PORT), Handler)
    server.serve_forever()
