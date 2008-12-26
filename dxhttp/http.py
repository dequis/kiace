import BaseHTTPServer
import handler

class ReloadingHandler(BaseHTTPServer.BaseHTTPRequestHandler):
    def do_GET(self):
        reload(handler)
        handler.handle(self)

if __name__ == '__main__':
    server = BaseHTTPServer.HTTPServer(('', 8080), ReloadingHandler)
    server.serve_forever()
