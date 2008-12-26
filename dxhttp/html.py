import xml.dom.minidom

class Base:
    def __init__(self):
        self.document = xml.dom.minidom.getDOMImplementation() \
            .createDocument(None, 'html', None)

        # make it global
        xml.dom.minidom.Element.documentObject = self.document
        
        # <html>
        self.html = self.document.documentElement
        
        self.write_function = self.html.toxml

    def write(self, file):
        file.write(self.write_function())

# magic stuff

def tag(self, name, text=None, **kwds):
    tag = self.documentObject.createElement(name)
    if text:
        textnode = self.documentObject.createTextNode(text)
        tag.appendChild(textnode)

    for (name, value) in kwds.iteritems():
        tag.setAttribute(name, value)

    self.appendChild(tag)
    return tag

# did someone say "prototype"?
xml.dom.minidom.Element.tag = tag
del tag

