import html

class Demo(html.Base):
    def __init__(self):
        html.Base.__init__(self)

        self.write_function = self.html.toprettyxml

        head = self.html.tag('head')
        head.tag('title', 'lol')
        body = self.html.tag('body')
        body.tag('div', id="lol").tag("h1", "lol")


def main(buffer, query_string):
    Demo().write(buffer)
