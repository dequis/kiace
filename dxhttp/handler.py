import html

def handle(self):
    self.send_response(200)
    self.send_header('Content-Type', 'text/plain; charset=UTF-8')
    self.end_headers()

    reload(html)
    Asd().write(self.wfile)

class Asd(html.Base):
    def __init__(self):
        html.Base.__init__(self)

        self.write_function = self.html.toprettyxml

        head = self.html.tag('head')
        head.tag('title', 'lol')
        body = self.html.tag('body')
        body.tag('div', id="lol").tag("h1", "lol")


