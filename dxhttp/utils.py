import config

pre = lambda text: "<pre>%s</pre>" % text

def get_mod(name):
    return __import__('.'.join([config.APPDIR, name]), globals(), locals(), ['main'])

def list_mods():
    import glob, os
    return ', '.join([''.join(x.split("/")[1:]) for x in
            glob.glob(os.path.join(config.APPDIR, "*.py"))])

def get_qs(raw):
    return '?'.join(raw.split(" ")[1].split("?", 1)[1:])
